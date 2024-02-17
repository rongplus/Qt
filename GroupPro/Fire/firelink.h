
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
#ifndef FIRELINK_H
#define FIRELINK_H

#include <QtWidgets/QMainWindow>
#include <QSerialPort>

#include "ui_firelink.h"
#include "fltreewidget.h"
#include "flpropertytable.h"
#include "FLData.pb.h"
#include "flzonecontrollor.h"
#include "flwio.h"
#include "qx_link.h"
#include "fljobwindow.h"

class FLFloor;
class FLZoneControllor;

class FireLink : public QMainWindow
{
	Q_OBJECT

public:
	FireLink(QWidget *parent = 0);
	~FireLink();

	void open_zigbee_network_monitor(FLFloor* fl);
	void open_usb_debug_terminal(FLFloor* fl);

	void open_zigbee_network_monitor(FLZoneControllor* fl);
	void open_usb_debug_terminal(FLZoneControllor* fl);
	
protected slots:
	///file menu
	void on_new_job();
	void on_save_job();
	void on_save_job_as();
	void on_delete_job();
	void on_open_job(QString );
	void on_exit();
	void on_ota();
	void on_clean_recently();
	void on_pcap_analysis();
	///view menu

	///tree
	
	///tool bar 
	void on_usb_debug(QString);
	void on_zigbee_debug(QString);
	void on_wireless_monitor(QString);
	void on_zigbee_network(QString, int);
	void on_ss(QString);


	void on_usb_debug1();
	void on_zigbee_debug1();
	void on_wireless_monitor1();
	void on_zigbee_monitor1();
	void on_ss1();

	void on_about();
	void on_open_jobManager();
	
	///tab
	void TabCloseRequested(int index);
	void TabChangeRequested(int index);
	///interaction
private:
	void add_tab(QWidget*, QSerialPort* );

private:
	Ui::FireLinkClass ui;
	FLJobWindow* job_window;

};

#endif // FIRELINK_H
