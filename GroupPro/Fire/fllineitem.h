#ifndef FLLINEITEM_H
#define FLLINEITEM_H

#include <QGraphicsLineItem>

class FLLineItem : public QGraphicsLineItem
{
public:
	FLLineItem(qreal x1, qreal y1, qreal x2, qreal y2, QGraphicsItem * parent = 0);
	~FLLineItem();

	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void setTimer(int msec, QColor clr);

	bool isActived() { return _length_time>0; }
	QColor getColor() { return _clr_list[ _length_time % 3];}
	QPen getPen();
	
private:
	int _length_time;
	QColor _clr;
	int _highling_item;
	static QList<QColor> _clr_list;
};

#endif // FLLINEITEM_H
