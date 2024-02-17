#include "flfloorview.h"
#include <QWheelEvent>
#include <QSvgRenderer>
//#include <glog/logging.h>
#include <QDebug>
#include "flobjectlineitem.h"
#include <QGraphicsItem>
#include <QTime>
#include "fldeviceitem.h"
#include "fllineitem.h"
#include "fljob.h"
#include <QFileDialog>
#include "FLData.pb.h"
#include "fljobprotol.h"
#include "fldlgserialwrite.h"
#include <iostream>
#include <fstream>
#include <QSettings>
#include <QMessageBox>
#include <QCoreApplication>
using namespace std;

const int ZC_ITEM_WIDTH = 48;
const int ZC_ITEM_HEIGHT = 48;

bool caseInsensitiveLessThan(const QGraphicsItem* s1, const QGraphicsItem* s2)
{
	FLObject* obj1 = qobject_cast<FLObject*>((QObject*)s1->data(ItemPointerData).toULongLong());
	FLObject* obj2 = qobject_cast<FLObject*>((QObject*)s2->data(ItemPointerData).toULongLong());

	auto n1 = obj1->getProperty("Name").value.toString();
	auto n2 = obj2->getProperty("Name").value.toString();

	return n1 < n2;
}

FLFloorView::FLFloorView(QWidget *parent)
	: QGraphicsView(parent)
{
	QGraphicsScene* pScene = new QGraphicsScene(this);
	
	setScene(pScene);	
	setMouseTracking(true);

	m_pFloorItem = nullptr;
	itemOnResize = nullptr;
	item_pressed = nullptr;
	m_nPressedCorner = -1;

	palette_ = new GradientPalette(DEFAULT_WIDTH);

	palette_->setColorAt(0.45, Qt::red);
	palette_->setColorAt(0.55, Qt::yellow);
	palette_->setColorAt(0.65, Qt::blue);
	palette_->setColorAt(0.85, Qt::cyan);
	palette_->setColorAt(1.0, Qt::green);
	setUpdatesEnabled(true);
	_x = 50;
	_y = 30;
	_moveable = true;

	timer = new QTimer(this);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(refresh_items()));
	timer->start(1000);

	WIO_cnt = 0;


	this->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(ShowContextMenu(const QPoint &)));

	_bInit = false;
}

FLFloorView::~FLFloorView()
{

}

void FLFloorView::ShowContextMenu(const QPoint &pos)
{
	QMenu contextMenu(tr("Context menu"), this);

	QAction action1(QIcon(":/firelink/Resources/icons/hand-icon.png"), "Moveable", this);
	connect(&action1, SIGNAL(triggered()), this, SLOT(toggleMoveable()));
	contextMenu.addAction(&action1);

	QAction action2(QIcon(":/firelink/Resources/icons/add-floor-icon.png"), "Change floor plan", this);
	connect(&action2, SIGNAL(triggered()), this, SLOT(changeFloorPlan()));
	contextMenu.addAction(&action2);

	QAction action3(QIcon(":/firelink/Resources/icons/add-floor-icon.png"), "Fit window", this);
	connect(&action3, SIGNAL(triggered()), this, SLOT(fit_window()));
	contextMenu.addAction(&action3);

	contextMenu.addSeparator();
	
	auto lt = scene()->selectedItems();
	for each (auto item in lt)
	{
		if (item == m_pFloorItem)
			lt.removeOne(item);
		if (qgraphicsitem_cast<QGraphicsLineItem*>(item) != nullptr)
		{
			lt.removeOne(item);
		}
	}
	if (lt.count() > 1)
	{
		QAction *action32 = new QAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), "Align Horizontal", this);
		contextMenu.addAction(action32);
		action32 = new QAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), "Align Vertical", this);
		contextMenu.addAction(action32);
		action32 = new QAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), "Grid", this);
		contextMenu.addAction(action32);
		action32 = new QAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), "Align Bottom", this);
		contextMenu.addAction(action32);
	}
	else
	{
		QGraphicsItem *item = itemAt(pos);

		auto obj = (FLObject*)item->data(ItemPointerData).toULongLong();
		if (qobject_cast<FLZoneControllor*>(obj))
		{
			QAction *action32 = new QAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), "config", obj);			
			connect(action32, SIGNAL(triggered()), this, SLOT(on_write_zc()));
			contextMenu.addAction(action32);

			QAction *action33 = new QAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), "Select all WIOs", obj);
			connect(action33, SIGNAL(triggered()), this, SLOT(on_select_wios()));
			contextMenu.addAction(action33);
		}
		else if (qobject_cast<FLWio*>(obj))
		{

		}
	}
	

	auto ret = contextMenu.exec(mapToGlobal(pos));
	if (ret == nullptr)
		return;
	auto x = 0;
	auto y = 0;
	if (lt.count() > 1)
	{
		QGraphicsItem *item = itemAt(pos);
		y = item->pos().y();
		x = item->pos().x();
	}
	if (ret->text() == "Align Horizontal")
	{
		for each (auto item in lt)
		{
			item->setPos(item->pos().x(),y);
			emit Item_Position_Changed(item);

			FLObject* obj = qobject_cast<FLObject*>((QObject*)item->data(ItemPointerData).toULongLong());
			if (obj)
				emit focusItemChanged(obj);
			UpdateItem(item);

		}
	}
	else if (ret->text() == "Align Vertical")
	{
		for each (auto item in lt)
		{
			item->setPos(x, item->pos().y());
			emit Item_Position_Changed(item);

			FLObject* obj = qobject_cast<FLObject*>((QObject*)item->data(ItemPointerData).toULongLong());
			if (obj)
				emit focusItemChanged(obj);
			UpdateItem(item);

		}
	}
	else if( ret->text() == "Grid")
	{
		qSort(lt.begin(), lt.end(), caseInsensitiveLessThan);

		QGraphicsItem *item = itemAt(pos);
		auto n_pos = item->pos();
		x = n_pos.x();
		y = n_pos.y();
		int n = sqrt(lt.count());
		if (n*n < lt.count())
			n = n + 1;
		for (int i = 0; i < n; i++)
		{			
			for (int j = 0; j < n; j++)
			{
				if (j + i*n >= lt.count())
					break;


				auto item = lt[j + i*n];

				item->setPos(x + j * 80, y + i* 80);
				emit Item_Position_Changed(item);

				FLObject* obj = qobject_cast<FLObject*>((QObject*)item->data(ItemPointerData).toULongLong());
				if (obj)
					emit focusItemChanged(obj);
				UpdateItem(item);
			}
			
		}

		m_pFloorItem->update();
		
		
	}
	else if (ret->text() == "Align Bottom")
	{
		for each (auto item in lt)
		{
			item->setPos(item->pos().x(), y);
			emit Item_Position_Changed(item);

			FLObject* obj = qobject_cast<FLObject*>((QObject*)item->data(ItemPointerData).toULongLong());
			if (obj)
				emit focusItemChanged(obj);
			UpdateItem(item);

		}
	}

	scene()->update();
}

void FLFloorView::mousePressEvent(QMouseEvent *event)
{
	if ( (event->modifiers() &= Qt::ShiftModifier) && (event->button() == Qt::LeftButton))
	{
		qDebug() << "shift + left button";
		auto p1 = mapToScene(event->pos());
		QRect rt(event->pos(), mapFromScene( mousePressPos.toPoint()));
		rt = rt.normalized();
		
		auto list = items(rt);
		for each (auto obj in list)
		{
			if (obj == m_pFloorItem)
				continue;
			obj->setSelected(true); 
		}
		return;
	}
	m_nPressedCorner = -1;
	itemOnResize = nullptr;
	item_pressed = nullptr;
	QPointF pos = (event->pos());
	mousePressPos = mapToScene(event->pos());
	QGraphicsItem *item = itemAt(pos.toPoint());
	QList<QGraphicsItem *> items_undermouse = items(pos.toPoint());

	if (qgraphicsitem_cast<QGraphicsLineItem*>(item) != nullptr)
	{
		QGraphicsView::mousePressEvent(event);
		return;
	}
	if (item == NULL)
	{
		QGraphicsView::mousePressEvent(event);
		return;
	}
	

	QRectF rt = item->sceneBoundingRect();
	QPointF itPos = item->scenePos();

	emit focusItemChanged((FLObject*)item->data(ItemPointerData).toULongLong());

	//rt.translate(itPos);
	m_nPressedCorner = PtInCorner(mousePressPos, rt);
	if (m_nPressedCorner != -1 && item == m_pFloorItem)
		itemOnResize = item;
	else
		setCursor(Qt::SizeAllCursor);
	item_pressed = item;
	//event->accept();
	QGraphicsView::mousePressEvent(event);
}

void FLFloorView::mouseReleaseEvent(QMouseEvent *e)
{
	m_nPressedCorner = -1;
	itemOnResize = nullptr;
	//mousePressPos = QPointF(-1, -1);
	QGraphicsView::mouseReleaseEvent(e);
}

void FLFloorView::mouseDoubleClickEvent(QMouseEvent *)
{

}

void FLFloorView::mouseMoveEvent(QMouseEvent *event)
{
	if (_moveable == false)
		return;
	if (event->buttons() == Qt::NoButton)
	{
		setCursor(Qt::ArrowCursor);
		QPointF pos = (event->pos());
		QGraphicsItem *item = itemAt(pos.toPoint());
		if (item == NULL)
		{
			QGraphicsView::mouseMoveEvent(event);
			return;
		}
		pos = mapToScene(event->pos());
		setCursor(Qt::OpenHandCursor);
		QRectF rt = item->sceneBoundingRect();

		int nPressedCorner = PtInCorner(pos, rt);
		if (nPressedCorner == -1)
			return;
		if (nPressedCorner == 1 || nPressedCorner == 2)
			setCursor(Qt::SizeBDiagCursor);
		else
			setCursor(Qt::SizeFDiagCursor);
		QGraphicsView::mouseMoveEvent(event);
		return;
	}

	// 	if(item_pressed)
	// 	{
	// 		emit Item_Position_Changed(item_pressed);			
	// 	}
	if (scene()->selectedItems().count() > 1)
	{
		if (event->buttons() |= Qt::LeftButton)
		{
			QGraphicsView::mouseMoveEvent(event);
			auto kt = scene()->selectedItems();
			for each (auto item in kt)
			{
				if (qgraphicsitem_cast<QGraphicsLineItem*>(item) != nullptr)
				{
					continue;
				}

				emit Item_Position_Changed(item);

				FLObject* obj = qobject_cast<FLObject*>((QObject*)item->data(ItemPointerData).toULongLong());
				if (obj)
					emit focusItemChanged(obj);
				UpdateItem(item);
			}
		}

		scene()->update();
		return;
	}

	if (event->buttons() != Qt::LeftButton || m_nPressedCorner == -1 || itemOnResize == nullptr)
	{
		QGraphicsView::mouseMoveEvent(event);
		QGraphicsItem *item = itemAt(event->pos());
		if (item)
		{
			if (qgraphicsitem_cast<QGraphicsLineItem*>(item) != nullptr)
			{
				QGraphicsView::mouseMoveEvent(event);
				return;
			}

			emit Item_Position_Changed(item);

			FLObject* obj = qobject_cast<FLObject*>((QObject*)item->data(ItemPointerData).toULongLong());
			if (obj)
				emit focusItemChanged(obj);
			UpdateItem(item);
		
		}

		//mapper_->reset_points();
		auto floor = qobject_cast<FLFloor*>((QObject*)m_pFloorItem->data(ItemPointerData).toULongLong());
		for (int n = 0; n< floor->countZC(); n++)
		{
			QPointF pt = floor->getZC(n)->getProperty("Position").value.toPointF();
			//mapper_->addPoint(pt.x()+ ZC_ITEM_WIDTH/2 ,pt.y() + ZC_ITEM_HEIGHT/2, floor->getZC(n)->getProperty("Radius").value.toInt());
		}
		m_pFloorItem->update();
		//item_heatmap->setPixmap( QPixmap::fromImage(*canvas_) );
		
		scene()->update();
		return;
	}


	QRectF bb = itemOnResize->boundingRect();
	//QPointF pos = event->pos();	
	QPointF pos = mapToScene(event->pos());
	float dx = pos.x() - mousePressPos.x();
	float dy = pos.y() - mousePressPos.y();
	QTransform t;

	pos = event->pos();
	pos = mapToScene(pos.toPoint());

	if (m_nPressedCorner == 0)
	{
		float xScale = (bb.width() - dx) / bb.width();
		float yScale = (bb.height() - dy) / bb.height();

		t.scale(xScale, yScale);
		itemOnResize->setTransform(t, true);
		itemOnResize->setPos(pos);
	}
	else if (m_nPressedCorner == 1)
	{
		float xScale = (bb.width() + dx) / bb.width();
		float yScale = (bb.height() - dy) / bb.height();

		t.scale(xScale, yScale);
		itemOnResize->setTransform(t, true);

		pos.setX(pos.x() - itemOnResize->sceneBoundingRect().width());
		itemOnResize->setPos(pos);
	}
	else if (m_nPressedCorner == 2)
	{
		float xScale = (bb.width() - dx) / bb.width();
		float yScale = (bb.height() + dy) / bb.height();

		t.scale(xScale, yScale);
		itemOnResize->setTransform(t, true);

		pos.setY(pos.y() - itemOnResize->sceneBoundingRect().height());
		itemOnResize->setPos(pos);
	}
	else
	{
		float xScale = (bb.width() + dx) / bb.width();
		float yScale = (bb.height() + dy) / bb.height();
		t.scale(xScale, yScale);
		itemOnResize->setTransform(t, true);
	}
	qDebug() << "pos = " << pos << "mouse in" << event->pos();

	mousePressPos = mapToScene(event->pos());
	emit Item_Position_Changed(itemOnResize);
	UpdateItem(itemOnResize);
	emit focusItemChanged((FLObject*)itemOnResize->data(ItemPointerData).toULongLong());
	event->accept();
	scene()->update();
}

FLGraphicsSvgItem * FLFloorView::CreateSVGFloor( FLFloor* pFloor,QString file)
{
	FLGraphicsSvgItem* pIt = new FLGraphicsSvgItem(file);
	pIt->setToolTip(pFloor->getProperty("Name").value.toString());
	//pIt->setSharedRenderer(renderer);
	pIt->setData(Qt::UserRole, "PreFloorItem");
	pIt->setData(ItemPointerData, quintptr((FLFloor*)pFloor));
	pIt->setFloor(pFloor);
	pIt->setVisible(true);

	if (pFloor->hasKey("Position"))
	{
		QPointF pt = pFloor->getProperty("Position").value.toPointF();
		pIt->setPos(pt);
	}
	else
	{
		pFloor->addProperty("Position", QPointF(0, 0), false);
	}
	scene()->addItem(pIt);
	return pIt;
}

void FLFloorView::wheelEvent(QWheelEvent* e)
{

	if (e->modifiers() & Qt::ControlModifier) {

		qreal numSteps = (float)e->delta() / 15.0 / 8.0;

		if (numSteps == 0)
		{
			e->ignore();
			return;
		}
		qreal factor = pow(1.25, numSteps);
		scale(1.0f / factor, 1.0f / factor);

		e->accept();
	}
	else {
		QGraphicsView::wheelEvent(e);
	}
}

void FLFloorView::toggleMoveable() 
{
	setMoveable(!_moveable);
}

void FLFloorView::changeFloorPlan() 
{
	
	QString file_svg = QFileDialog::getOpenFileName(this, "Choose a SVG file");
	if (file_svg != "")
		Floor()->addProperty("FloorGraph", file_svg);
}

void FLFloorView::setFloor(QPointer<FLFloor> pFloor)
{
	auto rt = rect();
	scene()->clear();
	list_line_items.clear();
	m_pFloorItem = nullptr;
	m_pFloorPixmap = nullptr;

	QString graph_svg = pFloor->getProperty("FloorGraph").value.toString();
	if (QFile::exists(graph_svg) == false || graph_svg.endsWith("svg") == false)
	{
		graph_svg = "graphics/sample_floor_plan.svg";
	}
	else
	{
		
		if (0 && graph_svg.endsWith("svg") == false)
		{

			QGraphicsPixmapItem* pIt = new QGraphicsPixmapItem(graph_svg);
			pIt->setToolTip(pFloor->getProperty("Name").value.toString());
			//pIt->setSharedRenderer(renderer);
			pIt->setData(Qt::UserRole, "PreFloorItem");
			pIt->setData(ItemPointerData, quintptr((FLFloor*)pFloor));
			
			pIt->setVisible(true);

			if (pFloor->hasKey("Position"))
			{
				QPointF pt = pFloor->getProperty("Position").value.toPointF();
				pIt->setPos(pt);
			}
			else
			{
				pFloor->addProperty("Position", QPointF(0, 0), false);
			}

			m_pFloorPixmap = pIt;
			scene()->addItem(pIt);
			connect(pFloor, SIGNAL(propertyChanged(QString)), this, SLOT(on_PropertyChanged(QString)));
			connect(pFloor, SIGNAL(childrenChanged()), this, SLOT(On_Children_Change(int)));

			int num_zc = pFloor->countZC();
			for (int n = 0; n< num_zc; n++)
			{
				AddZC(pFloor->getZC(n), n);
			}

			
		}
	}
	
 	m_pFloorItem = CreateSVGFloor(pFloor, graph_svg);
	scene()->addItem(m_pFloorItem);
	connect(pFloor,SIGNAL(propertyChanged(QString)),this,SLOT(on_PropertyChanged(QString)));
	connect(pFloor,SIGNAL(childrenChanged()),this,SLOT(On_Children_Change(int)));

	int num_zc = pFloor->countZC();
	for(int n=0; n< num_zc; n++)
	{
		AddZC(pFloor->getZC(n),n);
	}
	


}

void FLFloorView::AddZC(FLZoneControllor* pZC,int nIndex)
{
	connect(pZC,SIGNAL(propertyChanged(QString)),this,SLOT(on_PropertyChanged(QString)));

	QGraphicsPixmapItem *pixmap_item = new FLDeviceItem(QPixmap(":/firelink/Resources/icons/zc.png"));
	pixmap_item->setParentItem(m_pFloorItem);
	pixmap_item->setToolTip ( pZC->getProperty("Name").value.toString());

	pixmap_item->setFlag(QGraphicsItem::ItemIsMovable, true);
	pixmap_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	pixmap_item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	pixmap_item->setData(ItemPointerData, quintptr(pZC));
	//pixmap_item->setData(ItemPointerData, quintptr(pZC));
	pixmap_item->setZValue(m_pFloorItem->zValue()+2);
	scene()->addItem(pixmap_item);
	connect(pZC,SIGNAL(childrenChanged()),this,SLOT(On_Children_Change(int)));
	if(pZC->hasKey("Scale"))
	{
		QPointF scale = pZC->getProperty("Scale").value.toPointF();
		float x_scale = scale.x();
		float y_scale = scale.y();
		QTransform t;
		t.scale(x_scale,y_scale);
		pixmap_item->setTransform(t,true);

	}
	else
	{
		pZC->addProperty("Scale",QPointF(1.0,1.0),false);		
	}

	if(pZC->hasKey("Position"))
	{
		QPointF pt = pZC->getProperty("Position").value.toPointF();
		if(pt == QPointF(0,0))
		{
			pt = QPointF(_x,_y);			
		}
		pixmap_item->setPos(pt);
		
	}
	else
	{
		QPointF pt(15, 100* nIndex + 15);
		pixmap_item->setPos(pt);
		pZC->addProperty("Position",pt,false);
		
	
	}
	QPointF zc_pt = pZC->getProperty("Position").value.toPointF();
	//mapper_->addPoint(zc_pt.x() + ZC_ITEM_WIDTH/2, zc_pt.y() + ZC_ITEM_HEIGHT/2, pZC->getProperty("Radius").value.toInt());

	int num_wio = pZC->countWio();
	QGraphicsItem* item_start = pixmap_item;
	QGraphicsItem* item_end = nullptr;
	_x = 200;
	for(int n=0; n< num_wio; n++)
	{
		item_end = AddWIO( pZC->getWio(n),nIndex,n);
		FLObjectLineItem* item_line = new FLObjectLineItem(item_start,item_end,this);
		item_end->setZValue(m_pFloorItem->zValue()+3);
		scene()->addItem( item_line->GetLineItem());
		connect(this, SIGNAL(Item_Position_Changed(QGraphicsItem*)), item_line, SLOT(UpdateItem(QGraphicsItem*)));
		//item_start = item_end;
		list_line_items.append(item_line);
	}
	_y += 100;
	_x = 200;
}

QGraphicsItem* FLFloorView::AddWIO(FLWio* pWIO,int nRow,int nCol)
{
	QGraphicsPixmapItem *pixmap_item = new FLDeviceItem(QPixmap(":/firelink/Resources/icons/wio.png"));
	pixmap_item->setParentItem(m_pFloorItem);
	//scene()->addItem(pixmap_item);
	pixmap_item->setFlag(QGraphicsItem::ItemIsMovable, true);
	pixmap_item->setFlag(QGraphicsItem::ItemIsSelectable, true);
	pixmap_item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
	pixmap_item->setData(ItemPointerData, quintptr(pWIO));
	//pixmap_item->setZValue(m_pFloorItem->zValue()+1);
	QStringList vList = pWIO->getProperty("MAC Address").items;
	QString mac_obj = "FF:FF:FF:FF:FF:FF:FF:FF";
	if (vList.size() == 8)
	{
		mac_obj = QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(vList[0]).arg(vList[1]).arg(vList[2]).arg(vList[3]).arg(vList[4]).arg(vList[5]).arg(vList[6]).arg(vList[7]);
	}

	pixmap_item->setToolTip ( pWIO->getProperty("Name").value.toString()+"\r\nMAC: " + mac_obj);
	if(pWIO->hasKey("Scale"))
	{
		QPointF scale = pWIO->getProperty("Scale").value.toPointF();
		float x_scale = scale.x();
		float y_scale = scale.y();
		QTransform t;
		t.scale(x_scale,y_scale);
		pixmap_item->setTransform(t,true);
	}
	else
	{
		pWIO->addProperty("Scale",QPointF(1.0,1.0),false);	
	}

	if(pWIO->hasKey("Position"))
	{
		QPointF pt = pWIO->getProperty("Position").value.toPointF();
		if(pt == QPointF(0,0))
		{
			pt = QPointF(_x,_y);
			_x += 80;
		}

		pixmap_item->setPos(pt);
	}
	else
	{
		QPointF pt(180 + nCol*80, nRow* 80 + 80);
		pWIO->addProperty("Position",pt,false);
		pixmap_item->setPos(pt);
	}
	return pixmap_item;
}

FLFloor* FLFloorView::Floor()
{
	if (m_pFloorItem != nullptr)	
		return  m_pFloorItem->Floor();
	if (m_pFloorPixmap != nullptr)
	{
		auto obj = (QObject*)m_pFloorPixmap->data(ItemPointerData).toULongLong();
		return qobject_cast<FLFloor*>(obj);
	}
	return nullptr;
}

int FLFloorView::PtInCorner(QPointF pt, QRectF rt)
{
	if( PtInCorner(pt, rt.topLeft()))
		return 0;
	if(PtInCorner(pt,rt.topRight()))
		return 1;
	if(PtInCorner(pt,rt.bottomLeft()))
		return 2;
	if(PtInCorner(pt,rt.bottomRight()))
		return 3;
	return -1;
}

bool FLFloorView::PtInCorner(QPointF pt, QPointF ptCorner)
{
	QRectF corner( ptCorner.x() - 10,ptCorner.y() -10, 20,20);
	if(corner.contains(pt))
		return true;
	return false;

}

QGraphicsItem * FLFloorView::find_item(FLObject * obj)
{
	QList<QGraphicsItem*> pix_item_list = scene()->items();

	QGraphicsItem* zc_item = nullptr;;
	foreach(auto item, pix_item_list)
	{
		if ((QObject*)item->data(ItemPointerData).toULongLong() == obj)
		{
			return item;		
		}
	}
	return nullptr;
}

void FLFloorView::on_PropertyChanged(QString strKey)
{
	FLObject* pNode =qobject_cast<FLObject*>( sender());
	QString type = pNode->getProperty("Type").value.toString();
	if(pNode == nullptr)
	{
		//LOG(ERROR) << "Wrong object signal";
		return;
	}
	if(pNode->getProperty("Type").value.toString() == "FLOOR")
	{
		FLFloor* pFllor = (FLFloor*) pNode;
		if(strKey == "FloorGraph")
		{
			auto aaa = pNode->getProperty(strKey).value.toString();
			if (aaa.endsWith("svg") && m_pFloorItem)
			{
				if (m_pFloorItem) 
				{
					QSvgRenderer* pRender = new QSvgRenderer(pNode->getProperty(strKey).value.toString());
					m_pFloorItem->setSharedRenderer(pRender);
				}
				/*else
				{
					scene()->removeItem(m_pFloorPixmap);
					m_pFloorPixmap = nullptr;
					m_pFloorItem = CreateSVGFloor(pFllor,aaa);
				}*/
				
			}
			else if(m_pFloorPixmap)
			{
				//scene()->removeItem(m_pFloorItem);
				m_pFloorItem = nullptr;
				if (m_pFloorPixmap)
				{
					m_pFloorPixmap->setPixmap(aaa);
				}
			}
		}
	}
	else if(pNode->getProperty("Type").value.toString() == "ZC")
	{
		auto* obj = (FLZoneControllor*) pNode;
		if(strKey == "Range")
		{			
			m_pFloorItem->update();
		}
	}
}

void FLFloorView::UpdateItem(QGraphicsItem* item)
{
	FLObject* node = qobject_cast<FLObject*> ( (QObject*) item->data(ItemPointerData).toULongLong());
	if (node == nullptr)
		return;
	QPointF pos = item->pos();
	node->addProperty("Position",pos,false);
}

void FLFloorView::showEvent(QShowEvent * event)
{
	QGraphicsView::showEvent(event);
	if (_bInit == false && m_pFloorItem)
	{
		_bInit = true;
		auto rt = rect();
		auto rt_floot = m_pFloorItem->boundingRect();
		float h = rt.width() / rt_floot.width();
		float v = rt.height() / rt_floot.height();
		float scale1 = qMin(h, v);
	
		QTransform t;
		t.scale(scale1, scale1);
		//pIt->setTransform(t, true);
		scale(scale1, scale1);
		//update();
		qDebug() << "Floor view show event";
	}
}

void FLFloorView::on_data_Change(int mode)
{
	//add/delete/visible
	auto obj = (FLObject*)sender();
	auto type = obj->getProperty("Type").value.toString();
	switch(mode)
	{
	case 0://add
		if(type == "ZC")
		{
			auto floor = (FLFloor*)obj->parent();
			int num_zc = floor->countZC();
			AddZC((FLZoneControllor*)obj,num_zc);
		}
		else if (type == "WIO")
		{
			//get last wio of the zc.
			auto zc = (FLZoneControllor*) obj->parent();
			auto add_item =  AddWIO((FLWio*)obj,0,0);
			int num_wio = zc->countWio();
			//only one wio
			if(num_wio==1)
			{			
				return;
			}
			
		}
		break;
	case 1://delete;		
		break;
	case 2://visible
		break;
	}

}

void FLFloorView::DeleteFLObject(FLObject *obj)
{	
	auto type = obj->getProperty("Type").value.toString();
	QList<QGraphicsItem*> pix_item_list = scene()->items();
	//delete 
	if(type == "ZC")// delete zc
	{		
		RemoveZC((FLZoneControllor*)obj);
	}
	else if(type == "WIO")//delete wio
	{		
		FLWio* wio = (FLWio*)obj;
		RemoveWIO(wio);
	}		
}

void FLFloorView::RemoveZC(FLZoneControllor* pZC)
{
	auto floor = (FLFloor*)pZC->parent();
	auto zc =  pZC;
	int num_wio = zc->countWio();
	//remove lines;
	for(int n=0; n< num_wio; n++)
	{
		auto wio = zc->getWio(n);
		foreach(auto item,list_line_items)
		{
			if( (FLObject*)item->StartItem()->data(ItemPointerData).toULongLong() == wio
				|| (FLObject*)item->EndItem()->data(ItemPointerData).toULongLong() == wio )
			{
				scene()->removeItem(item->GetLineItem());
				list_line_items.removeOne(item);
				delete item;
			}
		}
	}
	//remove wio items
	QList<QGraphicsItem*> pix_item_list = scene()->items();
	pix_item_list = scene()->items();
	for(int n=0; n< num_wio; n++)
	{
		auto wio = zc->getWio(n);
		foreach (auto item,pix_item_list)
		{
			if( (FLObject*)item->data(ItemPointerData).toULongLong() == wio)
			{
				pix_item_list.removeOne(item);
				scene()->removeItem(item);
				break;
			}
		}
	}
	//remove zc item
	foreach (auto item,pix_item_list)
	{
		if( (FLObject*)item->data(ItemPointerData).toULongLong() == zc)
		{
			pix_item_list.removeOne(item);
			scene()->removeItem(item);
			break;
		}
	}
}

void FLFloorView::AddFLObject(FLObject *obj)
{
	auto obj_sender =(FLObject*) obj->parent ();//(FLObject*) sender();
	auto type = obj->getProperty("Type").value.toString();

	QList<QGraphicsItem*> pix_item_list = scene()->items();

	if(type == "ZC")
	{
		auto floor = (FLFloor*)obj_sender;
		int num_zc = floor->countZC()-1;
		auto zc = (FLZoneControllor*) obj;
		AddZC(zc ,num_zc);
	}
	else if(type == "WIO")//add wio
	{
		auto zc = (FLZoneControllor*) obj_sender;
		int num_wio = zc->countWio();
		auto wio = (FLWio*)obj;
		auto add_item =  AddWIO(wio,0,0);

		add_item->setZValue(m_pFloorItem->zValue() + 3);

		//find zc item;				
		auto zc_item = find_item(zc);
		if (zc_item == nullptr)
			return;
		auto pos_last = zc_item->pos() ;
		QPointF pos = pos_last + QPointF(150 + (zc->countWio()-1)%7 * 80, (zc->countWio()-1) /7 * 80 );

		add_item->setPos( pos );		//add_item->mapFromItem(last_2_item, pos)

		FLObjectLineItem* line_item = new FLObjectLineItem(add_item, zc_item,this);
		scene()->addItem( line_item->GetLineItem());
		connect(this, SIGNAL(Item_Position_Changed(QGraphicsItem*)), line_item, SLOT(UpdateItem(QGraphicsItem*)));
		list_line_items.append(line_item);
	}
	return;
	
}

void FLFloorView::on_FLObject_Change(FLObject *obj,int mode)
{
	auto obj_sender =(FLObject*) sender();
	auto type = obj_sender->getProperty("Type").value.toString();

	QList<QGraphicsItem*> pix_item_list = scene()->items();

	if(mode == 1) //add object
	{
		AddFLObject(obj);
		return;
	}

	//delete 
	if(type == "FLOOR")// delete zc
	{		
		auto floor = (FLFloor*)obj_sender;
		auto zc = (FLZoneControllor*) obj;
		RemoveZC(zc);
	}
	else if(type == "ZC")//delete wio
	{
		//get last wio of the zc.
		auto zc = (FLZoneControllor*) obj_sender;
		FLWio* wio = (FLWio*)obj;
		RemoveWIO(wio);
	}
	
}

void FLFloorView::on_write_zc( )
{
	auto zone_control = (FLZoneControllor*) sender()->parent();
	
	//open serial port
	FLDlgSerialWrite dlg;
	//dlg.set_data(data);
	dlg.set_zc(zone_control);
	dlg.exec();
}

void FLFloorView::on_select_wios()
{
	auto zc =qobject_cast<FLZoneControllor*> (sender()->parent());
	if (zc == nullptr)
		return;
	auto zc_item = find_item(zc);
	zc_item->setSelected(false);

	auto count = zc->countWio();
	for (size_t i = 0; i < count; i++)
	{
		auto wio = zc->getWio(i);
		auto item = find_item(wio);
		if(item)
			item->setSelected(true);
	}

}

void FLFloorView::create_new_job()
{
	//select path
	QString strPath = QCoreApplication::applicationDirPath();
	QString file_path = QFileDialog::getSaveFileName(this, "Select file to save", strPath, "bin file(*.bin)");
	if (file_path.isEmpty() == true)
		return;
	//create job
	FLJob* job = new FLJob(0);
	job->Init();
	job->addProperty("Name", "Unnamed Job");
	
	job->addProperty("JobPath", file_path);
	//create floor
	FLFloor* fl = m_pFloorItem->Floor();
	fl->addProperty("Name", fl->getProperty("Name").value.toString());
	job->addFloor(fl);
	//add zc to floor
	//add wio to zc
	FLJobProtol::SaveJob(job);
}

void FLFloorView::reset_rect(QRect rt)
{
	auto rt_floot = m_pFloorItem->boundingRect();
	float h = rt.width() / rt_floot.width();
	float v = rt.height() / rt_floot.height();
	float scale1 = qMax(h, v);

	QTransform t;
	t.scale(scale1, scale1);
	m_pFloorItem->setTransform(t, true);
	//scale(1/scale1, 1/scale1);
	_bInit = true;
}

void FLFloorView::fit_window()
{
	auto list = items();
	if (list.count() == 0)
		return;
	qreal x1, x2, y1, y2;
	x1 = x2 = list[0]->scenePos().x();
	y1 = y2 = list[0]->scenePos().y();

	for each (auto it in list )
	{
		x1 = qMin(it->scenePos().x(), x1);
		y1 = qMin(it->scenePos().y(), y1);
		x2 = qMax(it->scenePos().x(), x2);
		y2 = qMax(it->scenePos().y(), y2);
	}
	QRectF rt11 = sceneRect();
	QRect rt(0, 0, x2 - x1+100, y2 - y1+100);
	//translate(-x1, -y1);
	
	list[0]->setSelected(true);
	

	x1 -= 10;
	y1 -= 10;
	
	for each (auto it in list)
	{
		if (it == m_pFloorItem)
			continue;
		QPointF n_pos = it->scenePos() - QPointF(x1, y1);
		it->setPos(n_pos);
	}
	m_pFloorItem->setPos(10, 10);
	scene()->setSceneRect(rt);
	centerOn(m_pFloorItem);

	for each (auto line in list_line_items)
	{
		line->ResetLine();
	}

	update();

	scene()->update();
}

void FLFloorView::RemoveWIO(FLWio* wio)
{
	QList<QGraphicsItem*> pix_item_list = scene()->items();

	//get last wio of the zc.
	auto zc = (FLZoneControllor*)wio->parent();
	int num_wio = zc->countWio();

	foreach(auto item, list_line_items)
	{
		if ((FLObject*)item->StartItem()->data(ItemPointerData).toULongLong() == wio
			|| (FLObject*)item->EndItem()->data(ItemPointerData).toULongLong() == wio)
		{
			scene()->removeItem(item->GetLineItem());
			list_line_items.removeOne(item);
			delete item;
		}
	}


	//remove wio item

	pix_item_list = scene()->items();
	auto item = find_item(wio);
	if (item)
	{
		pix_item_list.removeOne(item);
		scene()->removeItem(item);
	}
	return;
}

void FLFloorView::setItemVisible(FLObject* obj, bool bVisible)
{
	QList<QGraphicsItem*> pix_item_list = scene()->items();
	foreach(auto item, pix_item_list)
	{
		auto item_obj = (FLObject*)item->data(ItemPointerData).toULongLong();
		if (item_obj == obj)
		{
			item->setVisible(bVisible);
			break;
		}
	}
}

void FLFloorView::refresh_items()
{
	if (m_pFloorItem == nullptr)
		return;
	//qDebug() << "Update view";

	auto list = scene()->items();
	QList<QGraphicsLineItem*> line_list;

	foreach(auto item, list)//get all line items;
	{
		auto line = qgraphicsitem_cast<QGraphicsLineItem*> (item);
		if (line)
		{
			line_list.append(line);
		}
	}
	foreach(auto item, list)
	{
		auto device = qgraphicsitem_cast<FLDeviceItem*> (item);
		if (device)
		{
			if (device->isActived())
			{
				device->updateItem();
				device->update();
			}
		}
		auto line = qgraphicsitem_cast<FLLineItem*> (item);
		if (line)
		{
			if (line->isActived())
			{
				line->setPen(line->getPen());
				line->update();
			}
		}
	}
	m_pFloorItem->update();
	update();
}


#pragma region Network_view


void FLFloorView::link_wio(QString src, int num1, QString dest, int num2)
{
	auto wio_src = find_item("", src);
	auto wio_dest = find_item("", dest);
	if (wio_src == nullptr || wio_dest == nullptr)
		return;
	Edge * p = nullptr;
	for each (auto edge in list_edge_items)
	{
		if (edge->sourceNode() == wio_src && edge->destNode() == wio_dest)
		{
			p = edge;
			edge->set_number(QString::number(num1), QString::number(num2));
			return;
		}

		if (edge->sourceNode() == wio_dest && edge->destNode() == wio_src)
		{
			p = edge;
			edge->set_number(QString::number(num2), QString::number(num1));
			return;
		}
	}

	p = new Edge(wio_src, wio_dest);
	p->set_number(QString::number(num1), QString::number(num2));
	list_edge_items.append(p);
	scene()->addItem(p);

}


FLDeviceItem * FLFloorView::find_item(QString mac, QString sMac)
{
	auto list = scene()->items();
	foreach(auto item, list)
	{
		auto device = qgraphicsitem_cast<FLDeviceItem*> (item);
		if (device == nullptr)
			continue;

		auto obj = qobject_cast<FLObject*>((QObject*)device->data(ItemPointerData).toULongLong());
		if (obj == nullptr)
			continue;

		if (obj->property("Short Address").isNull() == false)
		{
			if (obj->property("Short Address").toString().toUpper() == sMac.toUpper())
				return device;
		}
		if (obj->getProperty("Name").value.toString().toUpper() == sMac.toUpper())
			return device;
		if (mac == "")
			continue;
		QStringList vList = obj->getProperty("MAC Address").items;
		QString mac_obj = "";
		if (vList.size() == 8)
			mac_obj = QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(vList[0]).arg(vList[1]).arg(vList[2]).arg(vList[3]).arg(vList[4]).arg(vList[5]).arg(vList[6]).arg(vList[7]);
		if (mac.toUpper() == mac_obj.toUpper())
			return device;

	}
	if (WIO_cnt > 51)
	{
		qDebug() << "count ";
	}
	return nullptr;
}

void  FLFloorView::active_lineitem2WIO(FLWio* wio, int msec)
{
	if (wio == nullptr)
		return;

	foreach(auto line, list_line_items)
	{
		if ((FLObject*)line->StartItem()->data(ItemPointerData).toULongLong() == wio
			|| (FLObject*)line->EndItem()->data(ItemPointerData).toULongLong() == wio)
		{
			((FLLineItem*)line->GetLineItem())->setTimer(msec, Qt::red);
		}
	}

}

void FLFloorView::activeDevice(QString mac, int msec, QString ShrtAdd, QString info, QColor clr, QColor idleclr)
{
	bool found = false;
	qDebug() << "activeDevice" << mac << ShrtAdd;

	if (mac.compare("FF:FF:FF:FF:FF:FF:FF:FF", Qt::CaseInsensitive) == 0)
		return;


	auto device = find_item(mac, ShrtAdd);
	int nindex = _short_mac.indexOf(ShrtAdd);
	if (nindex != -1 && device == nullptr)
	{
		qDebug() << "wrong activeDevice Adding WIO " << mac << ShrtAdd << WIO_cnt;
	}
	if (device == nullptr)
	{
		_short_mac << ShrtAdd;
		_long_mac << mac;
		//Q_ASSERT(_short_mac.contains(ShrtAdd) == false);
		//Q_ASSERT(_long_mac.contains(mac) == false);
		
		qDebug()<<"activeDevice Adding WIO "<< mac << ShrtAdd  <<WIO_cnt;
		FLWio* wio = new FLWio(0);
		wio->Init();
		wio->setProperty("Short Address", ShrtAdd);

		wio->addProperty("Name", ShrtAdd);
		wio->setMacAddress(mac);

		device = AddWIOToFloor(Floor()->getZC(0),wio, WIO_cnt);
		WIO_cnt++;
		update();	
	}

	auto obj = qobject_cast<FLObject*>((QObject*)device->data(ItemPointerData).toULongLong());
	auto wio = qobject_cast<FLWio*>(obj);
	
	device->setTimer(msec, clr, DeviceStatus::Device_Inactive);
	QString text = QDate::currentDate().toString("yyyy-MM-dd") + "-" + QTime::currentTime().toString();
	device->setToolTip(obj->getProperty("Name").value.toString() + "\r\nMAC: " + mac + "\r\nSAD: " + ShrtAdd + "\r\nLRT: " + text + "\r\nInfo: " + info);
	device->setSelected(true);
}

void FLFloorView::activeDevice(FLObject * obj, int msec,QString info, DeviceStatus type,QString mac, QString ShrtAdd )
{
	//if (qobject_cast<FLZoneControllor*>(obj))
	//	return;

	auto dev = find_item(obj);
	if (dev == nullptr)
		return;
	FLDeviceItem* device = (FLDeviceItem*)dev;
	if (type == DeviceStatus::Device_LinkStatus)
	{
		//link_wio()
	}
	device->setTimer(msec, Qt::red, type);
	QString text = QDate::currentDate().toString("\r\nLRT:yyyy-MM-dd") + "-" + QTime::currentTime().toString();
	QString tip = obj->getProperty("Name").value.toString() + "\r\nMAC: " + mac + "\r\nSAD: " + ShrtAdd + text + "\r\nInfo: " + info;
	device->setToolTip(tip);

}

FLDeviceItem* FLFloorView::AddWIOToFloor(FLZoneControllor*zc,  FLWio* pWIO, int pos)
{
	auto floor = Floor();
	auto n = floor->countZC();
	zc->addWio(pWIO);
	int num_wio = zc->countWio();
	
	auto add_item = AddWIO(pWIO, (num_wio - 1) / 7, (num_wio - 1) % 7);

	add_item->setZValue(m_pFloorItem->zValue() + 3);

	//find zc item;
	QList<QGraphicsItem*> pix_item_list = scene()->items();

	QGraphicsItem* zc_item = nullptr;
	zc_item = find_item(zc);

	FLObjectLineItem* line_item = new FLObjectLineItem(add_item, zc_item, this);
	scene()->addItem(line_item->GetLineItem());
	connect(this, SIGNAL(Item_Position_Changed(QGraphicsItem*)), line_item, SLOT(UpdateItem(QGraphicsItem*)));
	list_line_items.append(line_item);

	if (pWIO->getProperty("Name").value.toString() == "")
		pWIO->addProperty("Name", "Unnamed WIO");

	return (FLDeviceItem*)add_item;

}

void FLFloorView::set_current_object(FLObject * obj)
{
	QList<QGraphicsItem*> pix_item_list = scene()->items();
	foreach(auto item, pix_item_list)
	{
		auto item_obj = (FLObject*)item->data(ItemPointerData).toULongLong();
		if (item_obj == obj)
		{
			item->setSelected(true);
			//break;
		}
		else
			item->setSelected(false);
	}
}

#pragma endregion
