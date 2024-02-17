#ifndef FLWIOPROPERTYDLG_H
#define FLWIOPROPERTYDLG_H

#include <QDialog>
#include "ui_flwiopropertydlg.h"
#include "flwio.h"

class FLWioPropertyDlg : public QDialog
{
	Q_OBJECT

public:
	FLWioPropertyDlg( FLWio* wio, QWidget *parent = 0);
	~FLWioPropertyDlg();

protected slots:
	void on_ok();
	void on_cancel();
	
	void on_stateChanged(int);
private:
	Ui::FLWioPropertyDlg ui;
	FLWio* _wio;
};

#endif // FLWIOPROPERTYDLG_H
