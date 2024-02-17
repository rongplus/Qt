#include "flzigbeenetworkmonitor.h"

#include "ZBType.h"

#include "zigbee.h"
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <QSettings>
#include <QMessageBox>
#include <QToolBar>

#include "fljobprotol.h"
#include "SnifferMC1322x.h"
#include "SavedFrames.h"
#include "MACFrame.h"
#include "MACFrameDef.h"
#include "Helper.h"
#include "NWKFrameDef.h"
#include "Spoiler.h"

#include <QtEndian>
#include <QTreeWidgetItem>
#include <QApplication>
#include <QFile>

#include <QTime>
#include <QFileInfo>
#include <QPdfWriter>
#include <qpainter.h>

#include <QDesktopServices>

using namespace std;


int RSA_test();
int AES_test();

NetworkView::NetworkView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	ui.comboBox->clear();
	foreach(QSerialPortInfo port, QSerialPortInfo::availablePorts())
	{
		if (port.isValid() && !port.isBusy())
			ui.comboBox->addItem(port.portName() + " (" + port.description() + ")", port.portName());
	}

	ui.ch_comboBox->clear();
	for (int i = 11; i <= 26; i++)
	{
		ui.ch_comboBox->addItem("Channel " + QString::number(i), i);
	}


	QSettings st("Mircom", "FireLink");
	QString str = st.value("Default_PCAP_Path").toString();

	if (str.isNull()) {		str = "";	}

	m_dirPath = str;

	ui.btnDir->setToolTip(QString("Current Dir: %1\r\nClick to Change").arg(str));
	ui.btnDir->setShortcut(tr("Ctrl+D"));
	ui.btnDir->setIcon(QIcon::fromTheme("folder-new"));

	//connect(ui.btnDir, SIGNAL(triggered()), this, SLOT(onSaveDir()));

	onSwitch = new Switcher(this);
	ui.gridLayout_0->addWidget(onSwitch, 0, 4); //add it to the layout

	scrollSwitch = new Switcher(this);
	ui.gridLayout_1->addWidget(scrollSwitch, 0, 2); //add it to the layout

	logSwitch = new Switcher(this);
	ui.gridLayout_2->addWidget(logSwitch, 0, 2); //add it to the layout

	connect(logSwitch, &Switcher::toggled, this, &NetworkView::onLog);

	connect(scrollSwitch, &Switcher::toggled, this, &NetworkView::onScroll);

	connect(onSwitch, &Switcher::toggled, this, &NetworkView::onzbScan);
	
	connect(ui.pb_openjob, SIGNAL(clicked()), this, SLOT(onOpenJob()));
	connect(ui.cb_floor, SIGNAL(currentIndexChanged(int)), this, SLOT(floorChanged(int)));

	
	
	m_serial = nullptr;
	index = 0;
	total = 10;

	// sniffer
	sniffer = new SnifferMC1322x();
	snifferMain = new SnifferMain();
	//mainWindow = new MainWindow();

	connect(sniffer, &SnifferMC1322x::notifyReceivedFrame, this, &NetworkView::receivedFrame);

	file = false;
	itemIndex = 0;

	openAction = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton), tr("Open"), this);
	openAction->setShortcut(tr("Ctrl+O"));
	openAction->setDisabled(false);

	playAction = new QAction(style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this);
	playAction->setShortcut(tr("Ctrl+P"));
	playAction->setDisabled(true);

	pauseAction = new QAction(style()->standardIcon(QStyle::SP_MediaPause), tr("Pause"), this);
	pauseAction->setShortcut(tr("Ctrl+A"));
	pauseAction->setDisabled(true);
	stopAction = new QAction(style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"), this);
	stopAction->setShortcut(tr("Ctrl+S"));
	stopAction->setDisabled(true);

	QToolBar *bar = new QToolBar();
	bar->addAction(openAction);
	bar->addAction(playAction);
	bar->addAction(pauseAction);
	bar->addAction(stopAction);
	ui.gridLayout_3->addWidget(bar, 0, 0);


	connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(playAction, SIGNAL(triggered()), this, SLOT(playFile()));
	connect(pauseAction, SIGNAL(triggered()), this, SLOT(pauseFile()));

	loadFileName = new QLabel();
	ui.gridLayout_3->addWidget(loadFileName, 0, 1);

	ui.tabWidget->setTabPosition(QTabWidget::West);

	nv.append(new flnetview(snifferMain));
	ui.tabWidget->addTab(nv[0], "PAN ID: ?");

	mPanList.clear();

	//RSA_test();
	//AES_test();
	
}

NetworkView::~NetworkView()
{
	if(onSwitch->isEnabled())
		sniffer->close();
	if (pauseAction->isEnabled()) 
	{
		reader->pauseReader();
		reader->terminate();		
	}
		
}

void NetworkView::set_port(QSerialPort * port)
{
	Q_ASSERT(port);
	m_serial = port;
	bool check = true;
	check = check &	m_serial->setBaudRate(QSerialPort::Baud38400);

	check = check & m_serial->setFlowControl(QSerialPort::NoFlowControl);
	is_new_command = true;
	index = 0;
	total = 10;
	connect(m_serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	onSwitch->setToggle(true);
}

void NetworkView::openSniffer(QString port, int channel)
{
	ui.comboBox->setEnabled(false);
	ui.ch_comboBox->setEnabled(false);
	QSerialPort::SerialPortError retCode = (QSerialPort::SerialPortError) sniffer->open(port, channel, (QSerialPort::FlowControl)ui.cbZS100->isChecked());
}

void NetworkView::set_floor(FLFloor * fl)
{
	nv[ui.tabWidget->currentIndex()]->getFloorView()->setFloor(fl);
}

void NetworkView::set_zc(FLZoneControllor * zc)
{
	set_floor((FLFloor*)zc->parent());
	nv[ui.tabWidget->currentIndex()]->getFloorView()->set_current_object(zc);
}

void NetworkView::receivedFrame(const QByteArray& frmBin)
{
	ZBTime zt  = currentTime();	
	ProcessPacket(frmBin, zt);
}

void NetworkView::addItem(QList<QString> list, QString data, int index)
{	
	if (list[7] != "-")
		nv[index]->insertItem(list, data);
}

int  NetworkView::getPanID(QList<QString> list) 
{	
	if (list[10] == "Beacon Request" || list[10] == "Acknowledge")
		return  0;	

	QString panID = list[11];

	int sIndex = mPanList.indexOf(panID);	
	//qDebug() << "Get PAN ID " << sIndex << mPanList.join(" ");

	if (sIndex == -1 && panID != "")
	{		
		sIndex = mPanList.length();
		mPanList.append(panID);

		if (sIndex != 0) 
		{
			nv.append(new flnetview(snifferMain));
			nv[sIndex]->setScroll(scrollSwitch->isEnabled());
			ui.tabWidget->addTab(nv[sIndex], "PAN ID: " + panID);					
		}
		else
		{
			ui.tabWidget->setTabText(0, "PAN ID: " + panID);
		}
	}
	
	return sIndex;
}

void NetworkView::addTrafficItem(int idx)
{
	const ZTCFrame& frame = SavedFrames::instance()->ztcFrame(idx);
	if (frame.empty())
		return;

	const SavedFrames::Saved& saved = SavedFrames::instance()->frame(idx);

	QList<QString> list = snifferMain->getTrafficItem( frame, saved);

	QString data = "";

	for (int i = 0; i< frame.payload().size(); i++)
		data += toHex(frame.payload().at(i)) + " ";	

	int panID = getPanID(list);
	addItem(list, data, panID);

}

void NetworkView::onConnect()
{
	auto currentPortName = ui.comboBox->currentData().toString();
	auto serial = new QSerialPort(currentPortName, this);

	if (!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Error"), serial->errorString());
		delete serial;
		return;
	}
	m_serial = serial;

	bool check = true;
	check = check &	m_serial->setBaudRate(QSerialPort::Baud38400);
	//check = check & m_serialPort->setDataBits(QSerialPort::Data8);
	//check = check & m_serialPort->setStopBits(QSerialPort::OneStop);
	//check = check & m_serialPort->setParity(QSerialPort::NoParity);
	check = check & m_serial->setFlowControl(QSerialPort::NoFlowControl);

	is_new_command = true;

	index = 0;
	total = 10;

	connect(m_serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

}

void NetworkView::onReadyRead()
{
	QByteArray arr = m_serial->readAll();


	QString str_comm;
	int m;
	for (int n = 0; n< arr.length(); n++)
	{
		//str_comm += QString::number(arr[n],16) + " ";
		m = arr[n];
		zb_data.raw[index] = m;
		//str_comm = toHex(m);

		if (is_new_command && m == SYNC)
		{
			//data_received.clear();
			index = 0;
			//ui.terminal->insertPlainText("RX: " + str_comm );
			//data_received +=  str_comm;	
			is_new_command = false;
		}
		else
		{
			//data_received +=  str_comm;

			if (index == 3) {
				total = (unsigned char)arr[n];
				total += 4;

			}
			else if (index == total)
			{
				index = 0;
				total = 10;
				if (zb_data.packet.OG == 0x86 &&
					zb_data.packet.OC == 0x03)
				{
					receivedFrame(QByteArray(zb_data.raw));;
				}

				is_new_command = true;
			}
		}
		index++;
	}	
}

void NetworkView::ProcessPacket(QByteArray arr, ZBTime zt)
{
	ZBBytes bytes = toBytes(arr);

	QString data = "";

	for (int i = 0; i< bytes.size(); i++)
		data += toHex(bytes[i]) + " ";


	QList<QString> list = snifferMain->getTrafficItem(0, bytes, zt);


	int panID = getPanID(list);

	nv[panID]->setFile(true);
	addItem(list, data, panID);

	if (list[12] != "")
		list[12] = list[12];
	ZBInt32 idx = SavedFrames::instance()->addFrame(ZB_RX, bytes);
}

void NetworkView::readPCAP(QString filename)
{
	pcap_hdr_t pcap_hdr;
	pcaprec_hdr_t pcaprec_hdr;
	mac_hdr_t *mac_hdr;
	qint64 bufSize = 1024;
	char *buf = new char[bufSize];

	qint64 dataSize;

	QSettings st("Mircom", "FireLink");
	QString str = st.value("Default_Job_Path").toString();
	
	QString fileName = filename;
	QFile fileToRead(fileName);

	fileToRead.open(QIODevice::ReadOnly);
	fileToRead.read((char*)&pcap_hdr, sizeof(pcap_hdr_t));

	if (pcap_hdr.magic_number == 0xd4c3b2a1 || pcap_hdr.magic_number == 0xa1b2c3d4)
	{
		int j = 0;
		itemIndex = 0;

		QTime myTimer;
		myTimer.start();
		// do something..
		file = true;

		nv[0]->getsData().clear();
		nv[0]->clear_table();

		while (!fileToRead.atEnd())
		{
			fileToRead.read((char*)&pcaprec_hdr, sizeof(pcaprec_hdr_t));
			int len;
			ZBTime zt;
			if (pcap_hdr.magic_number == 0xd4c3b2a1)
			{
				len = qToBigEndian(pcaprec_hdr.incl_len);
				zt = ZBTime(qToBigEndian(pcaprec_hdr.ts_sec), qToBigEndian(pcaprec_hdr.ts_usec));
			}
			else
			{
				len = (pcaprec_hdr.incl_len);
				zt = ZBTime((pcaprec_hdr.ts_sec), (pcaprec_hdr.ts_usec));
			}

			QByteArray arr = fileToRead.read(len);
			ProcessPacket(arr, zt);


			j++;
		}

		int nMilliseconds = myTimer.elapsed();
		QMessageBox::critical(this, "File loaded", tr("Total <b>%1</b> packets loaded in <br><b>%2</b> seconds.").arg(j).arg(nMilliseconds / 1000.0));
		nv[0]->set_table_rowcount(j);
	}
	else
	{
		QMessageBox::critical(this, "File read error", tr("Invalid pcap file: <b>%1</b>").arg(filename));
	}

	//dataStreamReader >> pcap_hdr.magic_number >> pcap_hdr.version_major >> pcap_hdr.version_minor >> pcap_hdr.thiszone >> pcap_hdr.sigfigs >>pcap_hdr.snaplen >> pcap_hdr.network;
	fileToRead.close();
}

void NetworkView::playFile()
{
	playAction->setDisabled(true);
	pauseAction->setDisabled(false);

	if (reader->isPaused())
		reader->resumeReader();
	else
		reader->start();

}

void NetworkView::pauseFile()
{
	pauseAction->setDisabled(true);
	playAction->setDisabled(false);
	reader->pauseReader();

}

void NetworkView::openFile()
{
	//dialog.setNameFilter(tr("PACP (*.pcap)"));
	QString str_file_name = QFileDialog::getOpenFileName(this, "Choose a PCAP File ","", tr("All PACP (*.pcap)"));
	if (str_file_name.isNull())
		return;

	
	loadFileName->setText("File Name: \r\n" + str_file_name.mid(str_file_name.lastIndexOf('/')));
	
	reader = new PCAPReader(str_file_name, this);
		
	//auto res = connect(reader, &PCAPReader::sendPacket, this, &NetworkView::ProcessPacket);

	auto res = connect(reader, SIGNAL(sendPacket(QByteArray, ZBTime)), this, SLOT(ProcessPacket(QByteArray, ZBTime)));

	res = connect(reader, SIGNAL(Finished(int)), this, SLOT(readFinished(int)));

	//readPCAP(str_file_name);

	QDir d = QFileInfo(str_file_name).absoluteDir();


	playAction->setDisabled(false);
}

void NetworkView::onPrint()
{
	nv[ui.tabWidget->currentIndex()]->print_table("OK");

}

void NetworkView::openSniffer()
{
	auto currentPortName = ui.comboBox->currentData().toString();
	auto currentChannel = ui.ch_comboBox->currentData().toInt();
	openSniffer(currentPortName, currentChannel);
}

void NetworkView::onScroll(bool enable)
{
	nv[0]->setScroll(enable);
	for (int i = 0; i < mPanList.size(); i++) 
		nv[i]->setScroll(enable);
}

void NetworkView::onSaveDir()
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

void NetworkView::onLog(bool enable)
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

void NetworkView::onzbScan(bool enable)
{
	if (enable)
	{
		ui.comboBox->setEnabled(false);
		//ui.openFile->setEnabled(false);
		ui.ch_comboBox->setEnabled(false);
		openSniffer();
	}
	else
	{
		sniffer->close();
		ui.comboBox->setEnabled(true);
		//ui.openFile->setEnabled(true);
		ui.ch_comboBox->setEnabled(true);		
	}
}

void NetworkView::onOpenJob()
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

void NetworkView::floorChanged(int index)
{
	if (index == -1)
		return;
	FLFloor* floor = (FLFloor*)ui.cb_floor->currentData().toULongLong();
	if (floor == nullptr)
		return;
	//ui.tabWidget->currentIndex();
	nv[ui.tabWidget->currentIndex()]->getFloorView()->setFloor(floor);

}

void NetworkView::showEvent(QShowEvent * event)
{
	QWidget::showEvent(event);	
}

void NetworkView::readFinished(int packets)
{
	QMessageBox::critical(this, "File read", QString("Read complete total %1 packets").arg(packets));
	openAction->setDisabled(false);
	playAction->setDisabled(true);
	pauseAction->setDisabled(true);
}