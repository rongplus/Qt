
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
#ifndef FLCREATEWIODLG_H
#define FLCREATEWIODLG_H

#include <QDialog>
#include "ui_flcreatewiodlg.h"
#include "flwio.h"
#include "flzonecontrollor.h"

class FLCreateWioDlg : public QDialog
{
	Q_OBJECT

public:
	FLCreateWioDlg(FLWio *pWio,QWidget *parent = 0);
	FLCreateWioDlg(FLZoneControllor *zc,QWidget *parent = 0);
	~FLCreateWioDlg();

	FLWio* WIO() { return m_pWio ; }
	QList<FLWio*> WIO_list() { return wio_list; }

private:
	Ui::FLCreateWioDlg ui;
	FLWio *m_pWio;
	bool m_bEdit;
	void Init(FLWio *pWio);

	QList<FLWio*> wio_list;

protected slots:
	void onOK();
	void OnCancel();
	void on_model_changed(int);
};

#endif // FLCREATEWIODLG_H
