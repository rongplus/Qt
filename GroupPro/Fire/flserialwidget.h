#ifndef FLSERIALWIDGET_H
#define FLSERIALWIDGET_H

#include <QWidget>
#include "ui_flserialwidget.h"
#include <QSerialPort>

class FLSerialWidget : public QWidget
{
	Q_OBJECT

public:
	FLSerialWidget(QWidget *parent = 0);
	~FLSerialWidget();

	QSerialPort* get_port() { return serial;  }
	void InitPort();
		
	/**
	* @brief fillPortsInfo()
	*/
	void fillPortsInfo();
	
	QString get_port_name();

	void set_enable_connect(bool enable);

protected slots:	
	void showPortInfo(int idx);
	void on_connect();
	void on_disconnect();
	void on_fresh_port();

signals:
	void state_change(int );//1 = connected , 0 = disconnected;
	

private:
	Ui::FLSerialWidget ui;

	QSerialPort* serial;
};

#endif // FLSERIALWIDGET_H
