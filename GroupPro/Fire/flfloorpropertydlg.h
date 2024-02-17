#ifndef FLFLOORPROPERTYDLG_H
#define FLFLOORPROPERTYDLG_H

#include <QDialog>
#include "ui_flfloorpropertydlg.h"
#include "flfloor.h"

class FLFloorPropertyDLG : public QDialog
{
	Q_OBJECT

public:
	FLFloorPropertyDLG(FLFloor* pFloor, QWidget *parent = 0);
	~FLFloorPropertyDLG();

protected slots:
	void on_ok();
	void on_cancel();
private:
	Ui::FLFloorPropertyDLG ui;
	FLFloor* _floor;
};

#endif // FLFLOORPROPERTYDLG_H
