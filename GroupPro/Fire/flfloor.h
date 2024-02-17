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
#ifndef FLFLOOR_H
#define FLFLOOR_H

#include <QObject>
#include "flzonecontrollor.h"
#include "flobject.h"

class FLFloor : public FLObject
{
	Q_OBJECT

public:
	FLFloor(QObject *parent);
	~FLFloor();

	void addZC(QPointer<FLZoneControllor> pZC);
	void removeZC(QPointer<FLZoneControllor> pZC);
	QPointer<FLZoneControllor> getZC(int nZC);
	QPointer<FLZoneControllor> addZC(QString str = "Default ZC");
	int countZC();
	void Init();
private:
	
	QList< QPointer<FLZoneControllor> > m_ltZC;
signals:

	void on_zc_changed();
};

#endif // FLFLOOR_H
