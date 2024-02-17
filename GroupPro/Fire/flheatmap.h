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
*	   **                            Muhammad Mehboob Fareed                   **      *
*	   **************************************************************************      *
*																					   *
***************************************************************************************/
#ifndef FLHM_H
#define FLHM_H

#include <QObject>

#include "flobject.h"

class FLHeatMap : public FLObject
{
	Q_OBJECT

public:
	FLHeatMap(QObject *parent);
	~FLHeatMap();

	void addTx(QPoint pZC);
	void removeTx(QPoint pZC);
	QPoint getTx(int nZC);
	QPoint addTx(QString str = "Default TX");
	int countTx();
	void Init();
private:
	
	QList< QPoint > m_ltTx;
signals:

	void on_Tx_changed();
};

#endif // FLFLOOR_H
