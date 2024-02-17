#ifndef FLDEVICEITEM_H
#define FLDEVICEITEM_H

#include <QGraphicsPixmapItem>
#include <QTimer>
#include "edge.h"

enum class DeviceStatus
{
	//wio
	Device_Inactive = 0,
	Device_MAC,
	Device_NWK,
	Device_BC,
	Device_Rx,
	Device_Tx,
	Device_Leave,
	Device_BroadCast,
	Device_AssociationRequest,
	Device_LinkStatus
	//zc
};

class FLDeviceItem : public QGraphicsPixmapItem//, public QObject
{	
	//Q_OBJECT
public:
	FLDeviceItem(const QPixmap & pixmap, QGraphicsItem * parent = 0);
	~FLDeviceItem();

	virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget);

	void setTimer(int msec, QColor clr, DeviceStatus type = DeviceStatus::Device_Inactive);

	bool isActived() { return _length_time>0; }

	void updateItem(int value = 2);

	void addEdge(Edge *edge);
	QList<Edge *> edges() const;

	//void setiColor(QColor iclr) { _iclr = iclr;  };
private:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

	QTimer *_timer;
	int _length_time;
	int _total_time;
	QColor _clr;
	int off_set;
	QColor _iclr;
	QColor _idleclr;
	QList<Edge *> edgeList;
};

#endif // FLDEVICEITEM_H
