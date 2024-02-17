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
*	   **                          Copyright 2015                              **      *
*	   **                            Omid Sakhi                                **      *
*	   **************************************************************************      *
*																					   *
***************************************************************************************/

#ifndef PORT_DIALOG_H
#define PORT_DIALOG_H

#include <QDialog>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSettings>
#include <QByteArray>
#include "ui_qx_port.h"

class PortDialog : public QDialog {
	Q_OBJECT
public:

	enum  WorkType
	{
		Send_Zigbee_Config = 0,
		Login_Zigbee = 1
	};
	PortDialog(WorkType type,QWidget *parent = 0) ;
	QString port();
	void set_data(QByteArray& ba);

	void write_zigbee_config(QByteArray* ba);

protected slots:
	void startWork();
	void exitWork();
protected:
	QSettings *m_settings;
	QSerialPort *_port;
	WorkType _type;
	QByteArray* _array;
	Ui::PortDialog ui;
};

#endif