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
*	   **                          Copyright 2015                              **      *
*	   **                            Omid Sakhi                                **      *
*	   **************************************************************************      *
*																					   *
***************************************************************************************/

#ifndef ABOUTDLG_H
#define ABOUTDLG_H

#include <QDialog>
#include <QStatusbar>
#include <QStatusTipEvent>

#include "ui_qx_about.h"

#define  BUILD_DATE __DATE__
#define  BUILD_TIME __TIME__

class AboutDlg :public QDialog
{
	Q_OBJECT

public:
	AboutDlg(QWidget * parent = 0, Qt::WindowFlags flags = 0);
	~AboutDlg();
	void initilize();
	
protected:
	public slots :
		void on_upddate();
private:
	QLabel *m_lblAppName;
	QLabel *m_lblVersionNo;
	QLabel *m_lblDate;
	QLabel *m_lblCopyRight;
	Ui::aboutDlg	ui;
};
#endif