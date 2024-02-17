#include "fljobwindow.h"

#include <QFileDialog>
#include <QMenu>
#include <iostream>
#include <fstream>
#include <QSettings>
#include <QMessageBox>
#include <QStandardPaths>
#include "FLJobWindow.h"

#include "firelink.h"
#include "fldlgserialwrite.h"
#include "fldaylightdlg.h"
#include "flzctimedlg.h"

//#include <glog/logging.h>
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
#include "flzcserialport.h"

#include "zc.pb.h"
#include "flchooseoutputdialog.h"
#include "fldaylightdlg.h"

#include "fljobpropertydlg.h"
#include "flfloorpropertydlg.h"
#include "flzcpropertydlg.h"
#include "flwiopropertydlg.h"

#define GLOG_NO_ABBREVIATED_SEVERITIES
#include <glog\logging.h>


using namespace std;

FLJobWindow::FLJobWindow(QString filename,QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowState(Qt::WindowMaximized);	
	ui.tabWidget->clear();
	Init();

	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(on_new_job()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(on_open_job()));
	connect(ui.actionSave, SIGNAL(triggered()), this, SLOT(on_save_job()));
	connect(ui.actionDelete, SIGNAL(triggered()), this, SLOT(on_close_job()));
	connect(ui.actionWrite, SIGNAL(triggered()), this, SLOT(on_write_hardware()));
	connect(ui.actionDockProperty, SIGNAL(triggered()), this, SLOT(on_res_DockProperty()));
	if (filename != "")
	{
		openjob(filename);
	}
	_firelink = (FireLink*)parent;
	
	LOG(INFO) << "Hello From Main Window";
	//restoreDockWidget(ui.dockWidget_3);
	addDockWidget(Qt::RightDockWidgetArea, ui.dockWidget_3);
	addDockWidget(Qt::LeftDockWidgetArea, ui.dockWidget);
}

FLJobWindow::~FLJobWindow()
{

}

void FLJobWindow::on_write_hardware()
{
	QTreeWidgetItem* item = m_pJobTree->currentItem();
	if (item == nullptr)
		return;

	FLObject *node = qobject_cast<FLObject*>((QObject*)item->data(0, ItemPointerData).toULongLong());
	if (node == nullptr)
		return;
	int num_tab = ui.tabWidget->count();
	QString type = node->getProperty("Type").value.toString();
	if (type == "ZC")
	{
		
		write_zc((FLZoneControllor*)node);
	}
	else if (type == "ZC_RELAY")
	{
	}
	else if (type == "WIO")
	{		
	}
}

void FLJobWindow::on_res_DockProperty()
{
	restoreDockWidget(ui.dockWidget_3);
	//addDockWidget(Qt::RightDockWidgetArea,ui.dockWidget_3);
}

void FLJobWindow::on_new_job()
{
	FLCreateJobDlg dlg(0,this);
	if(dlg.exec() == QDialog::Accepted)
	{
		m_pJobTree->addNode(dlg.Job());
		m_pProTable->setNode(dlg.Job());
	
		return;
	}
}

void FLJobWindow::on_save_job()
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	auto item = m_pJobTree->currentItem();
	if(item==nullptr)
	{
		item = m_pJobTree->topLevelItem(0);
		if(item==nullptr)
			return;
	}
	while ( item->parent() != nullptr)
	{
		item = item->parent();
	}
	auto job_obj =(QObject*)item->data(0,ItemPointerData).toULongLong();
	if(job_obj == nullptr)
		return;
	auto job = qobject_cast<FLJob*>(job_obj);

	if(job == nullptr)
		return;

	//Pro_Job *pro_job = new Pro_Job();

	FLJobProtol::SaveJob(job);

}

void FLJobWindow::on_save_job_as()
{

}

void FLJobWindow::on_delete_job()
{
	
}

void FLJobWindow::on_close_job()
{
	auto item = m_pJobTree->currentItem();
	if (item == nullptr)
		return;
	FLObject *node = qobject_cast<FLObject*>((QObject*)item->data(0, ItemPointerData).toULongLong());
	if (node == nullptr)
		return;

	auto pJob = (FLJob*)node;
	auto num_floor = pJob->countFloor();
	int num_tab = ui.tabWidget->count();
	for (int m = 0; m<num_floor; m++)
	{
		auto floor = pJob->getFloor(m);
		for (int n = num_tab - 1; n >= 0; n--)
		{
			auto tab = qobject_cast<FLFloorView*>(ui.tabWidget->widget(n));
			if (tab && tab->Floor() == floor)
			{
				ui.tabWidget->removeTab(n);
			}
		}
	}
	m_pJobTree->takeTopLevelItem(m_pJobTree->indexOfTopLevelItem(item));
}

void FLJobWindow::on_job_setting()
{
	auto item = m_pJobTree->currentItem();
	if (item == nullptr)
	{
		item = m_pJobTree->topLevelItem(0);
		if (item == nullptr)
			return;
	}
	while (item->parent() != nullptr)
	{
		item = item->parent();
	}
	auto job_obj = (QObject*)item->data(0, ItemPointerData).toULongLong();
	if (job_obj == nullptr)
		return;
	auto job = qobject_cast<FLJob*>(job_obj);

	//FLDayLightDlg dlg(job);
	FLJobPropertyDlg dlg(job);
	dlg.exec();
	
}

void FLJobWindow::openjob(QString filename)
{
	if (filename == "")
		return;
	QSettings st("Mircom", "FireLink");
	auto job_list = st.value("Recently_Job_Path").toStringList();
	QFile fi(filename);
	if (fi.exists() == false)
	{
		QMessageBox::warning(this, "Error Job file", "Please choose right job file.");
		job_list.removeOne(filename);
		st.setValue("Recently_Job_Path", job_list);
		return;
	}
	int num_topitem = m_pJobTree->topLevelItemCount();
	for (int n = 0; n< num_topitem; n++)
	{
		auto item = m_pJobTree->topLevelItem(n);
		auto node = qobject_cast<FLObject*> ((QObject*)item->data(0, ItemPointerData).toULongLong());
		if (node->getProperty("JobPath").value.toString() == filename)
		{
			QMessageBox::warning(this, "Job existed!", "Job is opened");
			return;
		}
	}

	Pro_Job *pro_job = new Pro_Job();
	fstream input(filename.toStdString(), ios::in | ios::binary);
	bool bOk = pro_job->ParseFromIstream(&input);
	if (bOk == false)
	{
		QMessageBox::warning(this, "Error Job file", "Please choose right job file.");
		job_list.removeOne(filename);	
		st.setValue("Recently_Job_Path", job_list);
		return;
	}
	FLJob* job = new FLJob(this);
	FLJobProtol::Proto2Object(pro_job, job);
	job->addProperty("JobPath", filename,false);

	m_pJobTree->addNode(job);

	input.close();
	
	job_list.removeOne(filename);
	job_list.insert(0, filename);
	st.setValue("Recently_Job_Path", job_list);
}

void FLJobWindow::openjob(FLJob * job)
{
	Q_ASSERT(job);
	job->setParent(this);
	m_pJobTree->addNode(job);
}

void FLJobWindow::on_open_job()
{
	LOG(INFO) << "Open dialog";
	LOG(INFO) << "Hello From Main on_open_job";
	QString str_file_name = QFileDialog::getOpenFileName(this,"Choose a Job ");
	LOG(INFO) << "Hello From  on_open_job 1 ";
	if (str_file_name.isNull())
		return;	
	LOG(INFO) << "Hello From on_open_job 2";
	openjob(str_file_name);		
}

void FLJobWindow::on_jobtree_clicked(QTreeWidgetItem * pItem, int nCol)
{
	QTreeWidgetItem* item = pItem;
	if(item == nullptr)
		return;

	FLObject *node = qobject_cast<FLObject*>((QObject*)item->data(0,ItemPointerData).toULongLong());
	if(node==nullptr)
		return;
	m_pProTable->setNode(node);	
	auto zc = qobject_cast<FLZoneControllor*>((QObject*)item->data(0, ItemPointerData).toULongLong());
	if (zc)
	{
		auto fl = visual_floor((FLFloor*)zc->parent());
		if(fl)fl->set_current_object(zc);
		return;
	}
	auto wio = qobject_cast<FLWio*>((QObject*)item->data(0, ItemPointerData).toULongLong());
	if (wio)
	{
		auto fl = visual_floor((FLFloor*)wio->parent()->parent());
		if (fl)fl->set_current_object(wio);
		return;
	}
}

void FLJobWindow::on_jobtree_doubleClicked(QTreeWidgetItem * pItem, int nCol)
{
	QTreeWidgetItem* item = pItem;
	if(item == nullptr)
		return;

	FLObject *node = qobject_cast<FLObject*>((QObject*)item->data(0,ItemPointerData).toULongLong());
	if(node == nullptr)
		return;
	FLObjectPropertys p = node->getProperty("Type");
	QString type = p.value.toString();
	if ( type == "FLOOR") 
	{
		open_floor((FLFloor*)node);
	}	
}

void FLJobWindow::TabCloseRequested(int index)
{
	auto widget = ui.tabWidget->widget(index);
	ui.tabWidget->removeTab(index);
	delete widget;
}

void FLJobWindow::UpdateItem(QGraphicsItem* item)
{
	FLObject* node = qobject_cast<FLObject*> ((QObject*)item->data(ItemPointerData).toULongLong());
	QPointF pos = item->scenePos();
	node->addProperty("Position",pos,false);
}

void FLJobWindow::Init()
{
	m_pJobTree = new FLTreeWidget(this);
	ui.dockWidget->setWidget(m_pJobTree);

	m_pProTable = new FLPropertyTable(this);
	ui.dockWidget_3->setWidget(m_pProTable);
	m_pProTable->setParent(ui.dockWidget_3);

	connect(m_pJobTree,SIGNAL(itemDoubleClicked(QTreeWidgetItem * , int )), this, SLOT(on_jobtree_doubleClicked(QTreeWidgetItem * , int )));
	connect(m_pJobTree,SIGNAL(itemClicked(QTreeWidgetItem * , int )), this, SLOT(on_jobtree_clicked(QTreeWidgetItem * , int )));
	connect(ui.tabWidget,SIGNAL(tabCloseRequested(int )),this ,SLOT(TabCloseRequested(int )));
	connect(m_pJobTree, SIGNAL(itemMenuEvent(QTreeWidgetItem*,QPoint)),this, SLOT(on_jobtree_menu(QTreeWidgetItem * , QPoint )));

	//init setting
	QSettings setting("Mircom","FLJobWindow");
	if(setting.value("Default_Job_Path").toString() == "")
	{		
		QString str_path = QApplication::applicationDirPath();
		setting.setValue("Default_Job_Path",str_path);
	}

}

void FLJobWindow::excute_job_menu(QTreeWidgetItem *item, FLObject* node, QPoint pt)
{
	int num_tab = ui.tabWidget->count();
	QMenu menu;
	QAction *a = menu.addAction(QIcon(":/firelink/Resources/icons/add-floor-icon.png"), tr("Add Floor"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/close.png"), tr("Close Job"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/save-icon.png"), tr("Save Job"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/job-icon.png"), tr("Setting"));
	QAction *res = 0;
	res = menu.exec(pt);
	if (res == 0) return;
	if (res->text() == tr("Add Floor"))
	{
		FLJob* pJob = (FLJob*)node;
		FLCreateFloorDlg dlg(pJob, false, this);
		if (dlg.exec() == QDialog::Accepted)
		{
			pJob->addFloor(dlg.Floor());
			m_pJobTree->addNode(item, dlg.Floor());
		}
	}
	else if (res->text() == tr("Close Job"))
		on_close_job();	
	else if (res->text() == tr("Save Job"))
		on_save_job();
	else if (res->text() == tr("Setting"))
		on_job_setting();
}

void FLJobWindow::excute_floor_menu(QTreeWidgetItem *item, FLObject* node, QPoint pt)
{
	QString type = node->getProperty("Type").value.toString();
	int num_tab = ui.tabWidget->count();
	QMenu menu;
	QAction *a = menu.addAction(QIcon(":/firelink/Resources/icons/zc-icon-48.png"), tr("Add zone controller"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/add-floor-icon.png"), tr("Change floor plan"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/remove-icon-48.png"), tr("Delete Floor"));
	menu.addSeparator();
	a = menu.addAction(QIcon(":/firelink/Resources/icons/selection.png"), tr("Open Floor"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/network.png"), tr("Open in Zigbee network monitor window"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/terminal.png"), tr("Open in usb debug terminal window"));
	menu.addSeparator();
	a = menu.addAction(QIcon(":/firelink/Resources/icons/setting.png"), tr("Floor Setting"));
	QAction *res = 0;
	res = menu.exec(pt);
	if (res == 0) return;
	if (res->text() == tr("Add zone controller"))
	{		
		FLFloor* pFloor = (FLFloor*)node;
		FLCreateZCDlg dlg(false, pFloor, this);
		if (dlg.exec() == QDialog::Accepted)
		{
			pFloor->addZC(dlg.ZC());
			m_pJobTree->addNode(item, dlg.ZC());

			auto view = visual_floor(pFloor);			
			if (view )			
				view->AddFLObject(dlg.ZC());
		}

	}
	else if (res->text() == tr("Change floor plan"))
	{
		FLFloor* pFloor = (FLFloor*)node;
		QString file_svg = QFileDialog::getOpenFileName(this, "Choose a SVG file");
		if (file_svg != "")
			pFloor->addProperty("FloorGraph", file_svg);
	}
	else if (res->text() == "Delete Floor")
	{
		FLFloor* pFloor = (FLFloor*)node;
		int n = visual_floor_index(pFloor);
		if(n != -1)
			ui.tabWidget->removeTab(n);
		
		//remove object
		auto job = (FLJob*)pFloor->parent();
		job->removeFloor(pFloor);
		//remove item
		m_pJobTree->deleteNode(pFloor);

	}
	else if (res->text() == "Open Floor")
	{
		FLFloor* pFloor = (FLFloor*)node;
		open_floor(pFloor);
	}
	else if (res->text() == "Open in Zigbee network monitor window")
	{
		//FireLink* fl = qobject_cast<FireLink*>(parent());
		//if(fl)
		FLFloor* pFloor = (FLFloor*)node;
		_firelink->open_zigbee_network_monitor(pFloor);
	}
	else if (res->text() == "Open in usb debug terminal window")
	{
		//FireLink* fl = qobject_cast<FireLink*>(parent());
		//if(fl)
		FLFloor* pFloor = (FLFloor*)node;

		_firelink->open_usb_debug_terminal(pFloor);
	}
	else if (res->text() == "Floor Setting")
	{
		FLFloor* pFloor = (FLFloor*)node;
		FLFloorPropertyDLG dlg(pFloor);
		dlg.exec();
		m_pProTable->setNode(node);
	}
}

void FLJobWindow::excute_zc_menu(QTreeWidgetItem *item, FLObject* node, QPoint pt)
{
	int num_tab = ui.tabWidget->count();
	QString type = node->getProperty("Type").value.toString();
	QMenu menu;
	QAction *a = menu.addAction(QIcon(":/firelink/Resources/icons/add-icon.png"), tr("Add Wio"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/remove-icon-48.png"), tr("Delete ZC"));
	menu.addSeparator();
	//a = menu.addAction(QIcon(":/firelink/Resources/icons/add-floor-icon.png"), tr("Active"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/terminal.png"), tr("Open in usb debug terminal window"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/network.png"), tr("Open in Zigbee network monitor window"));
	menu.addSeparator();
	a = menu.addAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), tr("send/get configuration"));
#if 0
	//a = menu.addAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), tr("Write firmware"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), tr("Write passcode"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), tr("Write Day light save"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/firmware_update.png"), tr("zc time"));
#endif
	a = menu.addAction(QIcon(":/firelink/Resources/icons/Downloading.png"), tr("Initial from file"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/setting.png"), tr("ZC Setting"));
	QAction *res = 0;
	res = menu.exec(pt);
	if (res == 0) return;
	if (res->text() == tr("Add Wio"))
	{
		auto zc = (FLZoneControllor*)node;
		FLCreateWioDlg dlg(zc, this);
		if (dlg.exec() == QDialog::Accepted)
		{
			//zc->addWio(dlg.WIO());
			//m_pJobTree->addNode(item->child(5), dlg.WIO());
			auto view = visual_floor((FLFloor*)zc->parent());
			
			auto lt = dlg.WIO_list();
			for each (auto wio in lt)
			{
				zc->addWio(wio);
				m_pJobTree->addNode(item->child(5), wio);
				m_pJobTree->expandItem(item);
				if (view)
					view->AddFLObject(wio);
			}

		}
	}
	else if (res->text() == tr("Delete ZC"))
	{
		auto floor = (FLFloor*)node->parent();
		floor->removeZC((FLZoneControllor*)node);
		m_pJobTree->deleteNode((FLZoneControllor*)node);
		auto view = visual_floor(floor);
		if(view)
			view->RemoveZC((FLZoneControllor*)node);
	}
	else if (res->text() == "Open in usb debug terminal window")
	{
		//FireLink* fl = qobject_cast<FireLink*>(parent());
		//if (fl)
		{
		//	emit zigBeeTerminal("");
		}
		_firelink->open_usb_debug_terminal( (FLZoneControllor*)node);
	}
	else if (res->text() == "Open in Zigbee network monitor window")
	{
		FireLink* fl = qobject_cast<FireLink*>(parent());
		if (fl)
		{
			//emit networkMonitor("");
		}
		auto floor = (FLFloor*)node->parent();
		_firelink->open_zigbee_network_monitor((FLZoneControllor*)node);
	}
	else if (res->text() == "send/get configuration")
	{
		write_zc((FLZoneControllor*)node);		
	}
	else if (res->text() == "Write firmware")
	{

	}
	else if( res->text() == "zc time")
	{ 
		//get time
		//query to set time.
		FLZCTimeDlg dlg;
		dlg.exec();

		CFG_RTC rtc;
		rtc.set_seconds(10);
		rtc.set_minutes(15);
		rtc.set_hours(5);
		rtc.set_date(1);
		rtc.set_month(2);
		rtc.set_year(2017);

		//save to file;
		QString fileName = "rtc.rtc";
		std::filebuf fb;
		fb.open(fileName.toStdString(), std::ios::out | std::ios::binary);
		std::ostream os(&fb);
		bool bSerialed = rtc.SerializeToOstream(&os);
		fb.close();
	}
	else if( res->text() == "Write passcode")
	{
		auto zc = (FLZoneControllor*)node;
		PASSWORDS pcs;
		PASSWORD *pass = pcs.add_password();
		pass->set_len(4);
		pass->set_data(zc->getProperty("PassCode_Low").value.toString().toStdString());

		pass = pcs.add_password();
		pass->set_len(4);
		pass->set_data(zc->getProperty("PassCode_Mid").value.toString().toStdString());

		pass = pcs.add_password();
		pass->set_len(4);
		pass->set_data(zc->getProperty("PassCode_Hei").value.toString().toStdString());

		//pcs.SerializePartialToCodedStream();

		//save to file;
		QString fileName = "passcode.pwd";
		std::filebuf fb;
		fb.open(fileName.toStdString(), std::ios::out | std::ios::binary);
		std::ostream os(&fb);
		bool bSerialed = pcs.SerializeToOstream(&os);
		fb.close();

	}
	else if (res->text() == "Initial from file")
	{
		auto filename = QFileDialog::getOpenFileName(this,"select a cfg file","","cfg file(*.cfg)");
		if (filename == "")
			return;
		auto pro_zc = (FLZoneControllor*)node;
		FLJobProtol::ZCFromFile(pro_zc, filename);
		
		/*
		QFile fi(filename);
		if (fi.exists() == false)
		{
			QMessageBox::warning(this, "Error Job file", "Please choose right  file.");			
			return;
		}
		
		ZC *zc = new ZC();
		

		if (fi.open(QIODevice::ReadWrite) == false)
			return;
		QDataStream in_s(&fi);

		char *buff = new char[fi.size()];
		int rtc_len = fi.size();
		in_s.readRawData(buff, rtc_len);
		QByteArray arr;
		rtc_len--;
		while (buff[rtc_len] == 0x1a)
			rtc_len--;
		for (size_t i = 0; i < rtc_len; i++)
		{
			arr.append(buff[i]);
		}

		bool bOK = zc->ParseFromArray(buff, rtc_len + 1);

		auto pro_zc = (FLZoneControllor*)node;
		pro_zc->readFirmware(zc);
		*/

		//to do , update tree;
		auto p = item->parent();
		m_pJobTree->deleteNode(pro_zc);
		
		m_pJobTree->addNode(p, pro_zc);
		//delete zc;
	}
	else if (res->text() == "Write Day light save")
	{
		auto zc = (FLZoneControllor*)node;
		auto fl = zc->parent();
		FLJob* job = (FLJob*)fl->parent();
		FLDayLightDlg dlg((FLJob*)job,this);
		dlg.exec();

		CFG_DST dst;
		auto s = job->start_daylight();
		
		dst.set_startdayofweek(s.week);
		dst.set_starthour(s.hour);
		dst.set_startmin(s.min);
		dst.set_startmonth(s.month);
		dst.set_startordinalweek(s.dayOfWeek);

		s = job->end_daylight();
		dst.set_enddayofweek(s.week);
		dst.set_endhour(s.hour);
		dst.set_endmin(s.min);
		dst.set_endmonth(s.month);
		dst.set_endordinalweek(s.dayOfWeek);

		dst.set_dailyadjust(9);
		QString fileName = "dst.dst";
		std::filebuf fb;
		fb.open(fileName.toStdString(), std::ios::out | std::ios::binary);
		std::ostream os(&fb);
		bool bSerialed = dst.SerializeToOstream(&os);
		fb.close();
	}
	else if (res->text() == "ZC Setting")
	{
		FLZCPropertyDlg dlg((FLZoneControllor*)node);
		dlg.exec();
		m_pProTable->setNode(node);
	}

	
}
void FLJobWindow::excute_wio_menu(QTreeWidgetItem *item, FLObject* node, QPoint pt)
{
	int num_tab = ui.tabWidget->count();
	QString type = node->getProperty("Type").value.toString();
	QMenu menu;
	QAction *a = menu.addAction(QIcon(":/firelink/Resources/icons/remove-icon-48.png"), tr("Delete WIO"));
	//a = menu.addAction(QIcon(":/firelink/Resources/icons/add-floor-icon.png"), tr("Active"));
	a = menu.addAction(QIcon(":/firelink/Resources/icons/terminal.png"), tr("Open Debug Module"));	
	a = menu.addAction(QIcon(":/firelink/Resources/icons/relay-icon-48.png"), tr("Choose Output/Input"));
	menu.addSeparator();
	a = menu.addAction(QIcon(":/firelink/Resources/icons/setting.png"), tr("WIO Setting"));
	QAction *res = 0;
	res = menu.exec(pt);
	if (res == 0) return;
	if (res->text() == tr("Delete WIO"))
	{
		auto zc = (FLZoneControllor*)node->parent();
		zc->removeWio((FLWio*)node);
		m_pJobTree->deleteNode((FLWio*)node);

		auto view = visual_floor((FLFloor*)zc->parent());
		if (view)
			view->RemoveWIO((FLWio*)node);		
	}
	else if (res->text() == "Active")
	{
		auto zc = (FLZoneControllor*)node->parent();

		auto view = visual_floor((FLFloor*)zc->parent());
		if (view)			
		{
			QStringList vList = node->getProperty("MAC Address").items;
			QString mac = "FF:FF:FF:FF:FF:FF:FF:FF";
			if (vList.size() == 8)
			{
				mac = QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(vList[0]).arg(vList[1]).arg(vList[2]).arg(vList[3]).arg(vList[4]).arg(vList[5]).arg(vList[6]).arg(vList[7]);
			}
			view->activeDevice(mac, 20);			
		}
	}
	else if (res->text() == "Write to hardware")
	{
		//open serial port
			
	}
	else if (res->text() == tr("Choose Output/Input"))
	{
		//m_job->undoStack()->push(new FLCommandAddFloor(m_job,""));
		FLChooseOutputDialog dlg((FLZoneControllor*)node->parent(), node, this);		
		if (dlg.exec() == QDialog::Accepted)
		{
			m_pProTable->setNode(node);
		}
	}
	else if (res->text() == tr("WIO Setting"))
	{
		FLWioPropertyDlg dlg((FLWio*)node);
		dlg.exec();
		m_pProTable->setNode(node);
	}
}

void FLJobWindow::on_jobtree_menu(QTreeWidgetItem * item, QPoint pt)
{
	if (item == nullptr)
	{
		QMenu menu;
		QAction *a = menu.addAction(QIcon(":/firelink/Resources/icons/add-floor-icon.png"), tr("Open Job"));
		a = menu.addAction(QIcon(":/firelink/Resources/icons/add-floor-icon.png"), tr("New Job"));
		
		QAction *res = 0;
		res = menu.exec(pt);
		if (res == 0) return;
		if (res->text() == tr("Open Job"))
		{
			on_open_job();
		}
		else
			on_new_job();

		return;
	}

	FLObject *node = qobject_cast<FLObject*>((QObject*)item->data(0,ItemPointerData).toULongLong());
	if(node == nullptr)
		return;
	int num_tab = ui.tabWidget->count();
	QString type = node->getProperty("Type").value.toString();
	if ( type == "JOB")
	{
		excute_job_menu(item, node, pt);
		return;
	}
	else if ( type == "FLOOR") 
	{
		excute_floor_menu(item, node, pt);
	}
	else if (  type == "ZC") 
	{
		excute_zc_menu(item, node, pt);
	}
	else if (type == "ZC_RELAY" || type.compare("ZC_INPUTS") == 0 || type.compare("ZC_LOCAL_TROUBLES") == 0 
		|| type.compare("ZC_REMOTE_TROUBLE") == 0 || type.compare("ZC_STATS") == 0)
	{
		QMenu menu;
		QAction *a = menu.addAction(QIcon(":/firelink/icons/add-floor-icon.png"), tr("Choose Output/Input") );
		QAction *res = 0;
		res = menu.exec(pt);
		if (res == 0) return;
		if (res->text() == tr("Choose Output/Input"))
		{
			//m_job->undoStack()->push(new FLCommandAddFloor(m_job,""));
			FLChooseOutputDialog dlg((FLZoneControllor*)node->parent(), node, this);
			if (dlg.exec() == QDialog::Accepted)
			{
				m_pProTable->setNode(node);
			}
		}
	}	
	

	else if(type == "WIO")
	{
		excute_wio_menu(item, node, pt);		
	}


}

void FLJobWindow::View_Item_changed(FLObject* obj)
{
	m_pProTable->setNode(obj);
	m_pJobTree->clearFocus();
	m_pJobTree->clearSelection();
	m_pJobTree->setFocusItem(obj);

}

void FLJobWindow::on_update_zc(FLZoneControllor *zc)
{
	//m_pJobTree->upda
	auto itm =  m_pJobTree->parentItem(zc);
	m_pJobTree->deleteNode(zc);
	m_pJobTree->addNode(itm,zc);
}

void FLJobWindow::open_floor(FLFloor* node)
{
	auto view_index = visual_floor_index(node);
	if (view_index != -1)
	{
		ui.tabWidget->setCurrentIndex(view_index);
		return;
	}
		
	
	FLFloorView* pView = new FLFloorView(ui.tabWidget);
	FLObjectPropertys p1 = node->getProperty("Name");
	int tab_index = ui.tabWidget->addTab(pView,p1.value.toString());

	pView->setFloor(node);
	ui.tabWidget->setCurrentIndex(tab_index);
	connect(pView, SIGNAL(focusItemChanged(FLObject *)),this, SLOT(View_Item_changed(FLObject* )));
	//pView->show();
	auto v_rt = ui.tabWidget->rect();
	 v_rt .setWidth( v_rt.width() - ui.dockWidget->width() - ui.dockWidget_3->width());
	 v_rt.setHeight(v_rt.height() - 240);
	pView->reset_rect(v_rt);
}

void FLJobWindow::on_view_floor()
{
	QTreeWidgetItem* item = m_pJobTree->currentItem();
	if(item == nullptr)
		return;

	FLFloor *node = qobject_cast<FLFloor*>((QObject*)item->data(0,ItemPointerData).toULongLong());
	if(node == nullptr)
		return;

	open_floor(node);
}

void FLJobWindow::on_edit_add()
{
	QTreeWidgetItem* item = m_pJobTree->currentItem();
	if(item == nullptr)
		return;

	FLObject *node = qobject_cast<FLObject*>((QObject*)item->data(0,ItemPointerData).toULongLong());
	if(node == nullptr)
		return;
	int num_tab = ui.tabWidget->count();
	QString type = node->getProperty("Type").value.toString();
	if ( type == "JOB")
	{		
		FLJob* pJob = (FLJob*) node;
		FLCreateFloorDlg dlg(pJob,false,this);
		if(dlg.exec() == QDialog::Accepted)
		{
			pJob->addFloor(dlg.Floor());
			m_pJobTree->addNode(item,dlg.Floor());
		}
		return;	


	}
	else if ( type == "FLOOR") 
	{
		//m_job->undoStack()->push(new FLCommandAddFloor(m_job,""));
		FLFloor* pFloor = (FLFloor*) node;
		FLCreateZCDlg dlg(false,pFloor,this);
		if(dlg.exec() == QDialog::Accepted)
		{
			pFloor->addZC(dlg.ZC());
			m_pJobTree->addNode(item,dlg.ZC());

			for(int n=0; n< num_tab; n++)
			{
				auto view = qobject_cast<FLFloorView*>(ui.tabWidget->widget(n));
				if(view && view->Floor() == pFloor)
				{
					view->AddFLObject(dlg.ZC());
					break;
				}

			}
		}
	}
	else if (  type == "ZC") 
	{
		auto zc = (FLZoneControllor*) node;
		//m_job->undoStack()->push(new FLCommandAddFloor(m_job,""));
		FLCreateWioDlg dlg(zc,this);
		if(dlg.exec() == QDialog::Accepted)
		{				
			zc->addWio(dlg.WIO());
			m_pJobTree->addNode(item->child(5),dlg.WIO());

			for(int n=0; n< num_tab; n++)
			{
				auto view = qobject_cast<FLFloorView*>( ui.tabWidget->widget(n));
				if(view && view->Floor() == zc->parent())
				{
					view->AddFLObject(dlg.WIO());
					break;
				}

			}

		}
	}
	else if (  type == "ZC_RELAY")
	{

	}	
}

void FLJobWindow::on_edit_edit()
{

}

void FLJobWindow::on_edit_delete()
{
	QTreeWidgetItem* item = m_pJobTree->currentItem();
	if(item == nullptr)
		return;

	FLObject *node = qobject_cast<FLObject*>((QObject*)item->data(0,ItemPointerData).toULongLong());
	if(node == nullptr)
		return;
	int num_tab = ui.tabWidget->count();
	QString type = node->getProperty("Type").value.toString();
	if ( type == "JOB")
	{


	}
	else if ( type == "FLOOR") 
	{
		FLFloor* pFloor = (FLFloor*) node;

		for(int n=num_tab-1; n>=0; n--)
		{
			auto tab =qobject_cast<FLFloorView*>(ui.tabWidget->widget(n));
			//remove tab
			if(tab && tab->Floor() == pFloor)
			{
				ui.tabWidget->removeTab(n);
				break;
			}				
		}
		//remove object
		auto job = (FLJob*)pFloor->parent();
		job->removeFloor(pFloor);
		//remove item
		m_pJobTree->deleteNode(pFloor);
	}
	else if (  type == "ZC") 
	{
		auto floor = (FLFloor*) node->parent();
		floor->removeZC((FLZoneControllor*)node);
		m_pJobTree->deleteNode((FLZoneControllor*)node);

		for(int n=0; n< num_tab; n++)
		{
			auto view = qobject_cast<FLFloorView*> (ui.tabWidget->widget(n));
			if(view && view->Floor() == floor)
			{
				view->RemoveZC((FLZoneControllor*)node);
				break;
			}

		}
	}
	else if (  type == "ZC_RELAY")
	{

	}
	else if(type == "WIO")
	{
		auto zc = (FLZoneControllor*) node->parent();
		zc->removeWio((FLWio*)node);
		m_pJobTree->deleteNode((FLWio*)node);

		for(int n=0; n< num_tab; n++)
		{
			auto view = qobject_cast<FLFloorView*> (ui.tabWidget->widget(n));
			if(view && view->Floor() == zc->parent())
			{
				view->RemoveWIO( (FLWio*)node );
				break;
			}
		}
	}
}

QToolBar* FLJobWindow::GetMainToolbar()
{
	return ui.toolBar;
}

void FLJobWindow::write_zc(FLZoneControllor * flzc)
{
	auto zone_control = flzc;
	ZC *zc = new ZC();
	zone_control->writeFirmware(zc);
	QString fileName = "out.cfg";
	std::filebuf fb;
	fb.open(fileName.toStdString(), std::ios::out | std::ios::binary);
	std::ostream os(&fb);
	bool bSerialed = zc->SerializeToOstream(&os);
	fb.close();
/*
	QByteArray data;
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return;
	data = file.readAll();
	file.close();
	*/
	//open serial port
	FLDlgSerialWrite dlg;
	//dlg.set_data(data);
	dlg.set_zc(flzc);
	connect(&dlg, SIGNAL(updata_zc(FLZoneControllor*)), this, SLOT(on_update_zc(FLZoneControllor*)));
	dlg.exec();
}

FLFloorView * FLJobWindow::visual_floor(FLFloor * floor)
{
	int num_tab = ui.tabWidget->count();	
	for (int n = 0; n< num_tab; n++)
	{
		auto view = qobject_cast<FLFloorView*> (ui.tabWidget->widget(n));
		if (view && view->Floor() == floor)		
			return view;
	}
	return nullptr;
}

int FLJobWindow::visual_floor_index(FLFloor * floor)
{
	int num_tab = ui.tabWidget->count();
	for (int n = 0; n< num_tab; n++)
	{
		auto view = qobject_cast<FLFloorView*> (ui.tabWidget->widget(n));
		if (view && view->Floor() == floor)
			return n;
	}
	return -1;
}
