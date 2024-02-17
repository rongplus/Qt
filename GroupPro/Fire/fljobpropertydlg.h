#ifndef FLJOBPROPERTYDLG_H
#define FLJOBPROPERTYDLG_H

#include <QDialog>
#include "ui_fljobpropertydlg.h"
#include "fljob.h"

class FLJobPropertyDlg : public QDialog
{
	Q_OBJECT

public:
	FLJobPropertyDlg(FLJob* job,QWidget *parent = 0);
	~FLJobPropertyDlg();
protected slots:
	void on_ok();
	void on_cancel();
private:
	Ui::FLJobPropertyDlg ui;
	FLJob* _job;
};

#endif // FLJOBPROPERTYDLG_H
