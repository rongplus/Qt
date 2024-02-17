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

#ifndef QXLINK_H
#define QXLINK_H


#ifndef STR
#define STR(x) x.toUtf8().data()
#endif

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QByteArray>

#include "qx_xmodem.h"

class QueueItem : public QObject {
	Q_OBJECT
public:
	QueueItem(QObject *parent = 0) : QObject(parent) {}
};

class SendDataItem : public QueueItem {
	Q_OBJECT
public:
	QByteArray ba;
	uchar startChar;
};

struct FirmwareVersion {
	int major;
	int minor;
	int patch;
};

class QxLink : public QThread {
	Q_OBJECT
public:
	enum QueueItemPriority {
		ImmediatePriority,
		NormalPriority,
		IdlePriority
	};
	enum LinkError {
		ERROR_OPEN_SERIAL,
		ERROR_PARAMETERS_SERIAL,
		ERROR_DISCONNECT
	};
	QxLink(QObject *parent = 0);
	void setPort(QString port) {
		m_port = port;
	}
	~QxLink();

	bool Check();
	int chat(QByteArray snd, QStringList rcv, int msec, QString *response);
	void run();
	void send(QByteArray ba,QueueItemPriority priority, uchar startChar);
	bool isConnected();
	bool isBusy();
	FirmwareVersion firmwareVersion();
	QString firmwareVersionString();
	QString JobVersion() { return m_job_version; }
	QString JobID() { return m_job_ID; }
	int compareVersion(int major, int minor, int patch);
	int chat1(QByteArray snd, QStringList rcv, int msec, QString *response);
public slots:
	void cancel();
	void exitLoop();
signals:
	void progress(int);
	void message(QString);
	void error(QString);
	void success(QString);
	void transmissionStarted();
	void transmissionFinished();
	void transmissionError();
	void fetchedAll();
	void connected();
	void disconnected();
	void updatePanel();
private:
	QList<QueueItem*> m_normalQueue, m_idleQueue;
	QueueItem* m_currentItem;
	QSerialPort *m_serialPort;
	XMODEM *m_xModem;
	bool m_exit;	
	bool m_notify;
	QMutex m_mux;
	QString m_port;
	QString m_firmware_version;
	QString m_job_version;
	QString m_job_ID;
};

#endif