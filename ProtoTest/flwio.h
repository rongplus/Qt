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

#ifndef FLWIO_H
#define FLWIO_H

#include <QObject>
#include <QPointer>

#include "flobject.h"



class FLWio : public FLObject
{
	Q_OBJECT

public:
	FLWio(QObject *parent);
	~FLWio();

	void setLocation(QString strLocation);
	void setMacAddress(QString strMAc);
	void setShrtAddress(QString strShrt);
	void setModel(int nModel);
	void setSignal(int nSignal);

	void setModel(QString model);
	void setSignal(QString signal);

	QString Model();
	QString Signal();
	void Init();
	QString MacAddress();
private:
	
	QString Model(int nModel);
	QString Signal(int nSignal);

	QMap<QString, int> m_mapModel;
	QMap<QString, int> m_mapSignal;
};

#endif // FLWIO_H
