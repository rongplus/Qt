#include "fllineitem.h"
#include <QPen>
#include <QPainter>
#include <QtMath>
static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;
QList<QColor> FLLineItem::_clr_list = QList<QColor>();
FLLineItem::FLLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem * parent )
	: QGraphicsLineItem(x1,y1,x2,y2,parent)
{
	if(_clr_list.count()==0)
	{
		_clr_list.append(Qt::gray);
		_clr_list.append(QColor(218, 247, 166));
		_clr_list.append(QColor(0, 206, 209));
		_clr_list.append(QColor(64, 224, 208));
		_clr_list.append(QColor(127, 255, 212));
		_clr_list.append(QColor(30, 144, 255));
		_clr_list.append(QColor(135, 206, 235));
		_clr_list.append(QColor(175, 238, 238));
	}
	_length_time = 0;
	_highling_item = 0;
	setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

FLLineItem::~FLLineItem()
{

}

void FLLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/)
{
	QGraphicsLineItem::paint(painter,option,widget);
	_highling_item =_length_time % _clr_list.count();
 	if(_length_time >0)
// 	{
// 		QPen pen;
// 		pen.setColor(_clr_list[_highling_item]);
// 		pen.setStyle(Qt::DashDotDotLine);
// 		pen.setWidth(3);
// 		painter->save();
// 		setPen(pen);
// 		painter->restore();
 		_length_time-=2;
// 	}
	else
	{
		//setPen(getPen());
		setPen(QPen(_clr_list[2], 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		_highling_item = 0;
	}
	
	if (0)
	{
		auto sourcePoint = line().p1()-QPointF(10,10);
		auto destPoint = line().p2() - QPointF(10, 10);
		int arrowSize = 10;
		double angle = ::acos(line().x1() / line().length());
		if (line().dy() >= 0)
			angle = TwoPi - angle;

		QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
			cos(angle + Pi / 3) * arrowSize);
		QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
			cos(angle + Pi - Pi / 3) * arrowSize);
		QPointF destArrowP1 = destPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
			cos(angle - Pi / 3) * arrowSize);
		QPointF destArrowP2 = destPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
			cos(angle - Pi + Pi / 3) * arrowSize);

		painter->setBrush(Qt::black);
		painter->drawPolygon(QPolygonF() << sourcePoint << sourceArrowP1 << sourceArrowP2);
		painter->drawPolygon(QPolygonF() << destPoint << destArrowP1 << destArrowP2);
	}
// 	{
// 		QPen pen;
// 		pen.setColor(Qt::blue);
// 		pen.setStyle(Qt::DashDotDotLine);
// 		pen.setWidth(3);
// 		painter->save();
// 		setPen(pen);
// 		painter->restore();
// 		_length_time-=2;
// 	}
}

void FLLineItem::setTimer(int msec, QColor clr)
{
	_length_time = msec;
	_highling_item = 0;
}

QPen FLLineItem::getPen()
{
	QPen pen;
	pen.setColor(_clr_list[_highling_item]);
	//pen.setStyle(Qt::DashDotDotLine);
	pen.setWidth(3);
	return pen;
}
