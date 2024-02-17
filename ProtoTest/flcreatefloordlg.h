
/***************************************************************************************
*                                                                                      *
*	   **************************************************************************      *
*	   **            Company Confidential - For Internal Use Only              **      *
*	   **                   Mircom Group of Companies("MGC")     	           **      *
*	   **                                                                      **      *
*	   **   This information is confidential  and the exclusive property of    **      *
*	   ** Mircom.  It is intended for internal use and only for the purposes   **      *
*	   **   provided,  and may not be disclosed to any third party without     **      *
*	   **                prior written permission from Mircom.                 **      *
*	   **                                                                      **      *
*	   **                          Copyright 2016                              **      *
*	   **                            Rong Huang(ron)                           **      *
*	   **************************************************************************      *
*																					   *
***************************************************************************************/
#ifndef FLCREATEFLOORDLG_H
#define FLCREATEFLOORDLG_H

#include <QDialog>
#include "ui_flcreatefloordlg.h"
#include "flfloor.h"
#include "flzonecontrollor.h"
#include "fljob.h"
class FLCreateFloorDlg : public QDialog
{
	Q_OBJECT

public:
	FLCreateFloorDlg(FLJob* pJob ,bool show_add_group, QWidget *parent = 0);
	FLCreateFloorDlg(FLFloor* pFloor ,bool show_add_group , QWidget *parent = 0);
	~FLCreateFloorDlg();

	FLFloor* Floor() { return m_pFloor; }
private:
	Ui::FLCreateFloorDlg ui;
	FLFloor* m_pFloor;

	void Init(FLFloor* pFloor);
	void AddZC(FLZoneControllor* pZC);
	bool m_bEdit;

protected slots:

	void onSelectGraph();
	void onAdd();
	void onDelete();
	void OnEdit();
	void onOk();
	void onCancle();
};

#endif // FLCREATEFLOORDLG_H
