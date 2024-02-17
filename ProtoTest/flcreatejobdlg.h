
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
#ifndef FLCREATEJOBDLG_H
#define FLCREATEJOBDLG_H

#include <QDialog>
#include "ui_flcreatejobdlg.h"

#include "fljob.h"
#include "flfloor.h"


class FLCreateJobDlg : public QDialog
{
	Q_OBJECT

public:
	FLCreateJobDlg(FLJob* pJob = 0,QWidget *parent = 0);
	~FLCreateJobDlg();
	FLJob* Job() { return m_pJob;}

private:
	Ui::FLCreateJobDlg ui;
	FLJob* m_pJob;
	bool m_bEditJob;

	void Init(FLJob* pJob);

	QList<FLFloor*> list_floor_added;
protected slots:
	void onSelectPath();
	void onSelectGraph();
	void onAddFloor();
	void onDeleteFloor();
	void OnEditFllor();
	void onOk();
	void onCancle();

	void on_item_clicked(QTreeWidgetItem*,int);
	void on_jobname_changed(QString);
};

#endif // FLCREATEJOBDLG_H
