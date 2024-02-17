#ifndef FLSERIALWRITE_H
#define FLSERIALWRITE_H

#include <QDialog>
#include "ui_fldlgserialwrite.h"
#include "flzcserialport.h"

#include <QSerialPort>
#include <QTimer>
#include "flzonecontrollor.h"

class FLDlgSerialWrite : public QDialog
{
	Q_OBJECT

public:
	FLDlgSerialWrite(QWidget *parent = 0);
	~FLDlgSerialWrite();

	void set_datatype(int type);
	void set_data(QByteArray &data);

	void set_zc(FLZoneControllor*);

protected:	
	void send_zc_data();
	void send_wio_data();

protected slots:
	void On_OK();
	void On_Cancel();
	void on_login();
	void on_sendconfig();

	void on_getconfig();
	void on_filename();


	void onReadyRead_zc();
	void on_login_data();
	void on_getconfig_data();
	void on_sendconfig_data();
	void on_zigbee_data();

	void on_showmessage(QByteArray);

	void update_state();
	void onReadyRead_OTA();

	void on_state_changed(int state);
signals:
	void updata_zc(FLZoneControllor*);

private:
	bool save_data(QDataStream* datasteam, QByteArray& buff1029);
	bool save_data(QByteArray& buff1029);
	int receive(unsigned char *dest);
	bool _outbyte(int c, int times);
	int parse_rec_data(QByteArray arr);
	QString _exp_data;
	QString _password;
	QByteArray *_data;
	QByteArray _zc_data;
	int remain_data_size;

	QByteArray _out_string;
	QByteArray _in_string;
	QList<QStringList> _procedure;
	int _try_times;
	int _type;
	int retry_number;
	unsigned char pakager_serials;
	QSerialPort* serial;
	FLZoneControllor* _zc;
	QTimer _timer;
	FLZCSerialPort * _zcport;

	Ui::FLDlgSerialWrite ui;
};

#endif // FLSERIALWRITE_H
