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
#ifndef FLCREATEZCDLG_H
#define FLCREATEZCDLG_H

#include <QDialog>
#include "ui_flcreatezcdlg.h"
#include "flzonecontrollor.h"
#include "flwio.h"
#include "flfloor.h"

class FLCreateZCDlg : public QDialog
{
	Q_OBJECT

public:
	FLCreateZCDlg(bool show_add_group,FLZoneControllor* pZC, QWidget *parent = 0);
	FLCreateZCDlg(bool show_add_group,FLFloor* floor, QWidget *parent = 0);
	~FLCreateZCDlg();

	FLZoneControllor* ZC() {return m_pZC; }

private:
	Ui::FLCreateZCDlg ui;
	FLZoneControllor* m_pZC;
	bool m_bEdit;
	QTreeWidgetItem *m_pwioItem;
	void Init(FLZoneControllor* pZC);
	QList<FLWio*> list_wio_added;
protected slots:
	void onAdd();
	void onDelete();
	void OnEdit();
	void onOk();
	void onCancle();
};

#endif // FLCREATEZCDLG_H
