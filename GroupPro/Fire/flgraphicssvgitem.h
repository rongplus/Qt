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
#ifndef FLGRAPHICSSVGITEM_H
#define FLGRAPHICSSVGITEM_H

#include <QGraphicsSvgItem>
#include "flfloor.h"

class FLGraphicsSvgItem : public QGraphicsSvgItem
{
	Q_OBJECT

public:
	FLGraphicsSvgItem(QGraphicsItem *parent=0);
	FLGraphicsSvgItem(const QString & fileName, QGraphicsItem * parent = 0);
	~FLGraphicsSvgItem();


	void setFloor(QPointer<FLFloor> pFloor);
	QPointer<FLFloor> Floor();
	
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
	QPointer<FLFloor> m_pFloor;
};

#endif // FLGRAPHICSSVGITEM_H
