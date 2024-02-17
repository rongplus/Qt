#include "flobjectlineitem.h"
#include <QPen>
#include "flobject.h"
#include "fllineitem.h"

FLObjectLineItem::FLObjectLineItem(QGraphicsItem* item_s, QGraphicsItem* item_e, QObject *parent)
	: QObject(parent)
{
	item_end_ = item_e;
	item_start_ = item_s;
	item_line_ = new FLLineItem(item_start_->x(),item_start_->y(),item_end_->x(),item_end_->y());
	item_line_->setParentItem( item_end_->parentItem());
	item_line_->setData(ItemPointerData,quintptr(item_start_));
	item_line_->setData(ItemPointerData+1,quintptr(item_end_));
	QPen pen;
	pen.setColor(Qt::blue);
	pen.setStyle(Qt::DashDotDotLine);
	pen.setWidth(3);
	item_line_->setPen(((FLLineItem*)item_line_)->getPen());
	item_line_->setZValue(item_end_->zValue()-1);
	ResetLine();
	//connect(item_start_, SIGNAL())
}

FLObjectLineItem::~FLObjectLineItem()
{
	//delete item_line_;
}

void FLObjectLineItem::UpdateItem(QGraphicsItem* item)
{
	if(item == item_end_ || item_start_ == item)
		ResetLine();
}

void FLObjectLineItem::ResetLine()
{
	QPointF pos_1(item_start_->boundingRect().width()/2, item_start_->boundingRect().height()/2);
	QPointF pos_2( item_end_->boundingRect().width()/2, item_end_->boundingRect().height()/2);
	// 	QPointF pos_1(item_start_->scenePos().x() + item_start_->boundingRect().width()/2,item_start_->scenePos().y() + item_start_->boundingRect().height()/2);
	// 	QPointF pos_2(item_end_->scenePos().x() + item_end_->boundingRect().width()/2,item_end_->scenePos().y() + item_end_->boundingRect().height()/2);

	QLineF line(item_line_->mapFromItem(item_start_, pos_1), item_line_->mapFromItem(item_end_, pos_2));
	item_line_->setLine( line );
}
