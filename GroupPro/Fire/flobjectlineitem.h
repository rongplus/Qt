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
#ifndef FLOBJECTLINEITEM_H
#define FLOBJECTLINEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
class FLObjectLineItem : public QObject
{
	Q_OBJECT

public:
	FLObjectLineItem(QGraphicsItem* item_s, QGraphicsItem* item_e, QObject *parent);
	~FLObjectLineItem();
	
	QGraphicsLineItem* GetLineItem() { return item_line_; }
	QGraphicsItem* StartItem() {return item_start_; }
	QGraphicsItem* EndItem() { return item_end_; }

	void ResetLine();
private:
	
	QGraphicsItem* item_start_;
	QGraphicsItem* item_end_;
	QGraphicsLineItem* item_line_;

public slots:
	void UpdateItem(QGraphicsItem* item);
	
};

#endif // FLOBJECTLINEITEM_H
