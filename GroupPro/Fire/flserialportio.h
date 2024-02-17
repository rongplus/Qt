#ifndef FLSERIALPORTIO_H
#define FLSERIALPORTIO_H

#include <QThread>
#include <QSerialPort>
#include <QMutex>
#include "flzcserialport.h"

class FLSerialPortIO : public QThread
{
	Q_OBJECT

public:
	FLSerialPortIO(QObject *parent);
	~FLSerialPortIO();

	int send_data(QString cmd, QStringList res, QByteArray & data, QSerialPort * port);

	enum Serial_Command_State
	{
		Login_Success = 100,		
		Read_Time_Success,
		Write_Time_Success,
		DST_Success,
		Failed = -1,
		Write_OutTime = -2
	};

	void run();
	void set_stop() {		b_running = false;	}
	bool is_working() { return b_working; }
	void set_filename(QString filename) {		_filename = filename;	}
	QString command_name() { return _command; }
protected slots:
	void on_port_data();
	void show_modem_message(QString);
signals:
	void state_change(int);
private:
	void run_login();
	void run_read_data();
	void run_send_data();

	
	int send_binary_1K(QString cmd, QByteArray & data);
	int send_command(QString cmd, QSerialPort * port, bool bWait_c);
	bool send_data_1K(QSerialPort * port, QByteArray & ba);

	QSerialPort* _port;
	QStringList _response;
	int _times;
	QByteArray  _data;
	QString _command;
	
	QMutex mutex;
	FLZCSerialPort* _zcport;
	bool b_running;
	bool b_working;
	QString _filename;

};

#endif // FLSERIALPORTIO_H
