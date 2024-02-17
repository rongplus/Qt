#ifndef FLDAYLIGHTDLG_H
#define FLDAYLIGHTDLG_H

#include <QDialog>
#include "ui_fldaylightdlg.h"
#include "fljob.h"

class FLDayLightDlg : public QDialog
{
	Q_OBJECT

public:
	FLDayLightDlg(FLJob* job,QWidget *parent = 0);
	~FLDayLightDlg();
protected slots:
	void on_ok();
	void on_cancel();
private:
	void init();
	Ui::FLDayLightDlg ui;
	FLJob* _job;
};

#endif // FLDAYLIGHTDLG_H
