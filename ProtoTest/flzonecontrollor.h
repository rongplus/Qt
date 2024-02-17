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
#ifndef FLZONECONTROLLOR_H
#define FLZONECONTROLLOR_H

#include <QObject>
#include "flwio.h"

#include "flobject.h"
class ZC;

class FLZoneControllor : public FLObject
{
	Q_OBJECT

public:
	FLZoneControllor(QObject *parent);
	~FLZoneControllor();

	void addWio(QPointer<FLWio> pWio);
	void removeWio(QPointer<FLWio> pWio);
	QPointer<FLWio> getWio(int nWio);
	int countWio();

	void InitDefaultIO();	

	void addIO(FLObject* pIO);
	void deleteIO(FLObject* pIO);
	FLObject* getIO(int nIndex);
	int countIO();
	void Init();
	QList<FLObject*> outputs();
	QList<FLObject*> inputs();

	int output_number(bool bDefault= false);
	int input_number(bool bDefault = false);

	void reset_input();
	void reset_output();

	void writeFirmware(ZC* zc);
	void readFirmware(ZC* zc);
	int get_interval(int type);
	void add_timer(int id, int duration);

	QString MacAddress();
	void setMacAddress(QString strMAc);
private:
	QString get_default_corrs(QString type, QList<QPointer<FLObject>> lt);
	QString get_default_corrs(QString type, QList<FLObject*> lt);

	QString get_wio_corrs(QString type, QList<QPointer<FLObject>> lt);
	QString get_wio_corrs(QString type, QList<FLObject*> lt);

	void format_value(char &out_value, int in_value);
	QString format_value(int value);
	
	QList< QPointer<FLWio> > m_ltWio;
	QList<QPointer<FLObject>> m_ltNode;

	
};

#endif // FLZONECONTROLLOR_H
