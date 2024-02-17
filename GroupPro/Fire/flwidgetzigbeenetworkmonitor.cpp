#include "flwidgetzigbeenetworkmonitor.h"
#include <QSerialPort>
#include <QTime>
#include <QSerialPortInfo>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <QSettings>
#include <QMessageBox>

#include "SnifferMC1322x.h"
#include "SavedFrames.h"
#include "MACFrame.h"
#include "MACFrameDef.h"
#include "Helper.h"
#include "NWKFrameDef.h"
#include "Spoiler.h"
#include "fljobprotol.h"
using namespace std;


FLWidgetZigbeeNetworkMonitor::FLWidgetZigbeeNetworkMonitor(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.widget_3, &Switcher::toggled, this, &FLWidgetZigbeeNetworkMonitor::onLog);
	connect(ui.widget_2, &Switcher::toggled, this, &FLWidgetZigbeeNetworkMonitor::onScroll);
	connect(ui.widget, &Switcher::toggled, this, &FLWidgetZigbeeNetworkMonitor::onzbScan);

	sniffer = new SnifferMC1322x();
	snifferMain = new SnifferMain();
	//mainWindow = new MainWindow();

	connect(sniffer, &SnifferMC1322x::notifyReceivedFrame, this, &FLWidgetZigbeeNetworkMonitor::receivedFrame);

	foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts())
	{
		if (port.isValid() && !port.isBusy())
			ui.comboBox->addItem(port.portName() + " (" + port.description() + ")", port.portName());
	}


	for (int i = 11; i <= 26; i++)
	{
		ui.ch_comboBox->addItem("Channel " + QString::number(i), i);
	}

	connect(ui.pb_openjob, SIGNAL(clicked()), this, SLOT(onOpenJob()));
	connect(ui.cb_floor, SIGNAL(currentIndexChanged(int)), this, SLOT(floorChanged(int)));
	connect(ui.btnDir, SIGNAL(clicked()), this, SLOT(onSaveDir()));
	connect(ui.btnPrint, SIGNAL(clicked()), this, SLOT(on_snapshot()));

}

FLWidgetZigbeeNetworkMonitor::~FLWidgetZigbeeNetworkMonitor()
{

}

void FLWidgetZigbeeNetworkMonitor::onLog(bool enable)
{
	if (enable)
	{
		if (m_dirPath == "")
		{
			onSaveDir();
		}

		QString text = QDate::currentDate().toString("yyyy-MM-dd") + "-" + QTime::currentTime().toString().replace(":", ".");
		qDebug() << text;

		ui.saveFileName->setText("File Name: \r\n/" + text + ".pcap");

		sniffer->setSaveCapture(true, m_dirPath + "/" + text + ".pcap");

	}
	else
	{
		ui.saveFileName->setText("");
		sniffer->setSaveCapture(false, "");
	}
}



void FLWidgetZigbeeNetworkMonitor::onSaveDir()
{
	QSettings st("Mircom", "FireLink");
	QString str = st.value("Default_PCAP_Path").toString();

	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	//dialog.selectFile(text);
	QString str_file_name = dialog.getExistingDirectory(this, "Set PCAP Dir", str);
	if (str_file_name.isNull()) {

		return;
	}

	st.setValue("Default_PCAP_Path", str_file_name);
	ui.btnDir->setToolTip(QString("Current Dir: %1\r\nClick to Change").arg(str_file_name));

	m_dirPath = str_file_name;
}

void FLWidgetZigbeeNetworkMonitor::on_snapshot()
{
	FLWidgetZigbeeNetworkView * view = (FLWidgetZigbeeNetworkView*)ui.tabWidget->currentWidget();
	if (view)
		view->snapshot("OK");
}


void FLWidgetZigbeeNetworkMonitor::onScroll(bool enable)
{
	for(int n=0; n< ui.tabWidget->count(); n++)
	{
		((FLWidgetZigbeeNetworkView*)ui.tabWidget->widget(n))->set_auto_scroll(enable);
	}
	
}

void FLWidgetZigbeeNetworkMonitor::onzbScan(bool enable)
{
	if (enable)
	{
		ui.tabWidget->clear();
		ui.comboBox->setEnabled(false);
		//ui.openFile->setEnabled(false);
		ui.ch_comboBox->setEnabled(false);

		ui.comboBox->setEnabled(false);
		ui.ch_comboBox->setEnabled(false);
		auto currentPortName = ui.comboBox->currentData().toString();
		auto currentChannel = ui.ch_comboBox->currentData().toInt();

		QSerialPort::SerialPortError retCode = (QSerialPort::SerialPortError) sniffer->open(currentPortName, currentChannel, (QSerialPort::FlowControl)ui.cbZS100->isChecked());
		if (retCode == QSerialPort::NoError)
		{
			//return;
			ui.tabWidget->clear();

			ui.groupBox_5->setEnabled(false);
			if (ui.cb_floor->count() == 0)
				return;
			FLFloor* floor = (FLFloor*)ui.cb_floor->currentData().toULongLong();
			if (floor == nullptr)
				return;
			//ui.tabWidget->currentIndex();
			auto zc_count = floor->countZC();
			for (size_t i = 0; i < zc_count; i++)
			{
				auto zc = floor->getZC(i);
				auto id = zc->getProperty("PAN ID").value.toString();
				FLWidgetZigbeeNetworkView* view = new FLWidgetZigbeeNetworkView(id, this);
				view->set_floor(floor);
				ui.tabWidget->addTab(view, "Pan " + id);
			}
			
		}

	}
	else
	{
		sniffer->close();
		ui.groupBox_5->setEnabled(true);
		ui.comboBox->setEnabled(true);
		//ui.openFile->setEnabled(true);
		ui.ch_comboBox->setEnabled(true);
		
	}
}

void FLWidgetZigbeeNetworkMonitor::receivedFrame(const QByteArray& frmBin)
{
	ZBTime zt = currentTime();
	ProcessPacket(frmBin, zt);
}


void FLWidgetZigbeeNetworkMonitor::ProcessPacket(QByteArray arr, ZBTime zt)
{
	//_mutex.lock();
	ZBBytes bytes = toBytes(arr);

	QString data = "";

	for (int i = 0; i< bytes.size(); i++)
		data += toHex(bytes[i]) + " ";


	QList<QString> list = snifferMain->getTrafficItem(0, bytes, zt);

	if (list[10] == "Beacon Request" || list[10] == "Acknowledge")
		return ;

	QString panID = list[11];

	FLWidgetZigbeeNetworkView* view = getPanID(panID);
	
	view->process_data(list, data);
	view->addFrame(ZB_RX, bytes);
	//_mutex.unlock();
	//ZBInt32 idx = SavedFrames::instance()->addFrame(ZB_RX, bytes);
}


FLWidgetZigbeeNetworkView*  FLWidgetZigbeeNetworkMonitor::getPanID(QString panID)
{
	for (int n = 0; n < ui.tabWidget->count(); n++)
	{
		FLWidgetZigbeeNetworkView* view = (FLWidgetZigbeeNetworkView*) ui.tabWidget->widget(n);
		if (view->pan_id().right(4) == panID.right(4)) //ignore 0x
		{
			return view;
		}
	}
	FLWidgetZigbeeNetworkView* view = new FLWidgetZigbeeNetworkView(panID,this);

	ui.tabWidget->addTab(view, "Pan " + panID);

	if (ui.cb_floor->count() != 0)
	{
		FLFloor* floor = (FLFloor*)ui.cb_floor->currentData().toULongLong();
		
		if (floor != nullptr)
		{
			int nZC = floor->countZC();
			for (int n = 0; n < nZC; n++)
			{
				if (floor->getZC(n)->getProperty("PAN ID").value.toString().right(4) == panID.right(4))
				{
					view->set_floor(floor);
					return view;
				}
			}
			
			FLZoneControllor* zc = nullptr;
			{
				zc = new FLZoneControllor(floor);
				zc->Init();
				zc->InitDefaultIO();
				zc->addProperty("Name", panID);
				zc->addProperty("PAN ID", panID);
				zc->setProperty("Short Address", "0x0000");
				//qDebug() << "pan id" << zc->getProperty("PAN ID").value.toString();
				floor->addZC(zc);
			}
			view->set_floor(floor);
		}
	}	
	else
	{
		//;
	}

	return view;
}


void FLWidgetZigbeeNetworkMonitor::onOpenJob()
{
	ui.cb_floor->clear();
#if 1 
	QSettings st("Mircom", "FireLink");
	QString str = st.value("Default_Job_Path").toString();

	QString str_file_name = QFileDialog::getOpenFileName(this, "Choose a Job ", str);
	if (str_file_name.isNull())
		return;
#else
	QString str_file_name = "C:\\My Work\\config-jobs\\lab";
#endif
	Pro_Job *pro_job = new Pro_Job();
	fstream input(str_file_name.toStdString(), ios::in | ios::binary);
	pro_job->ParseFromIstream(&input);
	FLJob* job = new FLJob(this);
	FLJobProtol::Proto2Object(pro_job, job);

	auto floor_count = job->countFloor();
	for (int n = 0; n< floor_count; n++)
	{
		FLFloor* floor = job->getFloor(n);
		ui.cb_floor->addItem(floor->getProperty("Name").value.toString(), quintptr(floor));
	}
	input.close();

}


void FLWidgetZigbeeNetworkMonitor::floorChanged(int index)
{
	return;

	if (index == -1)
		return;
	FLFloor* floor = (FLFloor*)ui.cb_floor->currentData().toULongLong();
	if (floor == nullptr)
		return;
	//ui.tabWidget->currentIndex();
	FLWidgetZigbeeNetworkView* view =(FLWidgetZigbeeNetworkView*) ui.tabWidget->widget(ui.tabWidget->currentIndex());
	//nv[ui.tabWidget->currentIndex()]->getFloorView()->setFloor(floor);
	if(view)view->set_floor(floor);
	//sniffer->reset();
}