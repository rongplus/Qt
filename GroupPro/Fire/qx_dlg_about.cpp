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
#include <QDialog>
#include <QTextStream>
#include "qx_dlg_about.h"

#include <QGraphicsDropShadowEffect>
#include "mgc_checkupdate.h"


AboutDlg::AboutDlg(QWidget * parent , Qt::WindowFlags flags )
		: QDialog(parent)
{
	ui.setupUi(this);

	//setWindowFlags(Qt::WindowStaysOnTopHint);
	setWindowFlags(0);

	int w = 588;
	int h = 301;
	//setGeometry((parent->window()->width()-w)/2,(parent->window()->height()-h)/2,w,h);

	m_lblAppName = ui.lblAppName;
	m_lblCopyRight = ui.lblCopyRight;
	m_lblDate = ui.lblDate;
	m_lblVersionNo = ui.lblVersion;
	m_lblAppName->setText("FireLink Config");
	m_lblVersionNo->setText("Version 1.0.0.0");
	m_lblDate->setText(QString(BUILD_DATE) +" - "+ QString(BUILD_TIME));
	m_lblCopyRight->setText("Copyright (C) 2015\nMircom Group of Companies.");
	connect(ui.pbOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_upddate()));

// 	QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
// 	effect->setBlurRadius(10);
// 	setGraphicsEffect(effect);

	QFile releaseNotes(":/RELEASE_NOTES");
	if (releaseNotes.open(QFile::ReadOnly | QFile::Text))
	{
		QTextStream in(&releaseNotes);
		ui.tbReleaseNotes->setText(in.readAll());
		releaseNotes.close();
	}
}


AboutDlg::~AboutDlg()
{

}


void AboutDlg::on_upddate()
{
	MGC_CheckUpdate::GetInstance()->check_update(true);
}


