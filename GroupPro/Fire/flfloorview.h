/***************************************************************************************
*                                                                                      *
*	   **************************************************************************      *
*	   **            Company Confidential - For Internal Use Only              **      *
*	   **                   Mircom Group of Companies("MGC")     	           **      *
*	   **                                                                      **      *
*	   **   This information is confidential  and the exclusive property of    **      *
*	   ** Mircom.  It is intended for internal use and only for the purposes   **      *
*	   **   provided,  and may not be disclosed to any third party without     **      *
*	   **                prior written permission from Mircom.                 **      *
*	   **                                                                      **      *
*	   **                          Copyright 2016                              **      *
*	   **                            Rong Huang(ron)                           **      *
*	   **************************************************************************      *
*																					   *
***************************************************************************************/
#ifndef FLFLOORVIEW_H
#define FLFLOORVIEW_H

#include <QGraphicsView>
#include <QMenu>

#include "flfloor.h"
#include "flheatmap.h"
#include "flgraphicssvgitem.h"
#include "flobjectlineitem.h"
#include "fldeviceitem.h"
#include "heatmapper.h"
#include "gradientpalette.h"



class FLFloorView : public QGraphicsView
{
	Q_OBJECT
public:
    enum {
		CANVAS_WIDTH = 1000,
		CANVAS_HEIGHT = 700,
		DEFAULT_RADIUS = 60,
		DEFAULT_OPACITY = 128,
		DEFAULT_WIDTH = 255
	};

private://members
	QPointF mousePressPos;
	int m_nPressedCorner;//0,1,2,3
	QGraphicsItem* itemOnResize;
	QGraphicsItem* item_pressed;
	//QGraphicsPixmapItem* item_heatmap;

	QList<FLObjectLineItem*> list_line_items;

	QList<Edge*> list_edge_items;

	QTimer *timer;
	
	//HeatMapper *mapper_;
	
	//QImage *canvas_;
	
	GradientPalette *palette_;
	int _x;
	int _y;
	bool _moveable;
	int WIO_cnt;

	bool _bInit;
	QStringList _short_mac;//debug
	QStringList _long_mac;//debug
	QPointer<FLGraphicsSvgItem> m_pFloorItem;
	QGraphicsPixmapItem* m_pFloorPixmap;
public:
	FLFloorView(QWidget *parent);
	~FLFloorView();
	void setFloor(QPointer<FLFloor> pFloor);
	FLFloor* Floor();

	void AddZC(FLZoneControllor* pZC,int nIndex);
	QGraphicsItem* AddWIO(FLWio* pWIO,int nRow,int nCol);

	void RemoveZC(FLZoneControllor* pZC);
	void RemoveWIO(FLWio* wio);

	void DeleteFLObject(FLObject *obj);
	void AddFLObject(FLObject *obj);

	void setItemVisible(FLObject* obj,bool bVisible);

	void setMoveable(bool enable) { _moveable = enable; }
	
	void activeDevice(QString mac, int msec, QString ShrtAdd= "0x????", QString info="", QColor clr= Qt::red, QColor idleclr = Qt::gray);

	void activeDevice(FLObject* obj, int msec,QString info, DeviceStatus type, QString mac, QString ShrtAdd);

	
	FLDeviceItem* AddWIOToFloor(FLZoneControllor*zc, FLWio* pWIO, int pos = -1);

	void set_current_object(FLObject* obj);
	void create_new_job();

	void reset_rect(QRect rt);

	void link_wio(QString src, int num1, QString dest, int num2);

private://functuons
	

	
	int PtInCorner(QPointF pt, QRectF rt);
	bool PtInCorner(QPointF pt, QPointF ptCorner);

	FLDeviceItem* find_item(QString mac,QString sMac = "");
	void active_lineitem2WIO(FLWio* wio, int msec);
	QGraphicsItem* find_item(FLObject* obj);

	FLGraphicsSvgItem* CreateSVGFloor(FLFloor* floor, QString file);
	
protected:
	void wheelEvent( QWheelEvent* event );	
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseDoubleClickEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);

	void UpdateItem(QGraphicsItem* item);	

	virtual void showEvent(QShowEvent *event);

	
protected slots:
	void on_PropertyChanged(QString strKey);
	void on_data_Change(int);
	void on_FLObject_Change(FLObject *obj,int mode);
	void refresh_items();
	void on_write_zc();
	void on_select_wios();
	

signals:
	void Item_Position_Changed(QGraphicsItem*);
	void focusItemChanged(FLObject *obj);

public slots:
	void ShowContextMenu(const QPoint &pos);
	void toggleMoveable();
	void changeFloorPlan();
	void fit_window();
	

};

#endif // FLFLOORVIEW_H
