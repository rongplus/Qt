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
#ifndef FLTREEWIDGET_H
#define FLTREEWIDGET_H

#include <QTreeWidget>

#include "fljob.h"
#include "flfloor.h"
#include "flzonecontrollor.h"
#include "flwio.h"

class FLTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	FLTreeWidget(QWidget *parent);
	~FLTreeWidget();
	
public slots:
	void addNode( FLJob* pJob);
	void addNode( FLFloor* pJob);
	void addNode( FLWio* pJob);
	void addNode( FLObject* pJob);
	

	void deleteNode( FLJob* pJob);
	void deleteNode( FLFloor* pFloor);
	void deleteNode( FLWio* pWio);
	void deleteNode( FLObject* pObj);
	void deleteNode( FLZoneControllor* pObj);

	void on_propertyChanged(QString);

	QTreeWidgetItem* parentItem(FLFloor* node);
	QTreeWidgetItem* parentItem(FLZoneControllor* node);
	QTreeWidgetItem* parentItem(FLWio* node);

public:

	void addNode( QTreeWidgetItem *pParentItem, FLFloor* pFloor);
	void addNode( QTreeWidgetItem *pParentItem, FLWio* pWio);
	void addNode( QTreeWidgetItem *pParentItem, FLObject* pObj);
	void addNode( QTreeWidgetItem *pParentItem, FLZoneControllor* pZC);

	void setFocusItem(FLObject* obj);
	QTreeWidgetItem* setFocusItem(QTreeWidgetItem *pParentItem, FLObject* pObj);
	
protected:	
	virtual void contextMenuEvent(QContextMenuEvent *);

	QTreeWidgetItem* item(FLFloor* node);
	QTreeWidgetItem* item(FLZoneControllor* node);
	QTreeWidgetItem* item(FLWio* node);
	QTreeWidgetItem* item(FLObject* node);
signals:
	void itemDoubleClicked(QTreeWidgetItem *);
	void itemMenuEvent(QTreeWidgetItem *item, QPoint pt);
};

#endif // FLTREEWIDGET_H
