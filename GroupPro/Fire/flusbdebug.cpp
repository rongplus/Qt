#include "flusbdebug.h"

#include <string>
#include "qx_xmodem.h"
#include <QScrollBar>
#include <QFileDialog>
#include <QCheckBox>
#include <QDomDocument>

#include "flfloorview.h"
#include "flzcserialport.h"
#include "flzonecontrollor.h"
#include "zc.pb.h"
#include "fljobprotol.h"
#include <QTime>
#include <QSettings>
using namespace std;
FLZigbeeTest::FLZigbeeTest(QWidget *parent) : QWidget(parent)
{		
	ui.setupUi(this);
	m_serial = nullptr;
	InitPort();
	LoadMod();
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
	connect(ui.pbFresh, SIGNAL(clicked()), this, SLOT(refreshCommPorts()));
	connect(ui.pbCommand, SIGNAL(clicked()), this, SLOT(on_command()));

	connect(ui.pb_clear, SIGNAL(clicked()), this, SLOT(on_clear()));

	connect(ui.comboBox, SIGNAL(currentTextChanged(const QString & )), this, SLOT(commandChanged(const QString & )));
	connect(ui.serialPortInfoListBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));
	connect(ui.tabMod, SIGNAL(cellChanged(int, int)), this, SLOT(modChanged(int, int)));

	ready_send_file = false;
	ui.pbSendFile->setEnabled(ready_send_file);

	qApp->installEventFilter(this);

	ready_send_file = false;
	
	auto file_path = QApplication::applicationDirPath() + "/zigbee/";
	file_path += "ZigBeePro.xml";
	//xml_cmd.loadxml(file_path);
	FLXMLCommandManager* p = FLXMLCommandManager::GetZigBeeProCMD();
	xml_cmd = p;
	ui.widget->setMoveable(false);
	ui.label_8->setEnabled(false);	

	connect(ui.widget_3, &Switcher::toggled, this, &FLZigbeeTest::onLog);
	saveCapture = false;

	QButtonGroup* group = new QButtonGroup(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_command()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_command()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(on_command()));
	connect(ui.pushButton_4, SIGNAL(clicked()), this, SLOT(on_command()));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(on_command()));
	connect(ui.pushButton_6, SIGNAL(clicked()), this, SLOT(on_command()));
	connect(ui.pushButton_7, SIGNAL(clicked()), this, SLOT(on_command()));
	connect(ui.pushButton_8, SIGNAL(clicked()), this, SLOT(on_command()));
	connect(ui.pushButton_9, SIGNAL(clicked()), this, SLOT(on_command()));
	connect(ui.pushButton_10, SIGNAL(clicked()), this, SLOT(on_command()));

	connect(ui.pushButton_12, SIGNAL(clicked()), this, SLOT(on_read()));
	connect(ui.pushButton_13, SIGNAL(clicked()), this, SLOT(on_write()));	
	connect(ui.pushButton_14, SIGNAL(clicked()), this, SLOT(on_login()));

	connect(ui.lineEdit_4, SIGNAL(textChanged(const QString &)), this, SLOT(on_textChanged(const QString &)));

	_portIO = nullptr;
	_portIO = new FLSerialPortIO(this);
	_zcport = new FLZCSerialPort(this);
	auto_login = false;

	connect(_portIO, SIGNAL(state_change(int)), this, SLOT(on_io_state_changed(int)));
	_portIO->start();
	ui.tabWidget->setEnabled(false);
}

void FLZigbeeTest::modChanged(int row, int column)
{	
	QString textFromItem = ui.tabMod->item(row, column)->text();
	if (column == 0)
		mods[row].org = textFromItem;
	else if (column == 1)
		mods[row].mod = textFromItem;

}
FLZigbeeTest::~FLZigbeeTest()
{
	if (_portIO)
	{
		_portIO->set_stop();
		_portIO->quit();
		if (_portIO->isRunning())
			_portIO->wait();
		delete _portIO;
		_portIO = nullptr;
		
	}

	if (m_serial)
	{
		delete m_serial;
	}
		

}

void FLZigbeeTest::set_port(QSerialPort * port)
{
	Q_ASSERT(port);
	m_serial = port;
	disconnect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
	connect(m_serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	m_serial->write("\r");
	ui.pbConnect->setText("Disconnect");
	ui.comboBox->setCurrentText( port->portName());
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(on_disconnect()));
}

void FLZigbeeTest::set_floor(FLFloor * floor)
{
	ui.widget->setFloor(floor);

}

void FLZigbeeTest::set_zc(FLZoneControllor * zc)
{
	ui.widget->setFloor((FLFloor*)zc->parent());
	ui.widget->set_current_object(zc);
}

void FLZigbeeTest::LoadMod(void)
{

	auto file_path = QApplication::applicationDirPath() + "/mods/";
	file_path += "usermods.xml";

	QFile xml_file(file_path);
	QDomDocument dom;
	if (!xml_file.open(QIODevice::ReadOnly))
		return ;
	//QString manifest = xml_file.readAll();

	if (!dom.setContent(&xml_file)) {
		xml_file.close();
		return;
	}

	xml_file.close();

	//dom.setContent(manifest);
	if (dom.isNull() || (!dom.isDocument()))
	{
		qDebug() << "QxPluginServer::parseManifest: Failed to parse the manifest file!";		
	}
	
	
	QDomElement docElem = dom.documentElement();//UserMods

	auto node = docElem.firstChild();
	
	while(node.isNull() == false)
	{
		QDomElement e = node.toElement(); // try to convert the node to an element.
		if(!e.isNull()) {
			qDebug() << qPrintable(e.tagName()) ; // the node really is an element.
			QString str = e.tagName();
			if( str == "Mod")				
			{
				int index = ui.tabMod->rowCount();
				ui.tabMod->insertRow(index);

				Mod m;

				auto org = node.firstChild().toElement();
				auto mod = node.firstChild().nextSibling().toElement();
				auto col = node.firstChild().nextSibling().nextSibling().toElement();

				m.org = org.text();

				QTableWidgetItem *item=new QTableWidgetItem(org.text());
				ui.tabMod->setItem(index,0,item);

				m.mod = mod.text();
				QTableWidgetItem *item2=new QTableWidgetItem(mod.text());
				ui.tabMod->setItem(index,1,item2);

				QCheckBox *cb = new QCheckBox();
				//cb->setStyle(new MyStyle());
				ui.tabMod->setCellWidget(index, 2, cb);

				m.col = col.text();
				item2->setTextColor(QColor(col.text()));

				mods.append(m);
				//ui.tabMod->item(index, 2)->setTextColor();
			}
		}
		node = node.nextSibling();
	}

	ui.tabMod->resizeRowsToContents();
	ui.tabMod->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); 
}

bool FLZigbeeTest::eventFilter(QObject *obj, QEvent *event)
{    
    return QObject::eventFilter(obj, event);
}

void FLZigbeeTest::InitPort()
{
	QString description;
	QString manufacturer;
	QString serialNumber;
	QString blankString = "No description";
	const auto infos = QSerialPortInfo::availablePorts();
	foreach(const QSerialPortInfo &info, infos)
	{
		if (info.isValid() == false)
			continue;
		QStringList list;
		description = info.description();
		manufacturer = info.manufacturer();
		serialNumber = info.serialNumber();
		list << info.portName()
			<< (!description.isEmpty() ? description : blankString)
			<< (!manufacturer.isEmpty() ? manufacturer : blankString)
			<< (!serialNumber.isEmpty() ? serialNumber : blankString)
			<< info.systemLocation()
			<< (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
			<< (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

		ui.serialPortInfoListBox->addItem(list.first() );
	}

	ui.serialPortInfoListBox->addItem(tr("Custom"));
}

#pragma region a

void FLZigbeeTest::on_clear()
{
	ui.terminal->clear();
}
void FLZigbeeTest::onConnect() 
{
	auto currentPortName = ui.serialPortInfoListBox->currentText();// ui.serialPortInfoListBox->itemData(ui.serialPortInfoListBox->currentIndex()).toString();;
	auto serial = new QSerialPort(currentPortName,this);

	if (!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Error"), serial->errorString());
		delete serial;				
		return;
	}

	m_serial = serial;
	disconnect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
	connect(m_serial,SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	m_serial->write("\r");
	ui.pbConnect->setText("Disconnect");

	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(on_disconnect()));

	ui.tabWidget->setEnabled(true);
}


void FLZigbeeTest::onSaveDir()
{
	QSettings st("Mircom", "FireLink");
	QString str = st.value("Default_TXT_Path").toString();

	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	//dialog.selectFile(text);
	QString str_file_name = dialog.getExistingDirectory(this, "Set PCAP Dir", str);
	if (str_file_name.isNull()) 
	{
		return;
	}

	st.setValue("Default_TXT_Path", str_file_name);
	//ui.btnDir->setToolTip(QString("Current Dir: %1\r\nClick to Change").arg(str_file_name));

	m_dirPath = str_file_name;
}

void FLZigbeeTest::onLog(bool enable)
{
	if (enable)
	{
		if (m_dirPath == "")
		{
			onSaveDir();
		}

		QString text = QDate::currentDate().toString("yyyy-MM-dd") + "-" + QTime::currentTime().toString().replace(":", ".");
		qDebug() << text;

		ui.saveFileName->setText("File Name: \r\n/" + text + ".txt");

		setSaveCapture(true, m_dirPath + "/" + text + ".txt");

	}
	else
	{
		ui.saveFileName->setText("");
		setSaveCapture(false, "");
	}
}

void FLZigbeeTest::setSaveCapture(bool enable, QString filename)
{
	saveCapture = enable;
	if (saveCapture)
	{
		saveFile = new QFile(filename);
		saveFile->open(QIODevice::WriteOnly);
		

	}
	else
	{
		if (saveFile != nullptr)saveFile->close();
	}
}
void FLZigbeeTest::onReadyRead()
{

	QByteArray arr = m_serial->readAll();

	if (saveCapture)
	{
		
		saveFile->write(arr);
		saveFile->flush();

	}

	if(arr[0] =='C')
	{
		if (ready_send_file == true)
		{
			on_send_macs1();
		}
		ui.pbSendFile->setEnabled(true);
	}

	QString s_data = QString(arr.data());
	s_data = ProcessFilter(s_data);
	//ui.terminal->insertPlainText(arr);
	ui.terminal->insertHtml(s_data);
	ui.terminal->insertHtml("\r\n");
	while (!m_serial->atEnd()) {
		QByteArray data = m_serial->read(100);
		QString s_data = QString(data.data());
		s_data = ProcessFilter(s_data);
		//ui.terminal->insertPlainText(data);
		ui.terminal->insertHtml(s_data);
		ui.terminal->insertHtml("\r\n");
	}

	ui.terminal->verticalScrollBar()->setValue(ui.terminal->verticalScrollBar()->maximum());

}

#pragma endregion

QString FLZigbeeTest::ProcessFilter(QString in)
{
	//qDebug() << "ProcessFilter------------------";
	//qDebug() << "in string = " << in;
	for (int i = 0; i< mods.size(); i++)
	{
		//QWidget *pWidget = ui.tabMod->cellWidget(i, 2);
		QCheckBox *checkbox = (QCheckBox *)ui.tabMod->cellWidget(i, 2);
		if (checkbox && checkbox->isChecked()) {
			if (mods[i].mod.startsWith("ProcessMessage"))
			{
				QRegularExpression re(mods[i].org);

				QRegularExpressionMatchIterator it = re.globalMatch(in);
				while (it.hasNext())
				{
					QRegularExpressionMatch match = it.next();
					// ...


					if (match.hasMatch()) {
						
						ProcessMessage(mods[i].mod, match);
					}
				}

			}

			if (mods[i].mod == "CmdLookup")
			{
				QRegularExpression re(mods[i].org);

				QRegularExpressionMatchIterator it = re.globalMatch(in);
				while (it.hasNext())
				{
					QRegularExpressionMatch match = it.next();
					// ...


					if (match.hasMatch()) {
						QString matched = match.captured(1).trimmed(); // matched == "23 def"
																	   //in.replace(QRegularExpression(mods[i].org), "<font color=\"" + mods[i].col + "\">" + CmdLookup(matched) + "</font>");									 // ...
						in.replace(matched, "<font color=\"" + mods[i].col + "\">" + CmdLookup(matched) + "</font>");									 // ...
					}
				}

			}
			else
				in.replace(QRegularExpression(mods[i].org), "<font color=\"" + mods[i].col + "\">" + mods[i].mod + "</font>");
		}
		//in.replace(QRegularExpression("Rx code (.*)\r"), "CODE \\1\r");
	}


	in.replace("\\r", "<br>").replace("\r", "<br>");
	in.replace("TIME", "[" + QTime::currentTime().toString() + "]");
	return in;
}

QString FLZigbeeTest::CmdLookup(QString cmd)
{
	auto header = cmd;
	QString res = cmd;
	QDomNode node = xml_cmd->find_node_byHeader(header);
	qDebug() << "Mheader = " << header;
	if (node.isNull() == false)
	{
		auto name = node.firstChildElement("CmdName").text();
		res = name;
		qDebug() << "node name" << name;
	}
	QString in = res;
	in.replace("\\r", "<br>").replace("\r", "<br>");
	return in;
}

void FLZigbeeTest::refreshCommPorts() 
{		
 	ui.serialPortInfoListBox->clear();
	InitPort();
}

void FLZigbeeTest::on_toggle_trace()
{
	char t= 0x14;
	string str_comm;
	str_comm.append(&t);
	QByteArray arr;
	arr.append(t);
	arr.append('\r');
	str_comm +="\r";
	if(m_serial != nullptr)m_serial->write(arr);
}

void FLZigbeeTest::on_get_job()
{
	if(m_serial != nullptr)m_serial->write("J\r");
}

void FLZigbeeTest::on_get_firmware()
{
	if(m_serial != nullptr)m_serial->write("V\r");
}


void FLZigbeeTest::on_command()
{
	if (m_serial == nullptr)
		return;
	m_serial->disconnect();
	connect(m_serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	QString comm= "";
	// e.g. check with member variable _foobarButton
	QObject* obj = sender();
	if (obj == ui.pbCommand)
	{
		comm = ui.comboBox->currentText();
	}
	else
	{
		// e.g. casting to the class you know its connected with
		QPushButton* button = qobject_cast<QPushButton*>(sender());
		if (button != NULL)
		{
			comm = button->text().toLower();
		}
	}
	ui.listWidget->addItem(comm);
	if (comm == "trace on/off")
	{
		on_toggle_trace();
		return;
	}
	else if (comm == "login")
	{
		if (ui.lineEdit->text().isEmpty())
		{
			QMessageBox::warning(this, "Please input password.", "Password can not be null.");
			return;
		}
		m_serial->disconnect();
		FLZCSerialPort zc_port;
		disconnect(m_serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		auto suc = zc_port.login(m_serial, ui.lineEdit->text());
		
		if (suc == true)
			ui.terminal->insertPlainText("Login Success\r");
		else
			ui.terminal->insertPlainText("Login failed\r");
		connect(m_serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
		return;
	}
	else if (comm == "send macs")
	{
		auto mac_file = ui.label_7->text();
		if (mac_file.isNull())
		{
			QMessageBox::warning(this, "Please select a file", "Noe file selected");
			return;
		}
		QFile fi_mac(mac_file);
		if (fi_mac.open(QIODevice::ReadOnly) == false)
		{
			QMessageBox::warning(this, "Please select a file", "File could not be opened.");
			return;
		}

		if (m_serial != nullptr)
			m_serial->write("MAC\r");
		return;
	}
	else if (comm == "list")
	{
		m_serial->write("list\r");
	}
	//string str_comm = comm.toStdString()+"\r";
	QByteArray out_arr;
	out_arr.append(comm);
	out_arr.append('\r');
	if(m_serial != nullptr)m_serial->write(out_arr);
	//m_serial->write("\r\n");
}

void FLZigbeeTest::on_send_macs()
{
	auto mac_file = QFileDialog::getOpenFileName();
	if (mac_file.isNull())
		return;
	ui.label_7->setText(mac_file);
}

void FLZigbeeTest::on_send_macs1()
{
	ready_send_file = false;
	ui.pbSendFile->setEnabled(ready_send_file);
	auto mac_file = ui.label_7->text();
	if(mac_file.isNull())
		return;
	QFile fi_mac( mac_file);
	if(fi_mac.open(QIODevice::ReadOnly) == false)
		return;
	QTextStream mac_stream(&fi_mac);
	auto mac_content = mac_stream.readAll();

	QByteArray arr(mac_content.toLocal8Bit());
	unsigned int bufsize = 1024;		
	//LOG(INFO) << "XMODEM::xTransmit - size " << ba.size();
	if (arr.size()%bufsize != 0)
	{
		// Space padding in the buffer		
		const char ctrlz[2] = {CTRLZ,0x00};
		QString pad(ctrlz);
		pad = pad.repeated((bufsize-(arr.size()%bufsize)));
		arr.append(pad.toUtf8());		
		qDebug() << "XMODEM::xTransmit - size with padding" << arr.size();
	}	
	quint8 seqnum = 1;
	XMODEM m(m_serial,this);
	QByteArray packet;
	packet.append(QString(STX).toUtf8());
	packet.append(seqnum);
	packet.append(~seqnum);
	packet.append(arr);
	//packet.append("\r");
	unsigned short ccrc = m.crc16_ccitt((unsigned char*)arr.constData(),bufsize);
	packet.append((unsigned short)(ccrc>>8) & 0xFF);
	packet.append((unsigned short)ccrc & 0xFF);
	//packet.append("\r");
	qDebug() << "packet size= " << packet.size();
	m_serial->write(packet);
	//m_serial->waitForBytesWritten(5000);
	QMessageBox::warning(this, "Finished!", "MACs file was sent to terminal");
	//m_serial->write("\r");
}

void FLZigbeeTest::on_send_enter()
{
	m_serial->write("\r");
}

void FLZigbeeTest::closeEvent(QCloseEvent *)
{
	if(m_serial)
	{
		m_serial->setProperty("Used",false);
		disconnect(m_serial,SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	}
}

void FLZigbeeTest::on_port_changed(int index)
{	
	return;
}

void FLZigbeeTest::showEvent(QShowEvent *event)
{	
	QWidget::showEvent(event);
	if (ui.widget->Floor())
		return;
	FLFloor * floor = new FLFloor(0);
	FLZoneControllor* zc = new FLZoneControllor(0);
	zc->Init();
	zc->InitDefaultIO();
	floor->addZC(zc);
	ui.widget->setFloor(floor);

	
	qDebug() << "FLZigbeeTest view show event";
}

void FLZigbeeTest::on_disconnect()
{
	if(m_serial)
	{
		m_serial->close();
		delete m_serial;
		m_serial = nullptr;
	}
	ui.pbConnect->setText("Connect");

	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
	disconnect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(on_disconnect()));
	ui.tabWidget->setEnabled(false);
	
}

void FLZigbeeTest::commandChanged(const QString &strCommand)
{
	//ui.label_7->setEnabled(false);
	ui.pbSendFile->setEnabled(false);
	ui.lineEdit->setEnabled(false);
	if (strCommand == "login")
	{
		ui.lineEdit->setEnabled(true);
		ui.lineEdit->setInputMask("");
		ui.lineEdit->setEchoMode(QLineEdit::Password);
		ui.label_8->setText("Password");
	}
	else if (strCommand == "get status" || strCommand == "buzzer on/of")
	{
		ui.lineEdit->setEnabled(true);
		ui.lineEdit->clearMask();
		ui.lineEdit->setInputMask("NN:NN:NN:NN:NN:NN:NN:NN");
		ui.lineEdit->setText("FF:FF:FF:FF:FF:FF:FF:FF");
		ui.lineEdit->setEchoMode(QLineEdit::Normal);
		ui.label_8->setText("Content");
	}
	else if (strCommand == "Send MACs")
	{
		ui.pbSendFile->setEnabled(true);
	}
	
}

void FLZigbeeTest::showPortInfo(int idx)
{
	if (idx <0)
		return;
	QString blankString = "";
	const auto infos = QSerialPortInfo::availablePorts();

	if (idx >= infos.count())
		return;
	const QSerialPortInfo &info = infos[idx];
	QStringList list;

	QString description = info.description();
	QString manufacturer = info.manufacturer();
	QString serialNumber = info.serialNumber();

	list << info.portName()
		<< (!description.isEmpty() ? description : blankString)
		<< (!manufacturer.isEmpty() ? manufacturer : blankString)
		<< (!serialNumber.isEmpty() ? serialNumber : blankString)
		<< info.systemLocation()
		<< (info.vendorIdentifier() ? QString::number(info.vendorIdentifier(), 16) : blankString)
		<< (info.productIdentifier() ? QString::number(info.productIdentifier(), 16) : blankString);

	ui.descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : blankString));
	ui.manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : blankString));
	ui.serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : blankString));
	ui.locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : blankString));
	ui.vendorIDLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : blankString));
	ui.productIDLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : blankString));
}

void FLZigbeeTest::ProcessMessage(QString mod, QRegularExpressionMatch match) {

	
	QList<QString> cmdList = mod.split(" ");
	
	//str.toInt()

	if (cmdList[1] == "ACK") {
		QString str = match.captured(1).trimmed();
		qDebug() << "ProcessMessage" << str;
		bool ok;
		int r = str.toInt(&ok, 16);
		if (routers[r].valid) {
			ui.widget->activeDevice(QString("%1").arg(routers[r].MacAddr), 10, QString("0x%1").arg(routers[r].ShrtAddr));
		}
			
	}
	else if (cmdList[1] == "RouterEntry") {
		int Router = match.captured(1).trimmed().toInt();

		QString shrtAddr = match.captured(2).trimmed();
		QString macAddr = match.captured(3).trimmed().replace(" ",":");

		if (Router < 50) 
		{
			routers[Router] = { Router, shrtAddr, macAddr, true };
		}

		ui.widget->activeDevice(QString("%1").arg(macAddr), 10);
	}
	
}

void FLZigbeeTest::on_read()
{
	m_serial->disconnect();
	ui.tabWidget->setEnabled(false);
	auto_login = ui.checkBox_3->isChecked();
	if (auto_login)
	{
		_read_oprate = true;
		auto_login = false;
		write_time_after_login = false;
		ui.pushButton->setEnabled(false);
		ui.pushButton_4->setEnabled(false);
		on_login();
	}
	else
	{
		read();
		ui.tabWidget->setEnabled(true);
	}
}

void FLZigbeeTest::on_write()
{
	m_serial->disconnect();

	_read_oprate = false;
	auto_login = ui.checkBox_3->isChecked();
	ui.tabWidget->setEnabled(false);
	if (auto_login)
	{
		auto_login = false;
		ui.pushButton->setEnabled(false);
		ui.pushButton_4->setEnabled(false);
		on_login();
	}
	else
	{
		write();
		ui.tabWidget->setEnabled(true);
	}
}

void FLZigbeeTest::on_login()
{
	m_serial->disconnect();
	ui.tabWidget->setEnabled(false);
	if (ui.lineEdit_4->text().length() < 4)
	{
		QMessageBox::warning(this, "Password too short", "The password should be more than 4 characters");
		return;
	}
	Q_ASSERT(m_serial);
	ui.listWidget->addItem("Login ...");
	QByteArray arr;
	arr.append(ui.lineEdit_4->text());

	_portIO->send_data("login", QStringList() << "Password" << "Success", arr, m_serial);

}

void FLZigbeeTest::read()
{
	int index = ui.tabWidget_2->currentIndex();
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

void FLZigbeeTest::write()
{
	int index = ui.tabWidget_2->currentIndex();
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

bool FLZigbeeTest::read_cmd(QString cmd, QString filename)
{
	ui.listWidget->addItem(" start read  " + cmd);
	// wait ok;
	QByteArray arr = "get ";
	arr.append(cmd);
	//_portIO->set_filename(filename);
	//_portIO->send_data("get " + cmd,QStringList() <<"OK",arr, _port );

#if 1
	auto ret = _zcport->xmodemReceive(m_serial, arr, 1, filename);
	if (ret == 0)
	{
		ui.listWidget->addItem(cmd + " read ...successful");
		return true;
	}
	else
		ui.listWidget->addItem(cmd + " read ...failed");
#endif;

	return false;
}

void FLZigbeeTest::read_time()
{
	ui.listWidget->addItem("read time...");
	QString filename = "rtc_save.rtc";
	if (read_cmd("time", filename))
	{

		CFG_RTC rtc = FLJobProtol::RTCFromFile(filename);
		if (rtc.IsInitialized() == false)
		{
			return;
		}

		QTime tm;
		tm.setHMS(rtc.hours(), rtc.minutes(), rtc.seconds());
		ui.dateTimeEdit->setTime(tm);
		//QDateTime dt;
		QDate dd;
		dd.setDate(rtc.year(), rtc.month(), rtc.date());
		//dt.setDate(dd);
		ui.dateTimeEdit->setDate(dd);
	}
}

void FLZigbeeTest::read_daylightsaving()
{
	QString filename = "dst_save.dst";
	if (read_cmd("dst", filename))
	{
	}
}

void FLZigbeeTest::read_passcode()
{
	QString filename = "passcode_save.pwd";
	if (read_cmd("passcode", filename))
	{
	}
}

void FLZigbeeTest::read_firmware()
{
	QString filename = "firmware_save.fm";
	if (read_cmd("firmware", filename))
	{
	}
}

void FLZigbeeTest::read_configuration()
{
	QString filename = "config_save.cfg";
	if (read_cmd("config", filename))
	{
	}
}

void FLZigbeeTest::write_time()
{
	ui.listWidget->addItem(" start write time ");

	auto t1 = ui.dateTimeEdit->dateTime();
	if (ui.checkBox->isChecked())
	{
		t1 = QDateTime::currentDateTime();
	}
	CFG_RTC rtc;
	rtc.set_seconds(t1.time().second());
	rtc.set_minutes(t1.time().minute());
	rtc.set_hours(t1.time().hour());
	rtc.set_date(t1.date().day());
	rtc.set_month(t1.date().month());
	rtc.set_year(t1.date().year());
	char* buff = new char[rtc.ByteSize()];

	rtc.SerializePartialToArray(buff, rtc.ByteSize());
	//save to file;
	QString fileName = "rtc1.rtc";
	std::filebuf fb;
	fb.open(fileName.toStdString(), std::ios::out | std::ios::binary);
	std::ostream os(&fb);
	os.write(buff, rtc.ByteSize());
	fb.close();
	QByteArray arr(buff, rtc.ByteSize());


#if  1
	if (_zcport->send_binary_1K("send time", arr, m_serial) != FLZCSerialPort::Write_OutTime)
		ui.listWidget->addItem("Set time..successful.");
	else
		ui.listWidget->addItem("Set time..failed.");
#endif

	ui.tabWidget->setEnabled(true);
}

void FLZigbeeTest::write_daylightsaving()
{
	CFG_DST dst;

	QTime t1 = ui.timeEdit->time();
	dst.set_startdayofweek(ui.comboBox_3->currentIndex());
	dst.set_starthour(t1.hour());
	dst.set_startmin(t1.minute());
	dst.set_startmonth(ui.comboBox_5->currentIndex());
	dst.set_startordinalweek(ui.comboBox->currentIndex());

	t1 = ui.timeEdit_3->time();
	dst.set_enddayofweek(ui.comboBox_4->currentIndex());
	dst.set_endhour(t1.hour());
	dst.set_endmin(t1.minute());
	dst.set_endmonth(ui.comboBox_6->currentIndex());
	dst.set_endordinalweek(ui.comboBox_2->currentIndex());

	dst.set_dailyadjust(9);

	char* buff = new char[sizeof(CFG_DST)];


	dst.SerializePartialToArray(buff, sizeof(CFG_DST));

	QByteArray arr(buff, sizeof(CFG_DST));
#if  1
	if (_zcport->send_binary_1K("send dst", arr, m_serial) != FLZCSerialPort::Write_OutTime)
		ui.listWidget->addItem("Set dst..successful.");
	else
		ui.listWidget->addItem("Set dst..failed.");
#endif

}

void FLZigbeeTest::write_passcode()
{
	PASSWORDS pcs;
	PASSWORD *pass = pcs.add_password();
	pass->set_len(4);
	pass->set_data(ui.lineEdit->text().toStdString());

	pass = pcs.add_password();
	pass->set_len(4);
	pass->set_data(ui.lineEdit_2->text().toStdString());

	pass = pcs.add_password();
	pass->set_len(4);
	pass->set_data(ui.lineEdit_3->text().toStdString());

	//pcs.SerializePartialToCodedStream();
	pcs.ByteSize();

	char* buff = new char[pcs.ByteSize()];


	pcs.SerializePartialToArray(buff, pcs.ByteSize());

	QByteArray arr(buff, pcs.ByteSize());
#if  1
	if (_zcport->send_binary_1K("send passcodes", arr, m_serial) != FLZCSerialPort::Write_OutTime)
		ui.listWidget->addItem("Set passcodes..successful.");
	else
		ui.listWidget->addItem("Set passcodes..failed.");
#endif
}

void FLZigbeeTest::write_firmware()
{
	FLZoneControllor* _zc;
	Q_ASSERT(_zc);
	auto zone_control = _zc;
	ZC *zc = new ZC();

	//ui.label_login->setText("Send data...");
	delete zc;
	//delete[] buffer;
}

void FLZigbeeTest::write_configuration()
{
	QByteArray data;
	FLZCSerialPort::TransitState ret = _zcport->send_config(m_serial, data, 80);
}



void FLZigbeeTest::on_io_state_changed(int state)
{
	if (FLSerialPortIO::Login_Success == state)
	{
		//ui.label_14->setText( " Login Successful");
		ui.listWidget->addItem(" Login Successful");
		ui.progressBar->setValue(100);
		if (read_time_after_login)
		{
			read_time_after_login = false;
			ui.label_14->setText(" Read time...");
			ui.listWidget->addItem(" Start read time ");
			//read_cmd("time", "tmp_time.rtc");
#if 1
			if (read_cmd("time", "tmp_time.rtc"))
			{
				ui.listWidget->addItem(" Read time Successful");
				CFG_RTC rtc = FLJobProtol::RTCFromFile("tmp_time.rtc");
				if (rtc.IsInitialized())
				{
					ui.listWidget->addItem(" Parse time Successful");
					QTime tm;
					tm.setHMS(rtc.hours(), rtc.minutes(), rtc.seconds());
					ui.dateTimeEdit->setTime(tm);
					//QDateTime dt;
					QDate dd;
					dd.setDate(rtc.year(), rtc.month(), rtc.date());
					//dt.setDate(dd);
					ui.dateTimeEdit->setDate(dd);


					QTime cur_tm = QTime::currentTime();
					QDate cur_dt = QDate::currentDate();
					int sec = cur_tm.secsTo(tm);
					if (cur_dt != dd || abs(cur_tm.secsTo(tm)) >= 5)
					{
						auto ret = QMessageBox::question(this, "Time not match", "The time in the controller is not the same as the computer\n do you want to update time?", QMessageBox::Ok | QMessageBox::Cancel);
						if (ret != QMessageBox::Ok)
						{
							ui.tabWidget->setEnabled(true);
							return;
						}
						write_time_after_login = true;
						on_login();
						return;
					}
				}
				else
					ui.listWidget->addItem(" Parse time failed");

			}
#endif
		}
		if (write_time_after_login)
		{
			ui.label_14->setText(" Write time...");
			ui.listWidget->addItem(" Start write time");
			write_time_after_login = false;
			write_time();
			
		}

		if (auto_login)
		{
			if (_read_oprate)
				read();
			else
				write();
			auto_login = false;
		}

	}
	else if (state == FLSerialPortIO::Read_Time_Success)
	{
		ui.label_14->setText(" Read Time Successful");
		ui.listWidget->addItem(" Read time Successful");
		CFG_RTC rtc = FLJobProtol::RTCFromFile("tmp_time.rtc");
		if (rtc.IsInitialized())
		{
			ui.listWidget->addItem(" Parse time Successful");
			QTime tm;
			tm.setHMS(rtc.hours(), rtc.minutes(), rtc.seconds());
			ui.dateTimeEdit->setTime(tm);
			//QDateTime dt;
			QDate dd;
			dd.setDate(rtc.year(), rtc.month(), rtc.date());
			//dt.setDate(dd);
			ui.dateTimeEdit->setDate(dd);


			QTime cur_tm = QTime::currentTime();
			QDate cur_dt = QDate::currentDate();
			int sec = cur_tm.secsTo(tm);
			if (cur_dt != dd || abs(cur_tm.secsTo(tm)) >= 5)
			{
				auto ret = QMessageBox::question(this, "Time not match", "The time in the controller is not the same as the computer\n do you want to update time?", QMessageBox::Ok | QMessageBox::Cancel);
				if (ret != QMessageBox::Ok)
					return;
				write_time_after_login = true;
				on_login();
			}

		}
		else
			ui.listWidget->addItem(" Parse time failed Successful");
		ui.tabWidget->setEnabled(true);
	}

	else if (state < 0)
	{
		ui.listWidget->addItem(_portIO->command_name() + " Failed");
	
		ui.tabWidget->setEnabled(true);
	}
	else if (state == 100)
	{
		ui.listWidget->addItem(_portIO->command_name() + " Success");
		ui.tabWidget->setEnabled(true);
	}
	else
	{
		ui.progressBar->setValue((state) * 5);		
	}

	ui.pushButton->setEnabled(true);
	ui.pushButton_4->setEnabled(true);

}

void FLZigbeeTest::on_textChanged(const QString &text)
{
	ui.pushButton_12->setEnabled(text.size() >= 4);
	ui.pushButton_13->setEnabled(text.size() >= 4);
	ui.pushButton_14->setEnabled(text.size() >= 4);
}
