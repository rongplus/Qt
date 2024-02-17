#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <QSettings>
#include <QMessageBox>
#include <QStandardPaths>

#include "firelink.h"
#include <glog/logging.h>
#include "flfloorview.h"
#include "fljob.h"
#include "flfloor.h"
#include "flzonecontrollor.h"
#include "flpropertytable.h"
#include "fljobprotol.h"

#include "flcreatejobdlg.h"
#include "flcreatewiodlg.h"
#include "flcreatezcdlg.h"
#include "flcreatefloordlg.h"
#include "flusbdebug.h"

#include "qx_dlg_about.h"
#include "flzigbeeterminal.h"
#include "wirelessmonitor.h"
#include "flzigbeenetworkmonitor.h"
#include "flstartpage.h"
#include "SiteSurveyWindow.h"
#include "ZBType.h"
#include "mgc_checkupdate.h"
#include "flotawidget.h"
#include "flwidgetzigbeenetworkmonitor.h"
#include "flwidgetpcapview.h"

using namespace std;

FireLink::FireLink(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowState(Qt::WindowMaximized);

	ui.tabWidget->clear();
	//usb_linked = nullptr;
	connect(ui.actionNew_Job, SIGNAL(triggered()), this, SLOT(on_new_job()));
	//connect(ui.actionOpen_Job, SIGNAL(triggered()), this, SLOT(on_open_job()));
	connect(ui.actionSave_Job, SIGNAL(triggered()), this, SLOT(on_save_job()));
	connect(ui.actionDelete_Job, SIGNAL(triggered()), this, SLOT(on_delete_job()));
	connect(ui.actionSave_Job_As, SIGNAL(triggered()), this, SLOT(on_save_job_as()));

	connect(ui.actionAdd, SIGNAL(triggered()), this, SLOT(on_edit_add()));
	connect(ui.actionEdit, SIGNAL(triggered()), this, SLOT(on_edit_edit()));
	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(on_edit_delete()));

	connect(ui.actionSendJob, SIGNAL(triggered()), this, SLOT(on_send_job()));
	connect(ui.actionUpdateFirmware, SIGNAL(triggered()), this, SLOT(on_fimrware_update()));


	connect(ui.actionUsbDebug, SIGNAL(triggered()), this, SLOT(on_usb_debug1()));
	connect(ui.actionZigbee_debug, SIGNAL(triggered()), this, SLOT(on_zigbee_debug1()));

	connect(ui.actionZigbeeMonitor, SIGNAL(triggered()), this, SLOT(on_zigbee_monitor1()));
	connect(ui.actionWirelessMonitor, SIGNAL(triggered()), this, SLOT(on_wireless_monitor1()));
	connect(ui.actionJobManager, SIGNAL(triggered()), this, SLOT(on_open_jobManager()));
	connect(ui.actionSS, SIGNAL(triggered()), this, SLOT(on_ss1()));

	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(on_about()));
	connect(ui.actionFloor, SIGNAL(triggered()), this, SLOT(on_view_floor()));
	
	connect(ui.actionWIO, SIGNAL(triggered()), this, SLOT(on_hardware_wio()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(on_exit()));

	connect(ui.actionOTA, SIGNAL(triggered()), this, SLOT(on_ota()));
	connect(ui.actionPCAP, SIGNAL(triggered()), this, SLOT(on_pcap_analysis()));
	connect(ui.actionClean_recent_job, SIGNAL(triggered()), this, SLOT(on_clean_recently()));
	
	FLStartPage *startPage = new FLStartPage(this);		

	int index = ui.tabWidget->addTab(startPage, "Start Page");

	QTabBar* tabBar = ui.tabWidget->findChild < QTabBar* >(QLatin1String("qt_tabwidget_tabbar"));
	tabBar->tabButton(0, QTabBar::RightSide)->hide();

	ui.tabWidget->setCurrentIndex(index);

	connect(startPage, &FLStartPage::wirelessMonitor, this, &FireLink::on_wireless_monitor);
	connect(startPage, &FLStartPage::networkMonitor, this, &FireLink::on_zigbee_network);
	connect(startPage, &FLStartPage::zigBeeTerminal, this, &FireLink::on_zigbee_debug);
	connect(startPage, &FLStartPage::usbTerminal, this, &FireLink::on_usb_debug);
	connect(startPage, &FLStartPage::openSS, this, &FireLink::on_ss);
	
	connect(startPage, &FLStartPage::createJob, this, &FireLink::on_new_job);
	connect(startPage, SIGNAL(openJob(QString)), this,SLOT(on_open_job(QString)));

	connect(startPage, SIGNAL(ota()), this, SLOT(on_ota()));
	
	
	connect(ui.tabWidget,SIGNAL(tabCloseRequested(int )),this ,SLOT(TabCloseRequested(int )));
	connect(ui.tabWidget, SIGNAL(currentChanged(int)), this, SLOT(TabChangeRequested(int)));
	job_window = nullptr;
	//qRegisterMetaType(&ZBTime);
	qRegisterMetaType<ZBTime>("ZBTime");

	MGC_CheckUpdate::GetInstance()->check_update(true);


	QString logPath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/logs";
	QDir logDir(logPath);
	if (!logDir.exists())
		logDir.mkdir(logPath);
	FLAGS_logtostderr = false;
	google::SetLogDestination(google::GLOG_INFO, (logPath + "/log-").toUtf8().data());
	google::InitGoogleLogging("");

}

FireLink::~FireLink()
{
	MGC_CheckUpdate *p = MGC_CheckUpdate::GetInstance();
	delete p;
}

void FireLink::open_zigbee_network_monitor(FLFloor * fl)
{
	NetworkView *nv = new NetworkView();
	int index = ui.tabWidget->addTab(nv, "ZigBee Network Monitor");
	int n = ui.tabWidget->width();
	ui.tabWidget->setCurrentIndex(index);
	nv->set_floor(fl);
}

void FireLink::open_usb_debug_terminal(FLFloor * fl)
{
	FLZigbeeTest *newTest = new FLZigbeeTest(this);
	newTest->set_floor(fl);
	int index = ui.tabWidget->addTab(newTest, "USB Debug terminal");
	ui.tabWidget->setCurrentIndex(index);
	
}

void FireLink::open_zigbee_network_monitor(FLZoneControllor * zc)
{
	NetworkView *nv = new NetworkView();
	int index = ui.tabWidget->addTab(nv, "ZigBee Network Monitor");
	int n = ui.tabWidget->width();
	ui.tabWidget->setCurrentIndex(index);
	//nv->set_floor((FLFloor*)zc->parent());
	nv->set_zc(zc);
}

void FireLink::open_usb_debug_terminal(FLZoneControllor * zc)
{
	FLZigbeeTest *newTest = new FLZigbeeTest(this);
	//newTest->set_floor((FLFloor*)zc->parent());
	newTest->set_zc(zc);
	int index = ui.tabWidget->addTab(newTest, "USB Debug terminal");
	ui.tabWidget->setCurrentIndex(index);
}

void FireLink::on_new_job()
{
	int num_tab = ui.tabWidget->count();
	for(int n=0; n< num_tab; n++)
	{
		auto f_view = qobject_cast<FLJobWindow*> (ui.tabWidget->widget(n));
		if(f_view==nullptr)
			continue;
		ui.tabWidget->setCurrentIndex(n);
		job_window->on_new_job();
		return;
	}
	if (job_window == nullptr)
		job_window = new FLJobWindow("",this);
	int index = ui.tabWidget->addTab(job_window,"Jobs Manager");
	job_window->on_new_job();
	ui.tabWidget->setCurrentIndex(index);
}

void FireLink::on_save_job()
{	
	job_window->on_save_job();
}

void FireLink::on_save_job_as()
{	
	job_window->on_save_job_as();
}

void FireLink::on_delete_job()
{	
	job_window->on_delete_job();
}

void FireLink::on_open_job(QString filename)
{	
	int num_tab = ui.tabWidget->count();
	for(int n=0; n< num_tab; n++)
	{
		auto f_view = qobject_cast<FLJobWindow*> (ui.tabWidget->widget(n));
		if(f_view==nullptr)
			continue;
		ui.tabWidget->setCurrentIndex(n);
		job_window = f_view;
		//return;
	}
	if (job_window == nullptr)
	{
		job_window = new FLJobWindow("", this);
		int index = ui.tabWidget->addTab(job_window, "Jobs Manager");
		ui.tabWidget->setCurrentIndex(index);
	}
	else if (job_window )
	{
		int index = ui.tabWidget->addTab(job_window, "Jobs Manager");
		ui.tabWidget->setCurrentIndex(index);
	}

	if (filename == "")
		job_window->on_open_job();
	else
		job_window->openjob(filename);
}

void FireLink::TabCloseRequested(int index)
{
	auto widget = ui.tabWidget->widget(index);

	ui.tabWidget->removeTab(index);
	if(widget!= job_window)
		delete widget;
}

void FireLink::TabChangeRequested(int index)
{
	auto widget = ui.tabWidget->widget(index);

	//ui.tabWidget->removeTab(index);
	if (widget == job_window)
		//	delete widget;
	{
		addToolBar(job_window->GetMainToolbar());
		job_window->GetMainToolbar()->setVisible(true);

		//InsertMenu();
	}
	else if(job_window)
	{
		removeToolBar(job_window->GetMainToolbar());
	}
}

void FireLink::on_usb_debug1()
{
	on_usb_debug("");
}

void FireLink::on_zigbee_debug1()
{
	on_zigbee_debug("");
}

void FireLink::on_wireless_monitor1()
{
	on_wireless_monitor("");
}

void FireLink::on_zigbee_monitor1()
{
	FLWidgetZigbeeNetworkMonitor* w = new FLWidgetZigbeeNetworkMonitor(this);
	int index = ui.tabWidget->addTab(w, "Network monitor (Live)");
	ui.tabWidget->setCurrentIndex(index);

	//on_zigbee_network("", 0);
}
void FireLink::on_ss1()
{
	on_ss("");
}


void FireLink::on_about()
{
	AboutDlg dlg;
	dlg.exec();
}

void FireLink::on_exit()
{
	close();
}

void FireLink::on_ota()
{
	FLOTAWidget* ota = new FLOTAWidget(this);
	int index = ui.tabWidget->addTab(ota, "OTA && USB Configuration");
	ui.tabWidget->setCurrentIndex(index);
}

void FireLink::on_clean_recently()
{
	QSettings st("Mircom", "FireLink");
	auto job_list = st.value("Recently_Job_Path").toStringList();
	st.setValue("Recently_Job_Path",QStringList());
	FLStartPage* w = (FLStartPage*)ui.tabWidget->widget(0);
	w->init();
}

void FireLink::on_pcap_analysis()
{
	FLWidgetPCAPView *newTest = new FLWidgetPCAPView(this);
	int index = ui.tabWidget->addTab(newTest, "Network Monitor (Recorded)");
	ui.tabWidget->setCurrentIndex(index);
}

void FireLink::on_usb_debug(QString port)
{	
	FLZigbeeTest *newTest = new FLZigbeeTest(this);	
	int index = ui.tabWidget->addTab(newTest,"USB Debug terminal");
	ui.tabWidget->setCurrentIndex(index);
	if (port.isEmpty())
		return;
	auto serial = new QSerialPort(port, newTest);

	if (!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Error"), serial->errorString());
		delete serial;
		return;
	}
	newTest->set_port(serial);

}

void FireLink::on_zigbee_debug(QString port)
{
	FLZigbeeMonitor *zm = new FLZigbeeMonitor();
	int index = ui.tabWidget->addTab(zm,"ZigBee Debug Terminal");
	ui.tabWidget->setCurrentIndex(index);
	
	if (port.isEmpty())
		return;
	auto serial = new QSerialPort(port, zm);

	if (!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Error"), serial->errorString());
		delete serial;
		return;
	}
	zm->set_port(serial);
}


void FireLink::on_wireless_monitor(QString port)
{
	WirelessMonitor *wm = new WirelessMonitor();
	int index = ui.tabWidget->addTab(wm,"Wireless Spectrum Monitor");
	ui.tabWidget->setCurrentIndex(index);
	if (port.isEmpty())
		return;
	auto serial = new QSerialPort(port, wm);

	if (!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Error"), serial->errorString());
		delete serial;
		return;
	}
	wm->set_port(serial);
}

void FireLink::on_zigbee_network(QString port,int channel)
{
	FLWidgetZigbeeNetworkMonitor* w = new FLWidgetZigbeeNetworkMonitor(this);
	int index = ui.tabWidget->addTab(w, "Network monitor (Live)");
	ui.tabWidget->setCurrentIndex(index);
	return;
	NetworkView *nv = new NetworkView();
	index = ui.tabWidget->addTab(nv,"Network Monitor (Live)");
	int n = ui.tabWidget->width();
	ui.tabWidget->setCurrentIndex(index);
	if (port.isEmpty())
		return;
	
	nv->openSniffer(port, channel);
}


void FireLink::on_ss(QString port)
{
	SiteSurveyWindow *ss = new SiteSurveyWindow();
	int index = ui.tabWidget->addTab(ss, "Site Survey");
	ui.tabWidget->setCurrentIndex(index);

	
}

void FireLink::on_open_jobManager()
{
	int num_tab = ui.tabWidget->count();
	for (int n = 0; n< num_tab; n++)
	{
		auto f_view = qobject_cast<FLJobWindow*> (ui.tabWidget->widget(n));
		if (f_view)
		{
			ui.tabWidget->setCurrentIndex(n);
			return;
		}
	}
	if(job_window == nullptr)
		job_window = new FLJobWindow("",this);
	int index = ui.tabWidget->addTab(job_window, "Jobs Manager");	
	ui.tabWidget->setCurrentIndex(index);
}


void FireLink::add_tab(QWidget *, QSerialPort *)
{
}