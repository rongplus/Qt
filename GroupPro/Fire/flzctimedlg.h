#ifndef FLZCTIMEDLG_H
#define FLZCTIMEDLG_H

#include <QDialog>
#include "ui_flzctimedlg.h"

#include <QSerialPort>

class FLZCTimeDlg : public QDialog
{
	Q_OBJECT

public:
	FLZCTimeDlg(QWidget *parent = 0);
	~FLZCTimeDlg();

	protected slots:
	void on_cancel();
	void on_update_to_current();
	void on_update_to_new();
	void on_connected();
private:
	Ui::FLZCTimeDlg ui;

	QSerialPort* _port;
};

#endif // FLZCTIMEDLG_H
