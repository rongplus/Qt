#include "flotawidget.h"
#include "zc.pb.h"

#include <iostream>
#include <fstream>
#include "fljobprotol.h"
#include <QMessageBox>
#include <QFile>
#include <fstream>
#include <iostream>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QFile>
#include <QDebug>


typedef unsigned char  BYTE; // 1byte
typedef uint32_t  LWORD; // 1byte


#include "flglobal.h"
#include "flxmlcommandmanager.h"
using namespace std;

const QString PARAMETER_TYPE_EDIT = "tEdit";
const QString PARAMETER_TYPE_ENUM = "tEnum";

typedef struct SCU_CODE_VERS_ENTRY_t
{
	BYTE     CPUType;
	char      szProdID[15];      // "FireLink"
	char      szCopyright[40];
	LWORD   VersNo;            // MAJOR/MINOR MUST BE SAME ON ALL CPUS
	BYTE     SysType;
	BYTE     LangType;
	BYTE     HWType;            // MUST BE SAME AS gSCUHWInfo
	LWORD   HWVers;            // MAJOR/MINOR MUST BE SAME AS gSCUHWInfo
	char date[21];
	int32_t sign;
} SCU_CODE_VERS_ENTRY;

void FLOTAWidget::PrintArray(QByteArray rcvBytes, QString name) {
	QString data = rcvBytes.toHex().toUpper();

	QString str_comm;
	for (int n = 0; n< data.length(); n += 2)
	{
		str_comm += QString("%1 ").arg(data.mid(n, 2));
	}

	//qDebug() << name << str_comm;
}

FLOTAWidget::FLOTAWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	
	connect(ui.widget_2, SIGNAL(state_change(int)), this, SLOT(on_state_changed(int)));
	

	ui.gbVZC->setEnabled(false);

	ui.gbNetwork->setEnabled(false);

	_port = nullptr;
	_zcport = new FLZCSerialPort(this);	

	read_time_after_login = true;

	write_time_after_login = false;
	auto_login =false;
	_portIO = nullptr;
	_portIO = new FLSerialPortIO(this);
	connect(_portIO, SIGNAL(state_change(int)), this, SLOT(on_io_state_changed(int)));
	_portIO->start();
	is_running = false;

	OTA_active = false;			

	connect( ui.pushButton_8, SIGNAL(clicked()), this, SLOT(on_clear_cmd()));
}

FLOTAWidget::~FLOTAWidget()
{
	if (_portIO)
	{
		_portIO->set_stop();
		_portIO->quit();
		if(_portIO->isRunning())
			_portIO->wait();
		delete _portIO;
		_portIO = nullptr;
		_port = nullptr;
	}

	if (_port)
		_port->close();
}

void FLOTAWidget::on_read()
{	
	
}

void FLOTAWidget::on_write()
{
	
}

void FLOTAWidget::on_login()
{
	
}

void FLOTAWidget::on_io_state_changed(int state)
{
	

}
void FLOTAWidget::on_textChanged(const QString & text)
{	
	
}
void FLOTAWidget::tab_ChangeRequested(int index)
{
	QString con;
	if (index == 0)
		con = "ZC Time";
	else if (index == 1)
		con = "Day Light Saving";
	else if (index == 2)
		con = "Passcode";
	else if (index == 3)
		con = "Firmware";
	else if (index == 4)
		con = "Configuration";

}
void FLOTAWidget::on_ota_changed(int)
{

}
void FLOTAWidget::on_state_changed(int state)
{
	if (sender() == ui.widget_2)
	{
		if (state == 1)
		{
		
			ui.gbVZC->setEnabled(true);
			_ota_port = ui.widget_2->get_port();
			bool check = true;
			check = check &	_ota_port->setBaudRate(QSerialPort::Baud38400);
			check = check & _ota_port->setFlowControl(QSerialPort::NoFlowControl);
			connect(_ota_port, SIGNAL(readyRead()), this, SLOT(onOTAReadyRead()));
			_ota_port->write("\r");
		
		}
		else
		{
			ui.gbVZC->setEnabled(false);
		
		}
		
		return;
	}

	if (state == 1)
	{
	
	}
	else
	{
		
	}

}

void FLOTAWidget::read()
{
	int index =0;
	switch (index)
	{
	case 0:
		read_time();
		break;
	case 1:
		read_daylightsaving();
		break;
	case 2:
		read_passcode();
		break;
	case 3:
		read_firmware();
		break;
	case 4:
		read_configuration();
		break;
	default:
		break;
	}
}

void FLOTAWidget::write()
{
	int index = 0;
	switch (index)
	{
	case 0:
		write_time();
		break;
	case 1:
		write_daylightsaving();
		break;
	case 2:
		write_passcode();
		break;
	case 3:
		write_firmware();
		break;
	case 4:
		write_configuration();
		break;
	default:
		break;
	}
}

bool FLOTAWidget::read_cmd(QString cmd, QString filename)
{
	

	return false;
}

void FLOTAWidget::read_time()
{
	
}

void FLOTAWidget::read_daylightsaving()
{
	QString filename = "dst_save.dst";
	if (read_cmd("dst", filename))
	{
	}
}

void FLOTAWidget::read_passcode()
{
	QString filename = "passcode_save.pwd";
	if (read_cmd("passcode", filename))
	{
	}
}

void FLOTAWidget::read_firmware()
{
	QString filename = "firmware_save.fm";
	if (read_cmd("firmware", filename))
	{
	}
}

void FLOTAWidget::read_configuration()
{	
	QString filename = "config_save.cfg";
	if (read_cmd("config", filename))
	{
	}
}

void FLOTAWidget::write_time()
{
	
}

void FLOTAWidget::write_daylightsaving()
{
	

}

void FLOTAWidget::write_passcode()
{	
	
}

void FLOTAWidget::write_firmware()
{
	FLZoneControllor* _zc;
	Q_ASSERT(_zc);
	auto zone_control = _zc;
	ZC *zc = new ZC();

	//ui.label_login->setText("Send data...");
	delete zc;
	//delete[] buffer;
}

void FLOTAWidget::write_configuration()
{
	QByteArray data;
	FLZCSerialPort::TransitState ret = _zcport->send_config(_port, data, 80);
}



#pragma region OTA

void FLOTAWidget::onStartDevice()
{
	
}

void FLOTAWidget::onStopDevice()
{
	
}


void FLOTAWidget::excute_command(QString cmd)
{
	auto name = cmd;
	FLXMLCommandManager*ZigBeePro_cmd = FLXMLCommandManager:: GetZigBeeProCMD();

	auto name_list = name.split(" ", QString::SkipEmptyParts);
	auto node = ZigBeePro_cmd->find_node_byName(name_list[0]);

	if (node.isNull() == false)
	{
		auto comm_text = node.firstChildElement("CmdHeader").text();
		auto head_list = comm_text.split(" ");
		
		QTreeWidgetItem* top_item = new QTreeWidgetItem();
		ui.treeWidget_2->addTopLevelItem(top_item);

		QLabel* label = new QLabel(this);
		label->setText(name);
		ui.treeWidget_2->setItemWidget(top_item, 0, label);

		bool bOk;
		QByteArray arr;
		//header
		char TX = 0x02;
		arr.append(TX);
		foreach(auto str, head_list)
		{
			int hex = str.toInt(&bOk, 16);
			arr.append(hex);
		}
		arr.append('0');

		QTreeWidgetItem* tree_item = new QTreeWidgetItem();
		set_item(tree_item, QStringList() << "Sync" << "[1 byte]" << "02");
		top_item->addChild(tree_item);
		tree_item = new QTreeWidgetItem();
		set_item(tree_item, QStringList() << "OpGroup" << "[1 byte]" << head_list[0]);//setText(0,"OpGroup" + str6_blank + "[1 byte]"+ head_list[0]);
		top_item->addChild(tree_item);
		tree_item = new QTreeWidgetItem();
		set_item(tree_item, QStringList() << "OpOption" << "[1 byte]" << head_list[1]);//tree_item->setText(0, item_dis_text.arg("OpGroup",8).arg(1,2).arg(head_list[1],2));
		top_item->addChild(tree_item);
		QTreeWidgetItem *len_item = new QTreeWidgetItem();
		set_item(len_item, QStringList() << "Lenth" << "[1 byte]" << head_list[1]);//tree_item->setText(0, item_dis_text.arg("OpGroup",8).arg(1,2).arg(head_list[1],2));
		top_item->addChild(len_item);


		int number_value = name_list.count();
		for (int n = 1; n < number_value; n++)
		{
			auto value = name_list[n];
			if (value.startsWith("0x", Qt::CaseInsensitive))
				value = value.right(value.length() - 2);

			while (value.length() >= 2)
			{
				QString right = value.right(2);
				int hex = right.toInt(&bOk, 16);
				arr.append(hex);
				value = value.left(value.length() - 2);
			}
			tree_item = new QTreeWidgetItem();
			set_item(tree_item, QStringList() << "OpOption" << QString("[%1 byte]").arg(name_list[n].length() / 2) << name_list[n]);//tree_item->setText(0, item_dis_text.arg("OpGroup",8).arg(1,2).arg(head_list[1],2));
			top_item->addChild(tree_item);
		}

		char ch_len = arr.length() - 4;

		arr[3] = ch_len;
		arr.append(Checksum(arr, arr.length()));
		_ota_port->write(arr);
	}
}


void FLOTAWidget::set_item(QTreeWidgetItem* item, QStringList &lt)
{
	int index = 0;
	QString str1 = " ";
	QString item_text;
	int len = 14;
	item->setFlags(item->flags() ^ Qt::ItemIsTristate);
	lt[0] = lt[0].left(3);
	foreach(auto str, lt)
	{
		if (str.length() < len)
		{
			str += str1.repeated(len - str.length());
		}
		item_text += str.toUpper();

	}
	item->setText(0, item_text);
	index++;
}


void FLOTAWidget::onTimer()
{
	QString address = "0x0000";
	excute_command("NLDE-DATA.Request 0x02 " + OTA_Address + " 0x02 0x00 0x06 0x06 0x00 0x01 0xA0 0x01");
	_timer.start(50000);
}

QString FLOTAWidget::openFile()
{
#if 1 
	QSettings st("Mircom", "FireLink");
	QString str = st.value("Default_BIN_Path").toString();

	QString str_file_name = QFileDialog::getOpenFileName(this, "Choose a BIN file ", str);
	if (str_file_name.isNull())
		return "";

	QFile fi(str_file_name);
	if (fi.exists() == false)
	{
		QMessageBox::warning(this, "Error ", "Please choose right  file.");

		return "";
	}
#else
	QString str_file_name = "C:\\My Work\\config-jobs\\lab";
#endif


	QDir d = QFileInfo(str_file_name).absoluteDir();

	st.setValue("Default_BIN_Path", d.absolutePath());

	return str_file_name;
}

void FLOTAWidget::onOpenCF()
{

	
}


void FLOTAWidget::onOpenZF()
{
	

}

void FLOTAWidget::onOpenWF()
{
	

}
void FLOTAWidget::write_zcfile(QString fileName, FLZoneControllor * flzc)
{
	auto zone_control = flzc;
	ZC *zc = new ZC();
	zone_control->writeFirmware(zc);
	
	std::filebuf fb;
	fb.open(fileName.toStdString(), std::ios::out | std::ios::binary);
	std::ostream os(&fb);
	bool bSerialed = zc->SerializeToOstream(&os);
	fb.close();
	
}

void FLOTAWidget::onCancelFW()
{
	ui.gbNetwork->setDisabled(false);
}

void FLOTAWidget::onGetDevList()
{
	sendPacket(OTA_IMAGE_NOTIFY);
}

QString FLOTAWidget::FindNextDevice(QString prev) 
{
	QString Address= "";
	for (int i = 0; i < ui.tblWIO->rowCount(); i++)
	{
		QTableWidgetItem * pWidget = ui.tblWIO->item(i, 3);
		//QWidget *pWidget = (QWidget *)ui.tblWIO->item(i, 3);
		if (pWidget != NULL)
		{
			
			if (pWidget->checkState())
			{
				QTableWidgetItem *pCell = ui.tblWIO->item(i, 0);

				if (pCell)
				{
					Address = pCell->text();
					if (Address == prev)
					{
						Address = "";
						pWidget->setText("Done");
						Qt::ItemFlags eFlags = pWidget->flags();
						eFlags ^= Qt::ItemIsUserCheckable;
						
						pWidget->setFlags(eFlags);
						pWidget->setCheckState(Qt::Unchecked);
						pWidget->setBackgroundColor(QColor(0, 255, 0, 0xff));
						
					}
				}
			}
		}

	}

	return Address;

}

void FLOTAWidget::UpdateDevice()
{
	
	//ui.pbWriteFW->setDisabled(true);
}

void FLOTAWidget::onSendConfig()
{
	if (cFile == "")
	{
		onOpenCF();
	}


}

void FLOTAWidget::onUpdateZC()
{
	if (zFile == "")
	{
		onOpenZF();
	}

	if (zFile != "") {
		
		OTA_Address = "0x0000";

		if (OTA_Address != "")
		{
			binFileName = zFile;
			SendType = SEND_ZF;
			UpdateDevice();
		}
	}

}

void FLOTAWidget::onUpdateWIO()
{
	if (wFile == "")
	{
		onOpenWF();
	}

	if (wFile != "") {
		OTA_Address = FindNextDevice("");

		if (OTA_Address != "")
		{
			binFileName = wFile;
			SendType = SEND_WF;
			UpdateDevice();
		}
	}

}

void FLOTAWidget::prepareHeader(QByteArray *arr)
{
	char header[] = { 0x02, 0x9A, 0x33, 0x00, 0x02, 0x97, 0x8e, 0x09, 0x00, 0x06, 0x06, 0x00, 0x01 };


	bool bOk;

	auto value = OTA_Address;


	if (value.startsWith("0x", Qt::CaseInsensitive))
		value = value.right(value.length() - 2);

	int i = 5;

	while (value.length() >= 2)
	{
		QString right = value.right(2);
		int hex = right.toInt(&bOk, 16);

		header[i++] = hex;
		value = value.left(value.length() - 2);
	}


	arr->append(header, 13);
}

void FLOTAWidget::sendPacket(int type)
{
	QByteArray arr;
	bool send = false;
	int ms;

	prepareHeader(&arr);

	switch (type) {

	case OTA_IMAGE_NOTIFY:
		qDebug() << "Sending Image Notify" << packet_number;
		arr[7] = packIN(&arr);

		send = true;
		inputFileOffset = 0;
		break;

	case OTA_QUERY_RESPONSE:
		qDebug() << "Sending Query Response" << packet_number;
		arr[7] = packQR(&arr);

		send = true;
		break;

	case OTA_IMAGE_RESPONSE:
		ms = lapseTimer.elapsed();


		qDebug() << ms << "Sending IB  Response " << data_packet
			<< "Offset" << inputFileOffset;

		arr[7] = packIBR(&arr);

		send = true;
		break;

	case OTA_UPGRADE_END:
		arr[7] = packUER(&arr);
		qDebug() << "Sending Upgrade End Response" << packet_number;
		send = true;
		break;

	default:
		break;
	}

	if (send) {

		arr[3] = arr.length() - 4;

		arr.append(Checksum(arr, arr.length()));

		_ota_port->write(arr);

		packet_number++;
	}


}

void FLOTAWidget::nextPacket()
{
	if (bytetoRead > 0)
	{

		sendPacket(OTA_IMAGE_RESPONSE);

		_OTAtimer.start(20);

	}
	else
	{
		_OTAtimer.stop();

	
		input.close();
	}

}

int FLOTAWidget::packQR(QByteArray *arr) {
	static OTA_QueryResponse packet = {

		{ 0x40, 0x08, 0x0019, 0x0109, 0x08, 0xFC },					//APS header	
		{ 0x19, 0x81, 0x02 },										//ZCL header
		{ 0x00, 0x1004, 0x0000, 0x30103010, 0x00000000 }			//ZCL Payload

	};

	packet.zclHeader.transSN = packet_number;
	packet.apsHeader.apsCounter = packet_number;

	if (SendType == SEND_CONFIG)
		packet.zclPayload.ImageType = OTA_IT_CONFIG;
	else
		packet.zclPayload.ImageType = OTA_IT_MANSPC;

	packet.zclPayload.imageSize = bytetoRead;

	arr->append((char*)&packet, sizeof(packet));
	return sizeof(packet);
}

int FLOTAWidget::packIN(QByteArray *arr) {
	static OTA_ImageNotify packet = {

		{ 0x40, 0x08, 0x0019, 0x0109, 0x08, 0xFC },		//APS header	
		{ 0x19, 0x81, 0x00 },							//ZCL header
		{ 0x03, 0x64,  }								//ZCL Payload

	};

	packet.zclHeader.transSN = packet_number;
	packet.apsHeader.apsCounter = packet_number;


	packet.zclPayload.PayloadType = 0;
	packet.zclPayload.QueryJitter = 0x64;
	(*arr)[5] = 0xFF;
	(*arr)[6] = 0xFF;
	arr->append((char*)&packet, sizeof(OTA_ImageNotify));
	return sizeof(OTA_ImageNotify);
}

int FLOTAWidget::packUER(QByteArray *arr) {
	static OTA_UpgradeEndResponse packet = {

		{ 0x40, 0x08, 0x0019, 0x0109, 0x08, 0xFC }, //APS header
		{ 0x19, 0x81, 0x07 },
		{ 0x1004, 0x0000, 0x30103010, 0x00000000, 0x00000000 }

	};

	packet.zclHeader.transSN = packet_number;
	packet.apsHeader.apsCounter = packet_number;
	if (SendType == SEND_CONFIG)
		packet.zclPayload.imgType = OTA_IT_CONFIG;

	packet.zclPayload.curTime = 0;
	packet.zclPayload.ugTime = 0;

	arr->append((char*)&packet, sizeof(packet));
	return sizeof(packet);
}

int FLOTAWidget::packIBR(QByteArray *arr) {
	return 0;
}

void FLOTAWidget::onOTA_Timer()
{
	//excute_command("NLDE-DATA.Request 0x02 " + OTA_Address + " 0x02 0x00 0x06 0x06 0x00 0x01 0xA0 0x01");

	//nextPacket();

}


void FLOTAWidget::onOTAReadyRead()
{
	// read packet from serial port
	const int waitTimeout = 100;

	QByteArray readBytes = _ota_port->readAll();

	rcvBytes.append(readBytes);

	int len = rcvBytes.size();
	qDebug("\nreadBytes Len=%d rcvBytes(Total) Len=%d", readBytes.length(), len);

	while (ExtractPacket());

}
bool FLOTAWidget::ExtractPacket()
{
	const int ext = 2; // sync, fcs
	const int hdr = 3;
	int len = rcvBytes.size();

	for (int pos = 0; pos < len - hdr - ext; pos++)
	{
		unsigned char plen = rcvBytes[pos + 3];
		int flen = hdr + plen;
		unsigned char st = rcvBytes[pos];
		if (st == ZTC_STX && pos + flen + ext <= len)
		{
			unsigned char rcvFCS = rcvBytes[pos + flen + 1];
			unsigned char frmFCS = 0;
			for (int i = 0; i < flen; i++)
				frmFCS ^= rcvBytes[pos + 1 + i];
			if (rcvFCS == frmFCS)
			{
				QByteArray packet = rcvBytes.mid(pos, flen + ext);
				rcvBytes = rcvBytes.mid(pos + flen + ext);

				//qDebug("Packet(%d) extracted , remaining Len=%d", packet.size(), rcvBytes.size());
				if (packet[1] != char(0x84) && packet[2] != char(0x04))
					PrintArray(packet, "Packet  ");
				if (rcvBytes.length() > 0)
					PrintArray(rcvBytes, "rcvBytes");

				if (packet[1] == char(0x9B) && packet[2] == char(0x40))
				{
					//qDebug("Processing Data packet");
					ProcessData(packet);

				}
				else
					parse_rec_data(packet);
				return true;
			}
			else
				qDebug() << "BAD CRC";
		}
	}

	return false;
}


void FLOTAWidget::on_clear_cmd()
{
	ui.treeWidget_2->clear();
}


void FLOTAWidget::floorChanged(int index)
{
	

}

void FLOTAWidget::OpenJob()
{
	

}

void FLOTAWidget::on_radio_toggled(bool)
{
	//ui.lbFW->setText("");
	binFileName = "";
}


void FLOTAWidget::parse_rec_data(QByteArray arr)
{
	int par_len = arr[3];
	int head_count = 4;
	while (arr.length() >= par_len + head_count + ZTC_CRC_LENGH  && par_len >= 0)
	{
		if (ui.treeWidget_2->topLevelItemCount() > 100)
		{
			ui.treeWidget_2->clear();
		}
		QString str_comm;
		for (int n = 0; n< par_len + head_count + ZTC_CRC_LENGH; n++)
		{
			str_comm += QString("%1 ").arg((unsigned char)arr[n], 2, 16, QLatin1Char('0')).toUpper();
		}
		//qDebug() << "rec data "<< str_comm;		

		auto rec_list = str_comm.split(" ");
		if (rec_list.count() < 4)
			return;

		if (!(rec_list[1] == "84" && rec_list[2] == "04"))
		{
			//qDebug() << rec_list[1] << rec_list[2];
		}

		if (rec_list[1] == "A0" && rec_list[2] == "13")
		{
			qDebug() << "ZDP-DeviceAnnce.Indication";

		}
		else if (rec_list[1] == "9B" && rec_list[2] == "40")
		{
			//qDebug() << "NLDE-Data.Indication-1";
			//ProcessData(arr);
		}
		else if (rec_list[1] == "9B" && rec_list[2] == "3F")
		{
			int ms = lapseTimer.elapsed();			
			qDebug() << ms << "NLDE-Data.Confirmation";
		}

		bool is_confim_req = false;
		
		auto rec_comm_list = FLXMLCommandManager::GetZigBeeProCMD()->all_commmand();

		foreach(auto node, rec_comm_list)
		{
			auto cmd_head = node.firstChildElement("CmdHeader");
			auto str_head = cmd_head.text();
			auto head_list = str_head.split(" ");
			if (head_list.count() != 2)
			{
				continue;
			}
			else if (rec_list[1] == head_list[0] && rec_list[2] == head_list[1])
			{
				QTreeWidgetItem* item;
				if (!OTA_active) {
					item = new QTreeWidgetItem(ui.treeWidget_2);
					ui.treeWidget_2->addTopLevelItem(item);

					QLabel* top_label = new QLabel("RX: " + str_comm, this);
					top_label->setWordWrap(true);

					ui.treeWidget_2->setItemWidget(item, 0, top_label);


				}


				parse_cmd_content(item, node, rec_list);
				is_confim_req = true;
				break;
			}

		}
		if (is_confim_req == false)
		{
			data2frame(rec_list);
		}

		arr = arr.right(arr.length() - par_len - head_count - ZTC_CRC_LENGH);
		par_len = arr[3];
	}

}

void FLOTAWidget::parse_cmd_content(QTreeWidgetItem* item, QDomNode command, QStringList& cmd_list)
{
	//3 length;
	//int len = cmd_list[3];
	bool bOK;
	auto cmd_name = command.firstChildElement("CmdName").text();

	if (cmd_name == "ZDP-DeviceAnnce.Indication")
	{
		qDebug() << "Device anouncement";
		OTA_Address = "0x" + cmd_list[5] + cmd_list[4];
		//ui.pbWriteFW->setEnabled(true);
	
		_timer.start(1000);
	}
	else if (cmd_name == "NLDE-DATA.Indication")
	{
		//qDebug() << "NLDE-Data.Indication-2";
		//qDebug() << "Data";
		//ProcessData(cmd_list);
	}
	else if (cmd_name == "NLDE-DATA.Confirm")
	{
		//if(txContinue) 
		//sendPacket();
	}
	else if (cmd_name == "NLME-JOIN.Confirm")
	{
		ui.gbNetwork->setEnabled(true);
		ui.gbOTAStatus->setEnabled(true);
	
	}

	if (OTA_active)
		return;

	if (cmd_name != "MacBeaconNotify.Indication")
	{
		if (cmd_name != "")
			qDebug() << cmd_name;
		else
			qDebug() << "Unknown Command/Event" << cmd_list[1] << cmd_list[2];
	}
	item->setText(0, "");
	auto label = (QLabel*)ui.treeWidget_2->itemWidget(item, 0);
	label->setText("RX: " + cmd_name + label->text().right(label->text().length() - 3));

	//---parse received;
	auto rec_list = cmd_list;
	if (rec_list.count() < 4)
		return;

	//---->01 byte		
	if (rec_list[0] == "02")
		add_child_item(item, QStringList() << "Sync" << "[1 byte] " << "02");
	else
		add_child_item(item, QStringList() << "UnK" << "[1 byte] =" << rec_list[0]);

	//--->02 03byte	
	add_child_item(item, QStringList() << "OPG" << "[1 byte]" << rec_list[1]);
	add_child_item(item, QStringList() << "OPO" << "[1 byte]" << rec_list[2]);
	add_child_item(item, QStringList() << "LEN" << "[1 byte]" << rec_list[3]);

	auto cmd_param = command.firstChildElement("CmdParms");
	auto cmd_node = cmd_param.firstChild();
	//payload node;
	if (cmd_node.firstChildElement("ParmName").text() == "PayloadLength")
		cmd_node = cmd_node.nextSibling();
	int pos = 4;
	while (cmd_node.isNull() == false && pos < cmd_list.count())
	{
		auto par_name = cmd_node.firstChildElement("ParmName").text();
		auto par_len = cmd_node.firstChildElement("ParmSize").text().toInt();
		auto par_type = cmd_node.firstChildElement("ParmType").text();
		QString parameter;
		if (par_type == PARAMETER_TYPE_EDIT && pos + par_len < cmd_list.count())
		{
			for (int n = 0; n< par_len; n++)
			{
				parameter += cmd_list[pos] + " ";
				pos++;
			}
		}
		else if (par_type == PARAMETER_TYPE_ENUM)
		{
			auto enum_node = cmd_node.firstChildElement("ParmTypeSpecifics");
			auto enum_item = enum_node.firstChild();
			bool bfound = false;
			while (enum_item.isNull() == false)
			{
				if (enum_item.toElement().text().toInt(&bOK, 16) == cmd_list[pos].toInt(&bOK, 16))
				{
					bfound = true;
					parameter = enum_item.firstChildElement("ParmText").text();
					break;
				}
				enum_item = enum_item.nextSibling();
			}
			if (bfound == false)
				parameter = cmd_list[pos];
			pos++;
		}

		add_child_item(item, QStringList() << par_name << "[" + cmd_node.firstChildElement("ParmSize").text() + "BYTE]" << parameter);
		cmd_node = cmd_node.nextSibling();
	}



}
void FLOTAWidget::add_child_item(QTreeWidgetItem* item, QStringList &lt)
{
	QTreeWidgetItem* ch_item = new QTreeWidgetItem();
	set_item(ch_item, lt);
	item->addChild(ch_item);
}

void FLOTAWidget::data2frame(QStringList lt)
{
}

void FLOTAWidget::insertField(QString text, int i, int j)
{
	QTableWidgetItem *pCell = ui.tblWIO->item(i, j);

	if (!pCell)
	{
		pCell = new QTableWidgetItem;
		//pCell->setBackgroundColor(col);
		ui.tblWIO->setItem(i, j, pCell);
	}

	pCell->setText(text);
	if(text.contains("Upgrade"))
		pCell->setCheckState(Qt::Unchecked);
}
void FLOTAWidget::AddDevice(QByteArray arr)
{
	QString device = QString("%1%2").arg((unsigned char)arr[8], 2, 16, QLatin1Char('0')).arg((unsigned char)arr[7], 2, 16, QLatin1Char('0')).toUpper();


	int i = nList.indexOf(device);

	if (i == -1)
	{

		nList.append(device);
		i = nList.indexOf(device);
		
		ui.tblWIO->insertRow(ui.tblWIO->rowCount());
	}


	
	QString hardware = (char )arr[30] == 0 ? "ZC": "WIO";

	QString version = QString("V%1.%2.%3").arg((unsigned char)arr[29]).arg((unsigned char)arr[28]).arg((unsigned char)arr[26]);


	insertField("0x" + device, i, 0);
	
	insertField(hardware, i, 1);

	insertField(version, i, 2);

	if(hardware!="ZC")
		insertField("Upgrade?", i, 3);
	
	i++;

}
void FLOTAWidget::ProcessData(QByteArray arr)
{
	int packetNumber;
	//qDebug() << data;
	int ms;

	if (arr[10] == char(0x40)) {

		switch (arr[20]) {

		case COMMAND_QUERY_NEXT_IMAGE_REQ:
			//OTA_active = true;

			packetNumber = (unsigned char)arr[19];

			AddDevice(arr);

			break;
		case COMMAND_IMAGE_BLOCK_REQ:
			//OTA_active = true;
			packetNumber = (unsigned char)arr[19];

			ms = lapseTimer.elapsed();
			memcpy(&inputFileOffset, arr.constData() + 30, 4);

			//qDebug() << ms  << "Received IB Request #" << packetNumber<< "Offset" << inputFileOffset;
			//if (lastPacket != packetNumber) 
			{



				lastPacket = packetNumber;
				data_packet = packetNumber;
				nextPacket();
			}

			break;
		case COMMAND_UPGRADE_END_REQ:
			qDebug() << "Received Upgrade End Request #" << QString("%1").arg(int(arr[19]));
			inputFile->close();
			sendPacket(OTA_UPGRADE_END);

			OTA_active = false;
		
			ui.gbNetwork->setDisabled(false);
			if(SendType == SEND_WF)
			{
				binFileName = wFile;
				OTA_Address = FindNextDevice(OTA_Address);
				if (OTA_Address != "")
					onUpdateWIO();
			}
			break;
		default:
			break;

		}

	}


}



#pragma endregion