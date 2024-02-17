#ifndef FLZCPROPERTYDLG_H
#define FLZCPROPERTYDLG_H

#include <QDialog>
#include "ui_flzcpropertydlg.h"
#include "flzonecontrollor.h"

class FLZCPropertyDlg : public QDialog
{
	Q_OBJECT

public:
	FLZCPropertyDlg(FLZoneControllor* zc,QWidget *parent = 0);
	~FLZCPropertyDlg();
	
	
protected slots:
	void on_ok();
	void on_cancel();
private:
	Ui::FLZCPropertyDlg ui;
	FLZoneControllor* _zc;
};

#endif // FLZCPROPERTYDLG_H
