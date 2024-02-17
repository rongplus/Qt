#ifndef FLDLGCHOOSESERIALPORT_H
#define FLDLGCHOOSESERIALPORT_H

#include <QDialog>
#include "ui_fldlgchooseserialport.h"

class FLDlgChooseSerialPort : public QDialog
{
	Q_OBJECT

public:
	FLDlgChooseSerialPort(QWidget *parent = 0);
	~FLDlgChooseSerialPort();

	QString get_port_name();
	QString get_port_fullname();
	void set_datatype(int type);
	void set_data(QByteArray &data);
protected:
	void InitPort();
	/**
	* @brief fillPortsParameters()
	*/
	void fillPortsParameters();

	/**
	* @brief fillPortsInfo()
	*/
	void fillPortsInfo();

	/**
	* @brief updateParameters()
	*/
	void updateParameters();

	void send_zc_data();
	void send_wio_data();
protected slots:
	void On_OK();
	void On_Cancel();
	void On_Update();
	void showPortInfo(int idx);

	void onReadyRead_zc();

	
private:
	Ui::FLDlgChooseSerialPort ui;
	QString _exp_data;
	QString _password;
	QByteArray *_data;

	QByteArray _out_string;
	QByteArray _in_string;
	QList<QStringList> _procedure;
	int _try_times;
	int _type;

};

#endif // FLDLGCHOOSESERIALPORT_H
