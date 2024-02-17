#ifndef FLPROGRESSDIALOG_H
#define FLPROGRESSDIALOG_H

#include <QDialog>
#include "ui_flprogressdialog.h"

#include "flzcserialport.h"

class FLProgressDialog : public QDialog
{
	Q_OBJECT

public:
	FLProgressDialog(QWidget *parent = 0);
	~FLProgressDialog();

	void send_data(QSerialPort* serial,QByteArray& data);
	void showEvent(QShowEvent *event);

protected slots:
	void show_progress(int);
private:
	Ui::FLProgressDialog ui;
	QSerialPort* _serial;
	QByteArray* _data;
};

#endif // FLPROGRESSDIALOG_H
