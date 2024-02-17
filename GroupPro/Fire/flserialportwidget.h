#ifndef FLSERIALPORTWIDGET_H
#define FLSERIALPORTWIDGET_H

#include <QWidget>
#include "ui_flserialportwidget.h"
#include <QSerialPort>
class FLSerialportWidget : public QWidget
{
	Q_OBJECT

public:
	FLSerialportWidget(QWidget *parent = 0);
	~FLSerialportWidget();

	QSerialPort* serial_port() { return _port; }

private:
	Ui::FLSerialportWidget ui;
	QSerialPort* _port;
};

#endif // FLSERIALPORTWIDGET_H
