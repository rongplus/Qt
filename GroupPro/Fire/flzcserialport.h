#ifndef FLZCSERIALPORT_H
#define FLZCSERIALPORT_H

#include <QObject>
#include <QSerialPort>
#include <QLabel>

class FLZCSerialPort : public QObject
{
	Q_OBJECT
	
public:
	enum TransitState
	{
		Successed = 0,
		Login_Err = 1,
		Read_OutTime = 2,
		Write_OutTime
	};

	FLZCSerialPort(QObject *parent=0);
	~FLZCSerialPort();

	bool login(QSerialPort* port, QString password);

	TransitState send_config(QSerialPort* port, QByteArray &data, int package_len);

	void  set_messagelable(QLabel* lab)
	{
		_label = lab;
	}	

	int send_binary_1K(QString cmd, QByteArray& data, QSerialPort* port);
	int send_binary_80(QString cmd, QByteArray& data, QSerialPort* port);

	int send_binary_1K(QString cmd, QString file, QSerialPort* port);
	int send_binary_80(QString cmd, QString file, QSerialPort* port);

	int send_data(QString cmd, QByteArray& data, QSerialPort* port);

	int xmodemReceive(QSerialPort* port, QByteArray startChar, int destsz, QString jobfile);
	int receive(unsigned char *dest);
	bool save_data(QDataStream* datasteam, QByteArray& buff1029);
	bool _outbyte(int c, int times);

signals:
	void progress(int);
	void message(QByteArray);
private:
	void show_message(QByteArray);
	bool send_data_1K(QSerialPort* port, QByteArray &data);
	bool send_data_80(QSerialPort* port, QByteArray &data);
	
	void set_port(QSerialPort* port);

	void set_peremeter(QMap<QString, int>);
	/**
	*
	**/
	int send_command(QString cmd, QSerialPort* port,bool bWait_c);

	int packQR(QByteArray *arr);
	int packIN(QByteArray *arr);
	int packUER(QByteArray *arr);
	int packIBR(QByteArray *arr);
	void prepareHeader(QByteArray *arr, bool is_wio,QString WIOAddress="");

	bool is_ota;
	unsigned char packet_number;
	unsigned char data_packet;
	int bytetoRead;
	QLabel* _label;
	QSerialPort * _port;

	QString m_jobfile;
	int retry_number;
};

#endif // FLZCSERIALPORT_H
