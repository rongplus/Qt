#include "fltreewidget.h"

#include <QMenu>
#include <QContextMenuEvent>


FLTreeWidget::FLTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	setHeaderHidden(true);
	//setExpandsOnDoubleClick(true);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
}

FLTreeWidget::~FLTreeWidget()
{

}

void FLTreeWidget::addNode(FLJob* pJob)
{
	QTreeWidgetItem *jobItem = new QTreeWidgetItem();
	
	jobItem->setIcon(0,QIcon(":/pro/Resources/icons/dialog-icon.png"));
	jobItem->setData(0,ItemTypeData,QString("JOB"));
	jobItem->setData(0,ItemPointerData,(quintptr(pJob)));
	jobItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	jobItem->setText(0,pJob->getProperty("Name").value.toString());
	addTopLevelItem(jobItem);

	int nFloors = pJob->countFloor();
	for(int n=0; n< nFloors; n++)
		addNode(jobItem,pJob->getFloor(n));
	connect(pJob, SIGNAL(propertyChanged(QString)), this, SLOT(on_propertyChanged(QString)));
}

void FLTreeWidget::addNode(FLFloor* pJob)
{

}

void FLTreeWidget::addNode(FLWio* pJob)
{

}

void FLTreeWidget::addNode(FLObject* pJob)
{

}


void FLTreeWidget::addNode(QTreeWidgetItem *pParentItem,FLFloor* pFloor)
{
	QTreeWidgetItem *floorItem = new QTreeWidgetItem();
	//floorItem->setCheckState(0,Qt::Checked);
	//floorItem->setCheckState(0,Qt::Unchecked);
	floorItem->setIcon(0,QIcon(":/pro/Resources/icons/network.png"));
	floorItem->setData(0,ItemTypeData,QString("FLOOR"));
	floorItem->setData(0,ItemPointerData,(quintptr(pFloor)));
	floorItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	floorItem->setText(0,pFloor->getProperty("Name").value.toString());
	pParentItem->addChild(floorItem);

	int nZC = pFloor->countZC();
	for(int n=0; n< nZC; n++)
		addNode(floorItem,pFloor->getZC(n));
	connect(pFloor, SIGNAL(propertyChanged(QString)), this, SLOT(on_propertyChanged(QString)));
}

void FLTreeWidget::addNode(QTreeWidgetItem *pParentItem,FLWio* pWio)
{
	QTreeWidgetItem *wioItem = new QTreeWidgetItem();
	//wioItem->setCheckState(0,Qt::Checked);
	//wioItem->setCheckState(0,Qt::Unchecked);
	wioItem->setIcon(0,QIcon(":/pro/Resources/icons/wio.png"));
	wioItem->setData(0,ItemTypeData,QString("WIO"));
	wioItem->setData(0,ItemPointerData,(quintptr(pWio)));
	wioItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	wioItem->setText(0,pWio->getProperty("Name").value.toString());
	pParentItem->addChild(wioItem);
	connect(pWio, SIGNAL(propertyChanged(QString)), this, SLOT(on_propertyChanged(QString)));

}

void FLTreeWidget::addNode(QTreeWidgetItem *pParentItem,FLObject* pObj)
{
	QString strIcon = "wio.png";
	QString type = pObj->getProperty("Type").value.toString();
	QString title = pObj->getProperty("Name").value.toString();
	if(type == "ZC_STATS")
		strIcon = "sys-stat-icon-48";
	else if(type== "ZC_REMOTE_TROUBLE")
		strIcon = "trouble-icon-48";
	else if(type == "ZC_LOCAL_TROUBLES")
		strIcon = "trouble-icon-48";
	else if(type == "ZC_RELAY")
		strIcon = "relay-icon-48.png";
	else if (type == "ZC_INPUTS")
	{
		strIcon = "input-icon-48.png";
		QString tag = pObj->getProperty("Tag").value.toString();
		if(tag != "")
			title += "\\" +tag;
	}


	QTreeWidgetItem *ioItem = new QTreeWidgetItem();

	ioItem->setIcon(0,QIcon(":/pro/Resources/icons/" + strIcon));
	ioItem->setData(0,ItemTypeData,QString("OBJECT"));
	ioItem->setData(0,ItemPointerData,(quintptr(pObj)));
	ioItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	ioItem->setText(0, title);
	pParentItem->addChild(ioItem);
	connect(pObj, SIGNAL(propertyChanged(QString)), this, SLOT(on_propertyChanged(QString)));
}

void FLTreeWidget::addNode(QTreeWidgetItem *pParentItem, FLZoneControllor* pZC)
{
	QTreeWidgetItem *zcItem = new QTreeWidgetItem();
	//zcItem->setCheckState(0,Qt::Checked);
	//zcItem->setCheckState(0,Qt::Unchecked);
	zcItem->setIcon(0,QIcon(":/pro/Resources/icons/zc.png"));
	zcItem->setData(0,ItemTypeData,QString("ZONECONTROL"));
	zcItem->setData(0,ItemPointerData,(quintptr(pZC)));
	zcItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	zcItem->setText(0,pZC->getProperty("Name").value.toString());
	pParentItem->addChild(zcItem);

	int nIO = pZC->countIO();

	QTreeWidgetItem *inputItem = new QTreeWidgetItem();
	inputItem->setIcon(0,QIcon(":/pro/Resources/icons/input-icon-48.png"));
	zcItem->setData(0,ItemTypeData,QString("NULL"));
	/*inputItem->setData(0,ItemPointerData,(quintptr(pZC)));*/
	inputItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	inputItem->setText(0,"Input");
	zcItem->addChild(inputItem);

	QTreeWidgetItem *outputItem = new QTreeWidgetItem();
	outputItem->setIcon(0,QIcon(":/pro/Resources/icons/relay-icon-48.png"));
	outputItem->setData(0,ItemTypeData,QString("NULL"));
	/*outputItem->setData(0,ItemPointerData,(quintptr(pZC)));*/
	outputItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	outputItem->setText(0,"RELAYS");
	zcItem->addChild(outputItem);

	QTreeWidgetItem *localTrItem = new QTreeWidgetItem();
	localTrItem->setIcon(0,QIcon(":/pro/Resources/icons/trouble-icon-48.png"));
	localTrItem->setData(0,ItemTypeData,QString("NULL"));
	/*localTrItem->setData(0,ItemPointerData,(quintptr(pZC)));*/
	localTrItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	localTrItem->setText(0,"LOCAL TROUBLES");
	zcItem->addChild(localTrItem);


	QTreeWidgetItem *remoteTrItem = new QTreeWidgetItem();

	remoteTrItem->setIcon(0,QIcon(":/pro/Resources/icons/trouble-icon-48.png"));
	/*remoteTrItem->setData(0,ItemPointerData,(quintptr(pZC)));*/
	remoteTrItem->setData(0,ItemTypeData,QString("NULL"));
	remoteTrItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	remoteTrItem->setText(0,"Remote TROUBLES");
	zcItem->addChild(remoteTrItem);

	QTreeWidgetItem *sysStateItem = new QTreeWidgetItem();

	sysStateItem->setIcon(0,QIcon(":/pro/Resources/icons/sys-stat-icon-48.png"));
	/*sysStateItem->setData(0,ItemPointerData,(quintptr(pZC)));*/
	sysStateItem->setData(0,ItemTypeData,QString("NULL"));
	sysStateItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	sysStateItem->setText(0,"System Status");
	zcItem->addChild(sysStateItem);
	
	for(int n=0; n< nIO; n++)
	{
		QString type = pZC->getIO(n)->getProperty("Type").value.toString();
		if(type == "ZC_STATS")
			addNode(sysStateItem,pZC->getIO(n));
		else if(type == "ZC_REMOTE_TROUBLE")
			addNode(remoteTrItem,pZC->getIO(n));
		else if(type == "ZC_LOCAL_TROUBLES")
			addNode(localTrItem,pZC->getIO(n));
		else if(type == "ZC_RELAY")
			addNode(outputItem,pZC->getIO(n));
		else if(type == "ZC_INPUTS")
			addNode(inputItem,pZC->getIO(n));	
		else 
			addNode(zcItem,pZC->getIO(n));
	}


	QTreeWidgetItem *wioItem = new QTreeWidgetItem();

	wioItem->setIcon(0,QIcon(":/pro/Resources/icons/wio.png"));
	/*wioItem->setData(0,ItemPointerData,(quintptr(pZC)));*/
	wioItem->setData(0,ItemTypeData,QString("NULL"));
	wioItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	wioItem->setText(0,"WIO");
	zcItem->addChild(wioItem);

	int nWio = pZC->countWio();
	for(int n=0; n< nWio; n++)
	{		
		addNode(wioItem,pZC->getWio(n));
	}
	connect(pZC, SIGNAL(propertyChanged(QString)), this, SLOT(on_propertyChanged(QString)));
}

void FLTreeWidget::deleteNode(FLJob* pJob)
{
	int num_topitem = topLevelItemCount();
	for(int n=0; n< num_topitem; n++)
	{
		auto item = topLevelItem(n);
		if( (QObject*)item->data(0,ItemPointerData).toULongLong() == pJob)
		{
			takeTopLevelItem(n);
			return;
		}
	}

}

void FLTreeWidget::deleteNode(FLFloor* pFloor)
{
	auto parent_item = parentItem(pFloor);
	int num_children = parent_item->childCount();
	for (int n=0; n< num_children; n++)
	{
		if( (QObject*)parent_item->child(n)->data(0,ItemPointerData).toULongLong() == pFloor)
		{
			parent_item->takeChild(n);
			return;
		}
	}
}

void FLTreeWidget::deleteNode(FLWio* pWio)
{
	auto zc_item =  parentItem(pWio);
	auto parent_item = zc_item->child(5);
	int num_children = parent_item->childCount();
	for (int n=0; n< num_children; n++)
	{
		if( (QObject*)parent_item->child(n)->data(0,ItemPointerData).toULongLong() == pWio)
		{
			parent_item->takeChild(n);
			return;
		}
	}
}

void FLTreeWidget::deleteNode(FLZoneControllor* pObj)
{
	auto parent_item = parentItem(pObj);
	int num_children = parent_item->childCount();
	for (int n=0; n< num_children; n++)
	{
		if( (QObject*)parent_item->child(n)->data(0,ItemPointerData).toULongLong() == pObj)
		{
			parent_item->takeChild(n);
			return;
		}
	}
}

void FLTreeWidget::on_propertyChanged(QString str)
{
	if (str == "Name")
	{
		FLObject* obj = (FLObject*)sender();
		auto obj1 = qobject_cast<FLJob*>(obj);
		if (obj1)
		{
			for (size_t i = 0; i < topLevelItemCount(); i++)
			{
				auto top = topLevelItem(i);
				if ( (FLObject*)top->data(0,ItemPointerData).toULongLong() == obj)
				{
					top->setText(0, obj->getProperty(str).value.toString());
					return;
				}
			}
			return;
		}
		auto obj2 = qobject_cast<FLFloor*>(obj);
		if (obj2)
		{
			auto itemt = item(obj2);
			itemt->setText(0, obj->getProperty(str).value.toString());
			return;
		}

		auto obj3 = qobject_cast<FLZoneControllor*>(obj);
		if (obj3)
		{
			auto itemt = item(obj3);
			itemt->setText(0, obj->getProperty(str).value.toString());

			return;
		}

		auto obj4 = qobject_cast<FLWio*>(obj);
		if (obj4)
		{
			auto itemt = item(obj4);
			itemt->setText(0, obj->getProperty(str).value.toString());
			return;
		}
	}
	else if (str == "Tag")
	{
		FLObject* obj = (FLObject*)sender();
	
		auto obj4 = qobject_cast<FLObject*>(obj);
		if (obj4)
		{
			auto title = obj4->getProperty("Name").value.toString();
			auto itemt = item(obj4);
			QString tag = obj4->getProperty("Tag").value.toString();
			if (tag != "")
				title += "\\" + tag;
			itemt->setText(0, title);
			return;
		}
	}
	
}

void FLTreeWidget::deleteNode(FLObject* pObj)
{
	auto job =qobject_cast<FLJob*> (pObj);
	if(job)
	{
		deleteNode(job);
		return;
	}
	auto floor = qobject_cast<FLFloor*>(pObj);
	if(floor)
	{
		deleteNode(floor);
		return;
	}
	auto zc = qobject_cast<FLZoneControllor*>(pObj);
	if(zc)
	{
		deleteNode(zc);
		return;
	}
	auto wio = qobject_cast<FLWio*>(pObj);
	if(wio)
	{
		deleteNode(wio);
		return;
	}
}

void FLTreeWidget::contextMenuEvent(QContextMenuEvent *event)
{
	QTreeWidgetItem* item = itemAt(event->pos());
	//if(item == nullptr)
	//	return;
	QPoint pt = mapToGlobal(event->pos());
	emit itemMenuEvent(item,pt);
	return;
	
	
		
}

QTreeWidgetItem* FLTreeWidget::parentItem(FLFloor* node)
{
	int num_topitem = topLevelItemCount();
	for(int n=0; n< num_topitem; n++)
	{
		auto item = topLevelItem(n);
		if( (QObject*)item->data(0,ItemPointerData).toULongLong() == node->parent())
		{		
			return item;
		}
	}
	return nullptr;
}

QTreeWidgetItem* FLTreeWidget::parentItem(FLZoneControllor* node)
{
	auto floor = (FLFloor*) node->parent();
	auto parent_item = parentItem(floor);
	int num_children = parent_item->childCount();
	for (int n=0; n< num_children; n++)
	{
		if( (QObject*)parent_item->child(n)->data(0,ItemPointerData).toULongLong() == floor)
		{
			return parent_item->child(n);
		}
	}
	return nullptr;

}

QTreeWidgetItem* FLTreeWidget::parentItem(FLWio* node)
{
	auto zc = (FLZoneControllor*) node->parent();
	auto parent_item = parentItem(zc);
	int num_children = parent_item->childCount();
	for (int n=0; n< num_children; n++)
	{
		if( (QObject*)parent_item->child(n)->data(0,ItemPointerData).toULongLong() == zc)
		{
			return parent_item->child(n);
		}
	}
	return nullptr;
}

void FLTreeWidget::setFocusItem(FLObject* obj)
{
	int num_top_item = topLevelItemCount();
	FLJob* job = nullptr;
	FLFloor* floor = nullptr;
	FLZoneControllor* zc = nullptr;
	FLWio* wio = nullptr;

	auto type = obj->getProperty("Type").value.toString();
	if(type == "WIO")
	{
		wio = (FLWio*)obj;
		zc = (FLZoneControllor*) wio->parent();
		floor = (FLFloor*) zc->parent();
		job = (FLJob*) floor->parent();
	}
	else if(type == "ZC")
	{
		zc = (FLZoneControllor*) obj;
		floor = (FLFloor*) zc->parent();
		job = (FLJob*) floor->parent();
	}
	else if(type == "FLOOR")
	{
		floor = (FLFloor*) obj;
		job = (FLJob*) floor->parent();
	}
	if(floor == nullptr)
		return;
	QTreeWidgetItem* focus_item = nullptr;
	for(int n=0; n< num_top_item; n++)
	{
		auto item = topLevelItem(n);
		if( (FLObject*)item->data(0,ItemPointerData).toULongLong() == job)
		{
			focus_item = setFocusItem(item,floor);
			expandItem(focus_item);
			focus_item->setSelected(true);
// 			if( floor && focus_item)
// 			{
// 				focus_item = setFocusItem(focus_item,floor);
// 			}
			if( zc && focus_item)
			{
				focus_item = setFocusItem(focus_item,zc);
				expandItem(focus_item);
				focus_item->setSelected(true);
			}
			if(wio && focus_item)
			{
				focus_item = focus_item->child(5);
				expandItem(focus_item);
				focus_item->setSelected(true);
				focus_item = setFocusItem(focus_item,wio);
			}
		}
	}
	if(focus_item == nullptr)
		return;
	expandItem(focus_item);
	focus_item->setSelected(true);
}

QTreeWidgetItem* FLTreeWidget::setFocusItem(QTreeWidgetItem *parent_item, FLObject* pObj)
{
	int num_child_items = parent_item->childCount();
	for(int n = 0; n<num_child_items; n++)
	{
		auto item = parent_item->child(n);
		if( (FLObject*)item->data(0,ItemPointerData).toULongLong() == pObj)
		{
			return item;
		}
	}
	return nullptr;
}

QTreeWidgetItem* FLTreeWidget::item(FLFloor* node)
{
	int num_topitem = topLevelItemCount();
	for (int n = 0; n< num_topitem; n++)
	{
		auto item = topLevelItem(n);
		for (size_t i = 0; i < item->childCount(); i++)
		{
			if ((QObject*)item->child(i)->data(0, ItemPointerData).toULongLong() == node)
			{
				return item->child(i);
			}
		}
		
	}
	return nullptr;
}

QTreeWidgetItem* FLTreeWidget::item(FLZoneControllor* node)
{
	auto floor = (FLFloor*)node->parent();
	auto parent_item = item(floor);
	int num_children = parent_item->childCount();
	for (int n = 0; n< num_children; n++)
	{
		if ((QObject*)parent_item->child(n)->data(0, ItemPointerData).toULongLong() == node)
		{
			return parent_item->child(n);
		}
	}
	return nullptr;
}

QTreeWidgetItem* FLTreeWidget::item(FLWio* node)
{
	auto zc = (FLZoneControllor*)node->parent();
	auto parent_item = item(zc);
	int num_children = parent_item->childCount();
	parent_item = parent_item->child(5);
	for (int n = 0; n< num_children; n++)
	{
		if ((QObject*)parent_item->child(n)->data(0, ItemPointerData).toULongLong() == node)
		{
			return parent_item->child(n);
		}
	}
	return nullptr;
}

QTreeWidgetItem* FLTreeWidget::item(FLObject* node)
{
	auto zc = (FLZoneControllor*)node->parent();
	auto parent_item = item(zc);
	int num_children = parent_item->childCount();
	parent_item = parent_item->child(0);
	for (int n = 0; n< num_children; n++)
	{
		if ((QObject*)parent_item->child(n)->data(0, ItemPointerData).toULongLong() == node)
		{
			return parent_item->child(n);
		}
	}
	return nullptr;
}
