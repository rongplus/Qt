#ifndef FLZIGBEETHREAD_H
#define FLZIGBEETHREAD_H

#include <QThread>
#include <QSerialPort>
#include <QDomDocument>

#include "flxmlcommandmanager.h"

class FLZigbeeThread : public QThread
{
	Q_OBJECT

public:
	FLZigbeeThread(QObject *parent);
	~FLZigbeeThread();

	void run();
	void exit_thread();

	void reset(QSerialPort* port,FLXMLCommandManager* man);
signals:
	void comm_msg(int,QStringList, QString);
	void data_msg(QStringList, QString);

private:
	void parse_data(QByteArray arr);

	void data2frame(QStringList lt, QString cmd);

	int find_cmd(QStringList lt);
	//bool crc();

	FLXMLCommandManager* ZigBeePro_cmd;
	QSerialPort *m_serial;
	QByteArray data_received;
	int remain_data_size;
	bool _exit;
	
};

#endif // FLZIGBEETHREAD_H
