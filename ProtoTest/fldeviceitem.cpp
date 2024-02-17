#include "fldeviceitem.h"
#include <QWidget>
#include <QPainter>
#include <QDebug>

#include "flobject.h"

#include "flwio.h"
#include "flzonecontrollor.h"

FLDeviceItem::FLDeviceItem(const QPixmap & pixmap, QGraphicsItem * parent )
	: QGraphicsPixmapItem(pixmap,parent)
{
	_length_time = -1;
	off_set = 6;
	
	_idleclr = QColor(0, 255, 255);
	//setFlag(ItemSendsGeometryChanges);
	//_normal = pixmap;
	//_active = QPixmap(":/pro/Resources/icons/wio_active.png");

	//QTimer *timer = new QTimer(this);
	//connect(timer, SIGNAL(timeout()), this, SLOT(processOneThing()));
	//timer->start();
}

FLDeviceItem::~FLDeviceItem()
{

}
void FLDeviceItem::addEdge(Edge *edge)
{
	edgeList << edge;
	edge->adjust();
}

QList<Edge *> FLDeviceItem::edges() const
{
	return edgeList;
}

QVariant FLDeviceItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) {
	case ItemPositionHasChanged:
		foreach(Edge *edge, edgeList)
			edge->adjust();
		//graph->itemMoved();
		break;
	default:
		break;
	};

	return QGraphicsItem::itemChange(change, value);
}

void FLDeviceItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{	
	off_set = _length_time% 8;
	if (isSelected())
	{
		QRectF rt = boundingRect();
		painter->save();
		painter->setBrush(Qt::cyan);
		//painter->fillRect(rt, Qt::red);
		painter->drawEllipse(-5 , -5 , rt.width() +10, rt.height()+10);
		painter->restore();
	}

	//if (_length_time <=0)	
	//	setPixmap(_normal);	
	//_length_time -= 2;

	QGraphicsPixmapItem::paint(painter, option, widget);

	auto obj = qobject_cast<FLObject*>((QObject*)data(ItemPointerData).toULongLong());
	if (obj != nullptr)
	{
		QRectF rt = boundingRect();
		rt.setHeight(rt.height() + 15);
		painter->drawText(rt, obj->getProperty("Name").value.toString(), Qt::AlignJustify| Qt::AlignBottom);
	}
	
}

void FLDeviceItem::setTimer(int msec, QColor clr, DeviceStatus type)
{	
	//todo: deal with index;

	int index = (int)type;
	static QList<QPixmap> wio_img_list = { QPixmap(":/pro/Resources/icons/wio_Inactive.png"),
		QPixmap(":/pro/Resources/icons/wio_MAC.png") ,
		QPixmap(":/pro/Resources/icons/wio_NWK.png") ,
		QPixmap(":/pro/Resources/icons/wio_BC.png") ,
		QPixmap(":/pro/Resources/icons/wio_RX.png"),
		QPixmap(":/pro/Resources/icons/wio_TX.png") ,
		QPixmap(":/pro/Resources/icons/wio_Leave.png") ,
		QPixmap(":/pro/Resources/icons/wio_BroadCast.png") ,
		QPixmap(":/pro/Resources/icons/wio_AssociationRequest.png") ,
		QPixmap(":/pro/Resources/icons/wio_LinkStatus.png")  };

	static QList<QPixmap> zc_img_list = { QPixmap(":/pro/Resources/icons/zc_Inactive.png"),
		QPixmap(":/pro/Resources/icons/zc_MAC.png") ,
		QPixmap(":/pro/Resources/icons/zc_NWK.png") ,
		QPixmap(":/pro/Resources/icons/zc_BC.png") ,
		QPixmap(":/pro/Resources/icons/zc_RX.png"),
		QPixmap(":/pro/Resources/icons/zc_TX.png") ,
		QPixmap(":/pro/Resources/icons/zc_Leave.png") ,
		QPixmap(":/pro/Resources/icons/zc_BroadCast.png") ,
		QPixmap(":/pro/Resources/icons/zc_AssociationRequest.png") ,
		QPixmap(":/pro/Resources/icons/zc_LinkStatus.png") };
	//todo , add more picture;
	if (index >= 9)
		index = 9;

	QObject* obj =(QObject*) data(ItemPointerData).toULongLong();
	auto wio = qobject_cast<FLWio*>(obj);
	if (wio)
	{
		if (_length_time <= 0 && msec>0)
			setPixmap(wio_img_list[index]);
		else if (_length_time <= 0)
			setPixmap(wio_img_list[0]);
	}
	else
	{
		if (_length_time <= 0 && msec>0)
			setPixmap(zc_img_list[index]);
		else if (_length_time <= 0)
			setPixmap(zc_img_list[0]);
	}
	
		
	_length_time = msec;
	_total_time = msec;
	_iclr = clr;
	

}

void FLDeviceItem::updateItem(int value)
{
	static QPixmap _normal_wio = QPixmap(":/pro/Resources/icons/wio_Inactive.png");
	static QPixmap _normal_zc = QPixmap(":/pro/Resources/icons/zc_Inactive.png");
	//static QPixmap _active = QPixmap(":/pro/Resources/icons/wio_active.png");
	_length_time -= value;
	QObject* obj = (QObject*)data(ItemPointerData).toULongLong();
	auto wio = qobject_cast<FLWio*>(obj);
	if (wio)
	{
		if (_length_time <= 0)
			setPixmap(_normal_wio);
	}
	else
	{
		if (_length_time <= 0)
			setPixmap(_normal_zc);
	}
	
}
