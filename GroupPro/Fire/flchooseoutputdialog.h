#ifndef FLCHOOSEOUTPUTDIALOG_H
#define FLCHOOSEOUTPUTDIALOG_H

#include <QDialog>
#include "ui_flchooseoutputdialog.h"

#include "flzonecontrollor.h"


class FLChooseOutputDialog : public QDialog
{
	Q_OBJECT

public:
	FLChooseOutputDialog(FLZoneControllor* zc, FLObject* port,QWidget *parent = 0);
	~FLChooseOutputDialog();

	void init();
protected slots:
	void on_ok();
	void on_cancel();
	void on_stateChanged(int state);
private:
	Ui::FLChooseOutputDialog ui;
	FLZoneControllor*_zc;
	FLObject* _port;
	bool _bOutput;
	bool _bWioOutput;

};

#endif // FLCHOOSEOUTPUTDIALOG_H
