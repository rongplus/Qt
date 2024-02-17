#include "flzigbeeterminal.h"
#include <QDomDocument>
#include <QDir>
#include <QDebug>
#include <QCheckBox>
#include <QLineEdit>
#include <QSplitter>
#include <stdint.h>
#include "flobject.h"
#include <QSpacerItem>
#include <QTime>
#include "global.h"
#include <QMenu>
#include <QFileDialog>
#include <qthread.h>
#include <QSettings>
#include <QRegExp>




#include "fldlgchooseserialport.h"
#include "fljobprotol.h"

#include "ledindicator.h"


const QString PARAMETER_TYPE_EDIT = "tEdit";
const QString PARAMETER_TYPE_ENUM = "tEnum";

FLZigbeeMonitor::FLZigbeeMonitor(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	m_serial = nullptr;

	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
	connect(ui.pbFresh, SIGNAL(clicked()), this, SLOT(refreshCommPorts()));

	connect(ui.treeWidget,SIGNAL( itemClicked(QTreeWidgetItem * , int ) ), this, SLOT(on_itemClicked(QTreeWidgetItem * , int )));
	connect(ui.treeWidget,SIGNAL( itemDoubleClicked(QTreeWidgetItem * , int ) ), this, SLOT(on_itemDoubleClicked(QTreeWidgetItem * , int )));
	connect(ui.pb_reset, SIGNAL(clicked()), this, SLOT(on_reset_command()));
	connect(ui.serialPortBox,SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));

	connect(ui.tree_comm_mostused, SIGNAL( itemClicked(QTreeWidgetItem * , int ) ), this, SLOT(on_itemClicked(QTreeWidgetItem * , int )));

	connect(ui.tree_comm_mostused, SIGNAL( itemDoubleClicked(QTreeWidgetItem * , int ) ), this, SLOT(on_mostusedtree_DoubleClicked(QTreeWidgetItem * , int )));
	connect(ui.tree_comm_mostused, SIGNAL(customContextMenuRequested(const QPoint &)),this, SLOT(on_mostusedtree_Menu(const QPoint &)));

	connect(ui.le_find,SIGNAL(textChanged(const QString & )), this , SLOT(find_textChanged(const QString & )));

	connect(ui.p_expand_all, SIGNAL(clicked()), this, SLOT(on_expand()));
	connect(ui.pb_eapand_mostly, SIGNAL(clicked()), this, SLOT(on_expand()));

	refreshCommPorts();

	auto xml_path  = QApplication::applicationDirPath() + "/zigbee";
	QDir xml_dir(xml_path);
	xml_dir.setFilter(QDir::Files);
	xml_dir.setSorting(QDir::Name);
	xml_dir.setNameFilters( QStringList() << "*.xml");
	QFileInfoList xml_list = xml_dir.entryInfoList();
	foreach(QFileInfo xml_file,xml_list)
	{
		ui.comboBox->addItem(xml_file.baseName());
	}
	connect(ui.comboBox,SIGNAL(currentIndexChanged(const QString & )),this,SLOT(on_xml_changed(const QString &)));
	

	QSplitter *sp = new QSplitter(ui.widget_contain);
	sp->setOrientation(Qt::Horizontal);
	//--left 1
	QWidget* w = new QWidget(this);
#pragma region  left	
	QGridLayout* gl = new QGridLayout(w);
	gl->setContentsMargins(1,1,1,1);
	
	w->setLayout(gl);
	int row = 0;
	gl->addWidget(ui.lb_find, row, 0, 1, 1);
	gl->addWidget(ui.le_find, row, 1, 1, 1);
	row++;
	gl->addWidget(ui.label_2, row,0);
	gl->addWidget(ui.comboBox, row,1);
	row++;
	gl->addWidget(ui.p_expand_all, row, 0,1,2);
	row++;
	gl->addWidget(ui.treeWidget, row,0,1,2);
	row++;
	gl->addWidget(ui.pb_eapand_mostly, row++, 0, 1, 2);
	gl->addWidget(ui.tree_comm_mostused, row,0,1,2);
	

	
	//QSpacerItem* pSpace = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	//gl->addItem(pSpace, 6, 1, 1, 1);

	gl->setVerticalSpacing(2);
	sp->addWidget(w);
#pragma endregion

	//center
	w = new QWidget(this);
	gl = new QGridLayout(w);
	gl->setContentsMargins(1,1,1,1);
	w->setLayout(gl);
	QSpacerItem* spaceitem = new QSpacerItem(0,0);
	
	gl->addWidget(ui.tableWidget,0,0,1,3);
	gl->addWidget(ui.le_comm,1,0,1,3);
	gl->addItem(spaceitem,2,0);
	gl->addWidget(ui.pb_reset,2,1);
	gl->addWidget(ui.pbSend_comm,2,2);
	sp->addWidget(w);
	//end center

	//right
	w = new QWidget(this);
	gl = new QGridLayout(w);
	gl->setContentsMargins(1,1,1,1);
	w->setLayout(gl);
	spaceitem = new QSpacerItem(0,0);
	
	gl->addWidget(ui.treeWidget_2,0,0,1,3);
	gl->addWidget(ui.le_data,1,0,1,3);
	gl->addItem(spaceitem,2,0);
	gl->addWidget(ui.pb_history_setting,2,1);
	gl->addWidget(ui.pb_clear_history,2,2);

	sp->addWidget(w);

	//end right 

	gl = new QGridLayout(this);
	gl->setContentsMargins(1,1,1,1);
	gl->addWidget(sp,0,0);

	ui.tab->setLayout(gl);

	ready_send_file = false;
	is_new_command = false;
	clicked_item = nullptr;

	

	auto file_path = QApplication::applicationDirPath() + "/zigbee/";
	file_path += "ZigBeePro.xml";
	ZigBeePro_cmd = new FLXMLCommandManager(file_path, this);
	rec_comm_list = ZigBeePro_cmd->all_commmand();
	xml_cmd = new FLXMLCommandManager(file_path, this);
	InitTestCommand();
	//loadxml(file_path);
	init_tree(ui.treeWidget);

	binFileName = "";
	OTA_active = false;
}

FLZigbeeMonitor::~FLZigbeeMonitor()
{
	clear_tree();

	int num_topitem = ui.tree_comm_mostused->topLevelItemCount();
	for (int n = 0; n< num_topitem; n++)
	{
		auto topitem = ui.tree_comm_mostused->topLevelItem(n);
		int num_child = topitem->childCount();
		for (int m = 0; m<num_child; m++)
		{
			auto ch_item = topitem->child(m);
			QDomElement* element = (QDomElement*)ch_item->data(0, ItemPointerData).toULongLong();
			delete element;
		}
		QDomElement* element = (QDomElement*)topitem->data(0, ItemPointerData).toULongLong();
		delete element;
	}

	delete ZigBeePro_cmd;
	delete xml_cmd;
	if (m_serial)
	{
		m_serial->close();
		delete m_serial;
	}
}

void FLZigbeeMonitor::refreshCommPorts() 
{	
	ui.serialPortBox->clear();
	
	const char* blankString = "default";
	int idx = 0;
	const auto infos = QSerialPortInfo::availablePorts();
	for (int i = 0; i < infos.size(); i++)
	{
		const QSerialPortInfo &info = infos[i];
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

		//ui.serialPortBox->addItem(list.first(), list);
		ui.serialPortBox->addItem(info.portName() , list);
		// hard cording
		if (info.vendorIdentifier() == 0x403 && info.productIdentifier() == 0x6001)
			idx = i;
	}
	ui.serialPortBox->addItem(tr("Custom"));
	ui.serialPortBox->setCurrentIndex(idx);
}

void FLZigbeeMonitor::onConnect() 
{
	auto currentPortName = ui.serialPortBox->currentData().toStringList()[0];
	auto serial = new QSerialPort(currentPortName,this);

	//serial->setBaudRate(QSerialPort::Baud38400);
	//serial->setDataBits(QSerialPort::Data8);
	//serial->setParity(QSerialPort::NoParity);
	//serial->setFlowControl(QSerialPort::HardwareControl);

	if (!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Error"), serial->errorString());
		delete serial;				
		return;
	}
	set_port( serial);	
}

void FLZigbeeMonitor::set_port(QSerialPort * port)
{
	Q_ASSERT(port);
	m_serial = port;
	bool check = true;
	check = check &	port->setBaudRate(QSerialPort::Baud38400);
	check = check & port->setFlowControl(QSerialPort::NoFlowControl);
	disconnect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
	connect(m_serial, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	port->write("\r");
	ui.pbConnect->setText("Disconnect");
	ui.pbFresh->setEnabled(false);
	ui.serialPortBox->setEnabled(false);
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(on_disconnect()));
	/*if (_thread)
	delete _thread;
	_thread = new FLZigbeeThread(this);
	_thread->reset(serial, ZigBeePro_cmd);
	connect(_thread, SIGNAL(comm_msg(int, QStringList, QString)), this, SLOT(on_comm_msg(int, QStringList, QString)));
	connect(_thread, SIGNAL(data_msg(QStringList, QString)), this, SLOT(on_data_msg(QStringList, QString)));
	_thread->start();*/
}

void FLZigbeeMonitor::onReadyRead()
{
	receive();
	return;	
}

void PrintArray(QByteArray rcvBytes, QString name) {
	QString data = rcvBytes.toHex().toUpper();

	QString str_comm;
	for (int n = 0; n< data.length(); n += 2)
	{
		str_comm += QString("%1 ").arg(data.mid(n, 2));
	}

	qDebug() << name <<str_comm;
}

bool FLZigbeeMonitor::ExtractPacket()
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

void FLZigbeeMonitor::receive()
{
	// read packet from serial port
	const int waitTimeout = 100;
	
	QByteArray readBytes = m_serial->readAll();

	rcvBytes.append(readBytes);

	int len = rcvBytes.size();
	qDebug("\nreadBytes Len=%d rcvBytes(Total) Len=%d", readBytes.length(), len);
		
	while (ExtractPacket());
	
}

void FLZigbeeMonitor::on_disconnect()
{
	/*if (_thread && _thread->isRunning())
	{
		_thread->exit_thread();
	}

	if(m_serial)
	{
		m_serial->close();
		delete m_serial;
		m_serial = nullptr;
	}*/
	ui.pbConnect->setText("Connect");
	ui.serialPortBox->setEnabled(true);
	ui.pbFresh->setEnabled(true);
	connect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(onConnect()));
	disconnect(ui.pbConnect, SIGNAL(clicked()), this, SLOT(on_disconnect()));
}

void FLZigbeeMonitor::on_xml_changed(const QString &str)
{
	clear_tree();

	ui.treeWidget->setHeaderLabel("All Commands");
	auto file_path = QApplication::applicationDirPath() + "/zigbee/";
	file_path += str + ".xml";
	xml_cmd->loadxml(file_path);
	//rec_comm_list = xml_cmd->res_commmand();
	init_tree(ui.treeWidget);
	return;
	
}

void FLZigbeeMonitor::on_itemClicked(QTreeWidgetItem * item, int column)
{
	if(clicked_item == item)
		return;
	
	qDebug() << "clicked item";
	QDomElement* element = (QDomElement*)item->data(0,ItemPointerData).toULongLong();
	if(element == nullptr)
		return;
	
	if(element->tagName() == "Cmd")
	{		
		clicked_item = item;

		if (clicked_item == nullptr)
			return;
		qDebug() << "clicked item";
		QDomElement* element = (QDomElement*)clicked_item->data(0, ItemPointerData).toULongLong();

		on_reset_command(element);
	}
	else if (element->tagName() == "MacroScript")
	{
		clicked_item = item;

		ui.tableWidget->clear();
		ui.tableWidget->setRowCount(0);

		ui.tableWidget->setColumnCount(2);
		ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Parameter" << "Value");
		ui.tableWidget->setColumnWidth(0, 200);
		ui.tableWidget->setColumnWidth(1, 100);
		int index = 0;
		ui.tableWidget->insertRow(index);
		
		auto node = element->firstChild();
		QTableWidgetItem* item = new QTableWidgetItem("Command: " + node.toElement().text());
		ui.tableWidget->setItem(index, 0, item);
		ui.tableWidget->setSpan(0, 0, 1, 2);
		node = node.nextSibling();

		while (node.isNull() == false)
		{
			index ++;
			ui.tableWidget->insertRow(index);
			auto sub = node.toElement().text();
			int pos = sub.indexOf(" ");
			QTableWidgetItem* item = new QTableWidgetItem(sub.left(pos));
			ui.tableWidget->setItem(index, 0, item);

			item = new QTableWidgetItem(sub.right(sub.length() - pos));
			ui.tableWidget->setItem(index, 1, item);
			node = node.nextSibling();
		}
		//on_reset_command(element);
	}
	else if (element->tagName() == "MacroCmd")
	{
		//
	}
}

void FLZigbeeMonitor::on_edit_textchanged(const QString &str)
{
	if(str.startsWith("0x") == false)
	{
		auto edit = (QLineEdit*) sender();
		edit->setText("0x"+ str);
	}
	QDomElement* element = (QDomElement*)clicked_item->data(0, ItemPointerData).toULongLong();
	ui.le_comm->setText (comm_string(element));
}

QString FLZigbeeMonitor::comm_string(QDomElement* element)
{	
	if(element == nullptr )
		return "";

	if(element->tagName() != "Cmd")
		return "";

	is_new_command = true;
	auto comm_text = element->firstChildElement("CmdName").text();
	char TX = 0x02;
	QByteArray arr;
	arr.append(TX);

	//add Sync	
	comm_text = element->firstChildElement("CmdHeader").text();
	auto head_list = comm_text.split(" ");
	bool bOk;
	foreach(auto str,head_list)
	{
		int hex = str.toInt(&bOk,16);
		arr.append(hex);
	}
	//add 	
	arr.append(0x01);

	int nLeng = 0;

	int row_count = ui.tableWidget->rowCount();
	bool bOK;
	for(int n=1; n< row_count; n++)
	{
		QLabel* item_label = (QLabel*)ui.tableWidget->cellWidget(n,0);
		QWidget* item_w = ui.tableWidget->cellWidget(n,1);
		QStringList lt;
		
		if(qobject_cast<QComboBox*> (item_w))
		{
			if(item_w->property("SelectName").toString() != "")
			{
				continue;
			}
			QComboBox* box = qobject_cast<QComboBox*> (item_w);
			auto item_data = box->currentData().toString();
			item_data = item_data.right(2);
			int hex = item_data.toInt(&bOk,16);
			if(bOk)
				arr.append(hex);
			lt << item_label->text() << "[1 byte]" << QString::number(hex,16);
		}
		else if(qobject_cast<QCheckBox*> (item_w))
		{
			QCheckBox* box = qobject_cast<QCheckBox*> (item_w);
			char ch;
			if(box->isChecked())
				ch = 0x01;
			else
				ch = 0x00;
			lt << item_label->text() << "[1 byte]" << QString::number(ch,16);
			arr.append(ch);
		}
		else if(qobject_cast<QLineEdit*> (item_w))
		{
			QLineEdit* edit = qobject_cast<QLineEdit*> (item_w);
			auto tx= edit->text();
			QStringList data_list = tx.split(",", QString::SkipEmptyParts);
			if(data_list.count()!=1)
			{
				int hex;
				foreach(auto str, data_list)
				{
					if(str.startsWith("0x"))
						str = str.right(tx.length()-2);
					while (str.length()>=2)
					{
						QString right = str.right(2);
						hex = right.toInt(&bOk,16);
						arr.append(hex);
						str = str.left(str.length()-2);
					}
				}
			}
			else
			{
				int pos  = 0;
				int hex;
				if(tx.startsWith("0x"))
					tx = tx.right(tx.length()-2);
				while (tx.length()>=2)
				{
					QString right = tx.right(2);
					hex = right.toInt(&bOk,16);
					arr.append(hex);
					tx = tx.left(tx.length()-2);
				}
			}
			QRegExp exp ("[\\d]"); 
			int pos = 0;
			auto lb_text = item_label->text();
			int param_len = 0;
			while( (pos = exp.indexIn(lb_text,pos )) != -1)
			{
				param_len += exp.cap(0).toInt();
				pos += exp.matchedLength();
			}
			pos = lb_text.indexOf("[");
			if(pos !=-1)
				lb_text = lb_text.left(pos);
			lt << lb_text << "[" + QString::number(param_len) + " byte]" << edit->text();
		}
		
	}

	char ch_len = arr.length() - 4;
	arr[3] = ch_len;

	arr. append( Checksum(arr,arr.length()));	

	QString str_comm;
	for (int n=0; n< arr.length(); n++)
	{
		//str_comm += QString::number(arr[n],16) + " ";
		str_comm += QString("%1 ").arg((unsigned char)arr[n],2,16,QLatin1Char('0'));
	}
	
	return str_comm;

}

void FLZigbeeMonitor::on_itemDoubleClicked(QTreeWidgetItem * item, int column)
{
	QDomElement* element = (QDomElement*)item->data(0,ItemPointerData).toULongLong();
	if(element == nullptr || m_serial == nullptr)
		return;
	
	if(element->tagName() != "Cmd")
		return;
	QTreeWidgetItem* top_item = new QTreeWidgetItem();
	ui.treeWidget_2->addTopLevelItem(top_item);

	is_new_command = true;
	auto comm_text = element->firstChildElement("CmdName").text();
	char TX = 0x02;
	QByteArray arr;
	arr.append(TX);
	
	//add Sync
	QTreeWidgetItem* tree_item = new QTreeWidgetItem();
	set_item(tree_item,QStringList() << "Sync" << "[1 byte]" << "02");
	top_item->addChild(tree_item);

	comm_text = element->firstChildElement("CmdHeader").text();
	auto head_list = comm_text.split(" ");
	bool bOk;
	foreach(auto str,head_list)
	{
		int hex = str.toInt(&bOk,16);
		arr.append(hex);
	}
	//add 
	tree_item = new QTreeWidgetItem();
	set_item(tree_item,QStringList() << "OpGroup" << "[1 byte]" << head_list[0]);//setText(0,"OpGroup" + str6_blank + "[1 byte]"+ head_list[0]);
	top_item->addChild(tree_item);
	tree_item = new QTreeWidgetItem();
	set_item(tree_item,QStringList() << "OpOption" << "[1 byte]" << head_list[1]);//tree_item->setText(0, item_dis_text.arg("OpGroup",8).arg(1,2).arg(head_list[1],2));
	top_item->addChild(tree_item);


	arr.append(0x01);

	QTreeWidgetItem* length_item = new QTreeWidgetItem();
	set_item(length_item,QStringList() << "Length" << "[1 byte]" << "01");
	top_item->addChild(length_item);

	int nLeng = 0;

	int row_count = ui.tableWidget->rowCount();
	bool bOK;
	for(int n=1; n< row_count; n++)
	{
		QStringList lt;
		QLabel* item_label = (QLabel*)ui.tableWidget->cellWidget(n,0);
		QWidget* item_w = ui.tableWidget->cellWidget(n,1);
		if(item_w == nullptr)
		{
			int v = 0;
			//n++;
			while( n< row_count)
			{
				n++;
				item_w = ui.tableWidget->cellWidget(n,1);
				auto box = qobject_cast<QCheckBox*>(item_w);
				if(box && box->property("Value").isValid() )
				{
					QWidget* ppp =(QWidget*) box->parent();
					if(box->isChecked())
					{
						auto param_value = box->property("Value").toString();	

						param_value = param_value.right(2);
						int hex = param_value.toInt(&bOk,16);
						if(bOk)
							v += hex;
					}
											
				}
				else
				{
					n--;
					break;
				}
				
			}
			
			lt<<item_label->text() << "[1 byte]" <<  QString::number(v,16);
		}
		
		else if(qobject_cast<QComboBox*> (item_w))
		{
			QComboBox* box = qobject_cast<QComboBox*> (item_w);
			auto item_data = box->currentData().toString();
			item_data = item_data.right(2);
			int hex = item_data.toInt(&bOk,16);
			if(bOk)
				arr.append(hex);
			lt << item_label->text() << "[1 byte]" << QString::number(hex,16);
		}
		else if(qobject_cast<QCheckBox*> (item_w))
		{
			QCheckBox* box = qobject_cast<QCheckBox*> (item_w);
			char ch;
			if(box->isChecked())
				ch = 0x01;
			else
				ch = 0x00;
			lt << item_label->text() << "[1 byte]" << QString::number(ch,16);
			arr.append(ch);
		}
		else if(qobject_cast<QLineEdit*> (item_w))
		{
			QLineEdit* edit = qobject_cast<QLineEdit*> (item_w);
			auto tx= edit->text();
			QStringList data_list = tx.split(",", QString::SkipEmptyParts);
			if(data_list.count()!=1)
			{
				int hex;
				foreach(auto str, data_list)
				{
					if(str.startsWith("0x"))
						str = str.right(tx.length()-2);
					while (str.length()>=2)
					{
						QString right = str.right(2);
						hex = right.toInt(&bOk,16);
						arr.append(hex);
						str = str.left(str.length()-2);
					}
				}
			}
			else
			{
				int pos  = 0;
				int hex;
				if(tx.startsWith("0x"))
					tx = tx.right(tx.length()-2);
				while (tx.length()>=2)
				{
					QString right = tx.right(2);
					hex = right.toInt(&bOk,16);
					arr.append(hex);
					tx = tx.left(tx.length()-2);
				}
			}
			QRegExp exp ("[\\d]"); 
			int pos = 0;
			auto lb_text = item_label->text();
			int param_len = 0;
			while( (pos = exp.indexIn(lb_text,pos )) != -1)
			{
				param_len += exp.cap(0).toInt();
				pos += exp.matchedLength();
			}
			pos = lb_text.indexOf("[");
			if(pos !=-1)
				lb_text = lb_text.left(pos);
			lt << lb_text << "[" + QString::number(param_len) + " byte]" << edit->text();
		}

		tree_item = new QTreeWidgetItem();
		set_item(tree_item,lt);
		top_item->addChild(tree_item);
	}

	char ch_len = arr.length() - 4;
	arr[3] = ch_len;

	arr. append( Checksum(arr,arr.length()));	
	
	QString str_comm;
	for (int n=0; n< arr.length(); n++)
	{
		//str_comm += QString::number(arr[n],16) + " ";
		str_comm += QString("%1 ").arg((unsigned char)arr[n],2,16,QLatin1Char('0'));
	}
	str_comm+="\r\n";
	comm_text = element->firstChildElement("CmdName").text();
	
	QLabel* top_label = new QLabel("TX: " + comm_text + ": " + str_comm.toUpper(),this);
	top_label->setWordWrap(true);
	//top_item->setText(0,);
	ui.treeWidget_2->setItemWidget(top_item,0,top_label);

	m_serial->write(arr);
}

void FLZigbeeMonitor::parseParam(QDomNode param_node,QWidget* parent_w)
{
	if(parent_w == nullptr)
		parent_w = ui.tableWidget;

	while(param_node.isNull() == false)
	{		
		auto param_element = param_node.toElement();
		
		//default value ;
		auto param_deafault_node = param_element.firstChildElement("ParmDefaultValue");
		QString str_default_value = param_deafault_node.text();
		if(str_default_value.isEmpty())
			str_default_value = "00";
		auto type_node = param_element.firstChildElement("ParmType");
				
		QWidget* node_widget = nullptr;
		auto param_name = param_element.firstChildElement("ParmName").text();
		qDebug() << "parseParam = " << param_element.firstChildElement("ParmName").text();
		QLabel* label = new QLabel(param_element.firstChildElement("ParmName").text(),parent_w);

		int index = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(index);
		ui.tableWidget->setCellWidget (index,0,label);

		if(type_node.text() == "tEnum")
		{
			auto combo =  new QComboBox(parent_w);
			auto param_items = param_node.firstChildElement("ParmTypeSpecifics");
			auto param_item = param_items.firstChild();
			while(param_item.isNull() == false)
			{
				combo->addItem( param_item.firstChildElement("ParmText").text(),param_item.firstChildElement("ParmValue").text());
				param_item= param_item.nextSibling();
			}
			node_widget = combo;
			combo->setCurrentIndex(str_default_value.toInt());
			connect(combo, SIGNAL(currentIndexChanged(int)),this, SLOT(on_enum_item_changed(int)));
			combo->setProperty("SelectName",parent_w->property("SelectName").toString());
		}
		else if(type_node.text() == "tEdit" ||type_node.text() == "tInt")
		{
			QLineEdit* edit = new QLineEdit(parent_w);
			auto param_size_node = param_node.firstChildElement("ParmSize");
			QString tx = label->text();
			tx = QString("%1[%2]").arg(tx).arg(param_size_node.text());
			label->setText (  tx );

			node_widget = edit;
			edit->setText("0x" + str_default_value.remove(" "));
			edit->setProperty("Row_Index",index);
			connect( edit, SIGNAL(	textEdited(const QString & )), this,SLOT(on_edit_textchanged(const QString &)));
			edit->setProperty("SelectName",parent_w->property("SelectName").toString());

		}
		else if(type_node.text() == "tBool")
		{
			QCheckBox* ch_box = new QCheckBox(parent_w);
			node_widget = ch_box;
			ch_box->setCheckState(str_default_value.toInt()==1?Qt::Checked:Qt::Unchecked);
			ch_box->setProperty("SelectName",parent_w->property("SelectName").toString());
		}
		else if(type_node.text() == "tArray")
		{
			QLineEdit* edit = new QLineEdit(ui.tableWidget);
			auto param_size_node = param_node.firstChildElement("ParmSize");
			QString tx = label->text();
			tx = QString("%1[%2][2]").arg(tx).arg(param_size_node.text());
			label->setText (  tx );
			node_widget = edit;
			auto default_list = str_default_value.split(" ");
			QString default_str;
			foreach(auto str, default_list)
			{
				default_str += "0x" + str + ", ";
			}
			default_str = default_str.left(default_str.length()-2);
			edit->setText(default_str);
			edit->setProperty("SelectName",parent_w->property("SelectName").toString());
			//connect( edit, SIGNAL(	textEdited(const QString & )), this,SLOT(on_edit_textchanged(const QString &)));
		}
		else if(type_node.text() == "tSelect")
		{
			auto box =  new QComboBox(parent_w);
			QString ssss = label->text();
			box->setProperty("SelectName",label->text());
			auto item = param_node.firstChildElement("ParmTypeSpecifics");
			if(item.isNull())
				continue;

			auto para_item = item.firstChildElement("Parm");
			while(para_item.isNull() == false)
			{
				//parseParam(item);
				qDebug() << "Select items = " << para_item.firstChildElement("ParmName").text() ;
				box->addItem( para_item.firstChildElement("ParmName").text() ,quintptr(new QDomNode(para_item)));
				para_item = para_item.nextSiblingElement();
			}

			item = param_node.firstChildElement("ParmTypeSpecifics");
			item = item.firstChildElement("Depend");
			auto cur_select = item.firstChildElement("DependShift").text().toInt();

			item = param_node.firstChildElement("ParmTypeSpecifics");
			para_item = item.firstChildElement("Parm");
			while(para_item.isNull() == false && cur_select>0)
			{
				para_item = para_item.nextSiblingElement();
				cur_select--;
			}
			cur_select = item.firstChildElement("DependShift").text().toInt();
			parse_struct_param(para_item,box);
			node_widget = box;
			connect(box, SIGNAL(currentIndexChanged(int )),this,SLOT(on_select_item_changed(int )));
			
		}
		else if(type_node.text() == "tBitArray")
		{
			auto child_param = param_node.firstChildElement("ParmTypeSpecifics").firstChild();
			//child_param = child_param.firstChild();
			while(child_param.isNull() == false)
			{
				int index_child = ui.tableWidget->rowCount();
				auto desc = child_param.firstChildElement("ParmDesc");
				auto value = child_param.firstChildElement("ParmName");
				auto last_value = child_param.firstChildElement("ParmLastValue").text().toInt();
				QLabel* lable_child = new QLabel(label);
				qDebug() << "desc.text() = " << desc.text();
				lable_child->setText(desc.text() + "[" + value.text() + "]");
				QCheckBox* child_box = new QCheckBox(label);
				child_box->setChecked( last_value == 1);
				child_box->setProperty("Value",value.text());
				ui.tableWidget->insertRow(index_child);
				ui.tableWidget->setCellWidget (index_child,0,lable_child);
				ui.tableWidget->setCellWidget(index_child,1,child_box);
				child_param = child_param.nextSibling();
			}			
		}		
		
		ui.tableWidget->setCellWidget(index,1,node_widget);
		param_node = param_node.nextSibling();
	
	}
}

void FLZigbeeMonitor::parse_struct_param(QDomNode command,QComboBox* box)
{
	auto item = command.firstChildElement("ParmTypeSpecifics");
	item = item.firstChildElement("Parm");
	while (item.isNull() == false)
	{
		parseParam(item,box);
		item = item.nextSiblingElement();
	}
}

void FLZigbeeMonitor::on_select_item_changed(int index)
{
	QComboBox* box = (QComboBox*) sender();
	QString pro = box->property("SelectName").toString();
	int combo_index;
	//remove all related item;
	int nRow = ui.tableWidget->rowCount();
	for(int n=nRow-1; n>0; n--)
	{
		QWidget* w =(QWidget*) ui.tableWidget->cellWidget(n,1);
		if(w == box)
		{
			combo_index = n;
			continue;
		}
		auto pw = w->parent();
		QString pro1 = w->property("SelectName").toString();
		if(w->parent() == box || pro == pro1)
		{
			ui.tableWidget->removeRow(n);
		}
	}
	//add new item;
	QDomNode* element = (QDomNode*)clicked_item->data(0,ItemPointerData).toULongLong();
	if(element == nullptr)
		return;
	
	auto param_node = *(QDomNode*)box->currentData().toULongLong();	
	param_node = param_node.firstChildElement("ParmTypeSpecifics").firstChild();
	while(param_node.isNull() == false)
	{	
		QWidget* node_widget = nullptr;
		
		QLabel* label = new QLabel(param_node.firstChildElement("ParmName").text(),box);
		label->setProperty("SelectName",box->property("SelectName").toString());
		int index = ui.tableWidget->rowCount();
		ui.tableWidget->insertRow(index);
		ui.tableWidget->setCellWidget (index,0,label);
		auto type_node = param_node.firstChildElement("ParmType");
		auto param_deafault_node = param_node.firstChildElement("ParmDefaultValue");
		QString str_default_value = param_deafault_node.text();

		if(type_node.text() == "tEnum")
		{
			auto combo =  new QComboBox(box);
			auto param_items = param_node.firstChildElement("ParmTypeSpecifics");
			auto param_item = param_items.firstChild();
			while(param_item.isNull() == false)
			{
				combo->addItem( param_item.firstChildElement("ParmText").text(),param_item.firstChildElement("ParmValue").text());
				param_item= param_item.nextSibling();
			}
			node_widget = combo;
			combo->setCurrentIndex(str_default_value.toInt());
			connect(combo, SIGNAL(currentIndexChanged(int)),this, SLOT(on_enum_item_changed(int)));
			combo->setProperty("SelectName",box->property("SelectName").toString());
		}
		else if(type_node.text() == "tEdit" ||type_node.text() == "tInt")
		{
			QLineEdit* edit = new QLineEdit(box);
			auto param_size_node = param_node.firstChildElement("ParmSize");
			QString tx = label->text();
			tx = QString("%1[%2]").arg(tx).arg(param_size_node.text());
			label->setText (  tx );

			node_widget = edit;
			edit->setText("0x" + str_default_value.remove(" "));
			edit->setProperty("Row_Index",index);
			connect( edit, SIGNAL(	textEdited(const QString & )), this,SLOT(on_edit_textchanged(const QString &)));
			edit->setProperty("SelectName",box->property("SelectName").toString());
		}
		else if(type_node.text() == "tBool")
		{
			QCheckBox* ch_box = new QCheckBox(box);
			node_widget = ch_box;
			ch_box->setCheckState(str_default_value.toInt()==1?Qt::Checked:Qt::Unchecked);
			ch_box->setProperty("SelectName",box->property("SelectName").toString());
		}
		else if(type_node.text() == "tArray")
		{
			QLineEdit* edit = new QLineEdit(box);
			auto param_size_node = param_node.firstChildElement("ParmSize");
			QString tx = label->text();
			tx = QString("%1[%2][2]").arg(tx).arg(param_size_node.text());
			label->setText (  tx );
			node_widget = edit;
			auto default_list = str_default_value.split(" ");
			QString default_str;
			foreach(auto str, default_list)
			{
				default_str += "0x" + str + ", ";
			}
			default_str = default_str.left(default_str.length()-2);
			edit->setText(default_str);
			edit->setProperty("SelectName",box->property("SelectName").toString());
			//connect( edit, SIGNAL(	textEdited(const QString & )), this,SLOT(on_edit_textchanged(const QString &)));
		}		
		//node_widget->setParent(box);
		ui.tableWidget->setCellWidget(index,1,node_widget);
		param_node = param_node.nextSibling();

	}
}

void FLZigbeeMonitor::set_item(QTreeWidgetItem* item,QStringList &lt)
{
	int index = 0;
	QString str1 = " ";
	QString item_text;
	int len = 14;
	item->setFlags( item->flags()^Qt::ItemIsTristate);
	lt[0] = lt[0].left(3);
	foreach(auto str, lt)
	{
		if(str.length() < len)
		{
			str += str1.repeated(len - str.length());
		}
		item_text += str.toUpper();
		
	}
	item->setText(0,item_text);
	index++;
}

void FLZigbeeMonitor::on_send_command()
{
	if(clicked_item)
		on_itemDoubleClicked(clicked_item,0);
}

void FLZigbeeMonitor::on_clear_history()
{
	ui.treeWidget_2->clear();
}

void FLZigbeeMonitor::on_enum_item_changed(int)
{
	QDomElement* element = (QDomElement*)clicked_item->data(0, ItemPointerData).toULongLong();
	ui.le_comm->setText (comm_string(element));
}

void FLZigbeeMonitor::on_reset_command(QDomElement* element)
{	
	if(element == nullptr)
		return;
	ui.tableWidget->clear();
	ui.tableWidget->setRowCount(0);

	ui.tableWidget->setColumnCount(2);
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Parameter" << "Value");
	ui.tableWidget->setColumnWidth(0,200);
	ui.tableWidget->setColumnWidth(1,100);
	//delete old;

	if(element->tagName() == "Cmd")
	{			
		auto comm_text = element->firstChildElement("CmdName").text();

		auto param_nodes = element->firstChildElement("CmdParms");

		auto param_node = param_nodes.firstChild();//param
		int index = 0;
		ui.tableWidget->insertRow(0);
		QTableWidgetItem* item = new QTableWidgetItem("Command: " + comm_text);
		QFont ft;
		ft.setWeight(15);
		ft.setBold(true);
		item->setFont(ft);
		ui.tableWidget->setItem(0,0,item);
		ui.tableWidget->setSpan(0,0,1,2);
		parseParam(param_node);
	}
	else if (element->tagName() == "MacroCmd")
	{
		auto cmd_name = clicked_item->text(0);
		auto node = ZigBeePro_cmd->find_node_byName(cmd_name);
		if (node.isNull() == false)
		{
			auto comm_text = node.firstChildElement("CmdName").text();

			auto param_nodes = node.firstChildElement("CmdParms");

			auto param_node = param_nodes.firstChild();//param
			int index = 0;
			ui.tableWidget->insertRow(0);
			QTableWidgetItem* item = new QTableWidgetItem("Command: " + comm_text);
			QFont ft;
			ft.setWeight(15);
			ft.setBold(true);
			item->setFont(ft);
			ui.tableWidget->setItem(0, 0, item);
			ui.tableWidget->setSpan(0, 0, 1, 2);
			parseParam(param_node);
		}
	}
	//QDomElement* element = (QDomElement*)clicked_item->data(0, ItemPointerData).toULongLong();
	ui.le_comm->setText (comm_string(element));
}

void FLZigbeeMonitor::showPortInfo(int idx)
{
	if (idx == -1)
		return;

	QStringList list = ui.serialPortBox->itemData(idx).toStringList();
	const char* blankString= "Default";
	ui.descriptionLabel->setText(tr("Description: %1").arg(list.count() > 1 ? list.at(1) : tr(blankString)));
	ui.manufacturerLabel->setText(tr("Manufacturer: %1").arg(list.count() > 2 ? list.at(2) : tr(blankString)));
	ui.serialNumberLabel->setText(tr("Serial number: %1").arg(list.count() > 3 ? list.at(3) : tr(blankString)));
	ui.locationLabel->setText(tr("Location: %1").arg(list.count() > 4 ? list.at(4) : tr(blankString)));
	ui.vendorIDLabel->setText(tr("Vendor Identifier: %1").arg(list.count() > 5 ? list.at(5) : tr(blankString)));
	ui.productIDLabel->setText(tr("Product Identifier: %1").arg(list.count() > 6 ? list.at(6) : tr(blankString)));
}

void FLZigbeeMonitor::InitTestCommand()
{
	QString xml_path  = QApplication::applicationDirPath() + "/mostused/TestCommands.xml";

	ui.tree_comm_mostused->clear();
	ui.tree_comm_mostused->setHeaderLabel("Test/Debug Commands");
	
	QFile xml_file(xml_path);
	QDomDocument dom;
	if (!xml_file.open(QIODevice::ReadOnly))
		return ;

	if (!dom.setContent(&xml_file)) {
		xml_file.close();
		return;
	}

	xml_file.close();

	if (dom.isNull() || (!dom.isDocument()))
	{
		qDebug() << "QxPluginServer::parseManifest: Failed to parse the manifest file!";		
	}

	//QDomNodeList pluginNodes = dom.elementsByTagName("plugin");
	QDomElement docElem = dom.documentElement();//zigbee

	auto node = docElem.firstChild();//Settings
	node = node.firstChild();//defaltConmannd

	while(node.isNull() == false)
	{
		QDomElement e = node.toElement(); // try to convert the node to an element.
		if(!e.isNull()) {
			qDebug() << qPrintable(e.tagName()) ; // the node really is an element.

			auto child = node.firstChild();
			while(child.isNull() == false)
			{
				if(child.toElement().tagName() == "MacroCmd")
				{
					QTreeWidgetItem* child_item = new QTreeWidgetItem(ui.tree_comm_mostused);
					child_item->setText(0,child.toElement().text());
					auto nd = ZigBeePro_cmd->find_node_byName(child.toElement().text());
					child_item->setData(0,ItemPointerData, (quintptr) new QDomElement(nd.toElement()));
					ui.tree_comm_mostused->addTopLevelItem(child_item);
				}
				else if(child.toElement().tagName() == "MacroScript")
				{
					auto marco_child = child.firstChild();//name

					QTreeWidgetItem* child_item = new QTreeWidgetItem(ui.tree_comm_mostused);
					child_item->setText(0,marco_child.toElement().text());
					child_item->setData(0,ItemPointerData, (quintptr) new QDomElement(child.toElement()) );
					ui.tree_comm_mostused->addTopLevelItem(child_item);
					marco_child = marco_child.nextSibling();
					while(marco_child.isNull() == false)
					{
						if(marco_child.toElement().tagName() == "MacroCmd")
						{
							QTreeWidgetItem* marco_child_item = new QTreeWidgetItem(child_item);
							marco_child_item->setText(0,marco_child.toElement().text());
							marco_child_item->setData(0,ItemPointerData, (quintptr) new QDomElement(marco_child.toElement()) );
							child_item->addChild(marco_child_item);
						}
						marco_child = marco_child.nextSibling();
					}
				}
				child = child.nextSibling();
			}

		}
		node = node.nextSibling();
	}
}

void FLZigbeeMonitor::clear_tree()
{
	clicked_item = nullptr;
	int num_topitem = ui.treeWidget->topLevelItemCount();
	for (int n = 0; n< num_topitem; n++)
	{
		auto topitem = ui.treeWidget->topLevelItem(n);
		int num_child = topitem->childCount();
		for (int m = 0; m<num_child; m++)
		{
			auto ch_item = topitem->child(m);
			QDomElement* element = (QDomElement*)ch_item->data(0, ItemPointerData).toULongLong();
			delete element;
		}
	}
	ui.treeWidget->clear();
}

void FLZigbeeMonitor::on_mostusedtree_DoubleClicked(QTreeWidgetItem * item, int column)
{
	if(m_serial == nullptr)
		return;
	if(item == nullptr)
		return;
	
	auto cmd_item = (QDomElement*)item->data(0,ItemPointerData).toULongLong();
	qDebug() << " item tagname " << cmd_item->tagName();
	if(cmd_item==nullptr )
		return;
	
	if( cmd_item->tagName()== "MacroCmd")
	{
		if (item->parent() == nullptr)
		{
			auto cmd_name = item->text(0);
			auto node = ZigBeePro_cmd->find_node_byName(cmd_name);
			if (node.isNull() == false)
				excute_command(node);
		}
		else//command of script;
		{

		}
		
		
	}
	else if(cmd_item->tagName()== "MacroScript")
	{
		auto cmd = cmd_item->firstChild();//name
		cmd = cmd.nextSibling();//first cmd
		while(cmd.isNull() == false)
		{
			excute_script_command(cmd);				
			cmd = cmd.nextSibling();
		}
	}
	else
	{
		on_send_command();
	}
}

void FLZigbeeMonitor::on_mostusedtree_Menu(const QPoint & pt)
{
	if (m_serial == nullptr)
		return;
	QTreeWidgetItem* item = ui.tree_comm_mostused-> itemAt(pt);
	if (item == nullptr)
		return;
	QPoint pt1 = ui.tree_comm_mostused->mapToGlobal(pt);

	auto cmd_item = (QDomElement*)item->data(0, ItemPointerData).toULongLong();
	qDebug() << " item tagname " << cmd_item->tagName();
	if (cmd_item == nullptr)
		return;

	if (cmd_item->tagName() == "MacroCmd")
	{
		QMenu menu;
		QAction *a = menu.addAction(QIcon(":/FLJobWindow/icons/add-floor-icon.png"), tr("Excute Command"));
		
		QAction *res = 0;
		res = menu.exec(pt1);
		if (res == 0) return;
		auto cmd_name = item->text(0);
		auto node = ZigBeePro_cmd->find_node_byName(cmd_name);
		if (node.isNull() == false)
			excute_command(node);

	}
	else if (cmd_item->tagName() == "MacroScript")
	{
		QMenu menu;
		QAction *a = menu.addAction(QIcon(":/FLJobWindow/icons/add-floor-icon.png"), tr("Excute Command Group"));
		
		QAction *res = 0;
		res = menu.exec(pt1);
		if (res == 0) return;		


		auto cmd = cmd_item->firstChild();//name
		cmd = cmd.nextSibling();//first cmd
		while (cmd.isNull() == false)
		{
			auto name = cmd.toElement().text();
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
				set_item(tree_item, QStringList() << "Len" << "[1 BYTE]" << QString::number(ch_len, 16));
				arr.append(Checksum(arr, arr.length()));
				m_serial->write(arr);

				cmd = cmd.nextSibling();

			}
		}

	}
}

void FLZigbeeMonitor::excute_command(QDomNode node)
{
	if(m_serial == nullptr)
		return;
	auto comm_text = node.firstChildElement("CmdHeader").text();
	auto head_list = comm_text.split(" ");

	QTreeWidgetItem* top_item = new QTreeWidgetItem();
	ui.treeWidget_2->addTopLevelItem(top_item);

	bool bOk;
	QByteArray arr;
	//header
	char TX = 0x02;
	arr.append(TX);
	foreach(auto str,head_list)
	{
		int hex = str.toInt(&bOk,16);
		arr.append(hex);
	}
	arr.append('0');


	QTreeWidgetItem* tree_item = new QTreeWidgetItem();
	set_item(tree_item,QStringList() << "Sync" << "[1 byte]" << "02");
	top_item->addChild(tree_item);	
	tree_item = new QTreeWidgetItem();
	set_item(tree_item,QStringList() << "OpGroup" << "[1 byte]" << head_list[0]);//setText(0,"OpGroup" + str6_blank + "[1 byte]"+ head_list[0]);
	top_item->addChild(tree_item);
	tree_item = new QTreeWidgetItem();
	set_item(tree_item,QStringList() << "OpOption" << "[1 byte]" << head_list[1]);//tree_item->setText(0, item_dis_text.arg("OpGroup",8).arg(1,2).arg(head_list[1],2));
	top_item->addChild(tree_item);
	QTreeWidgetItem *len_item = new QTreeWidgetItem();
	set_item(len_item,QStringList() << "Lenth" << "[1 byte]" << head_list[1]);//tree_item->setText(0, item_dis_text.arg("OpGroup",8).arg(1,2).arg(head_list[1],2));
	top_item->addChild(len_item);


	//parameter;
	auto params = node.firstChildElement("CmdParms");
	auto param_node = params.firstChild();

	while(param_node.isNull() == false)
	{		
		auto param_element = param_node.toElement();

		//default value ;
		QString default_value = param_element.firstChildElement("ParmDefaultValue").text();
		auto param_name = param_element.firstChildElement("ParmName").text();
		if(default_value.isEmpty())
			default_value = "00";
		auto type_node = param_element.firstChildElement("ParmType");

// 		if(type_node.text() == "tEnum")
// 		{
// 		}
// 		else if(type_node.text() == "tEdit" )
// 		{			
// 		}
// 		else if(type_node.text() == "tInt")
// 		{
// 		}
// 		else if(type_node.text() == "tBool")
// 		{
// 		}
// 		else 
		if(type_node.text() == "tBitArray")
		{

		}
// 		else if(type_node.text() == "tArray")
// 		{
// 			
// 		}
		else if(type_node.text() == "tSelect")
		{		

		}
		
		
		auto param_list = default_value.split(" ");
		foreach(auto param,param_list)
		{
			int hex = param.toInt(&bOk,16);
			arr.append(hex);
		}
		tree_item = new QTreeWidgetItem();
		set_item(tree_item,QStringList() << param_name << QString("[%1 byte]").arg(param_list.count()) << default_value);//tree_item->setText(0, item_dis_text.arg("OpGroup",8).arg(1,2).arg(head_list[1],2));
		top_item->addChild(tree_item);
		param_node = param_node.nextSibling();

	}


	char ch_len = arr.length() - 4;
	arr[3] = ch_len;

	arr.append( Checksum(arr,arr.length()));	

	QString str_comm;
	for (int n=0; n< arr.length(); n++)
	{
		//str_comm += QString::number(arr[n],16) + " ";
		str_comm += QString("%1 ").arg((unsigned char)arr[n],2,16,QLatin1Char('0'));
	}
	str_comm+="\r\n";
	comm_text = node.firstChildElement("CmdName").text();

	QLabel* top_label = new QLabel("TX: " + comm_text + ": " + str_comm.toUpper(),this);
	top_label->setWordWrap(true);
	//top_item->setText(0,);
	set_item(len_item,QStringList() << "Length" <<"[1 BYTE]"<< QString::number(ch_len) );//tree_item->setText(0, item_dis_text.arg("OpGroup",8).arg(1,2).arg(head_list[1],2));
	ui.treeWidget_2->setItemWidget(top_item,0,top_label);
	ui.le_comm->setText(str_comm);
	m_serial->write(arr);

}

void FLZigbeeMonitor::find_textChanged(const QString & str)
{
	ui.treeWidget->collapseAll();
	ui.treeWidget->clearSelection();
	int number_topitem = ui.treeWidget->topLevelItemCount();
	for(int n=0; n< number_topitem; n++)
	{
		QTreeWidgetItem* top_item = ui.treeWidget->topLevelItem(n);
		int number_child = top_item->childCount();
		for(int m=0; m< number_child; m++)
		{
			QTreeWidgetItem* child = top_item->child(m);
			if(child->text(0).contains(str,Qt::CaseInsensitive))
			{
				child->setSelected(true);
				ui.treeWidget->expandItem(top_item);
				ui.treeWidget->scrollToItem(child);
			}
		}
	}
}

void FLZigbeeMonitor::init_tree(QTreeWidget* tree)
{
	tree->clear();
	QMap<QString,QList<QDomNode>>::const_iterator it = xml_cmd->all_cmd_group().constBegin();
	while (it != xml_cmd->all_cmd_group().constEnd())
	{
		auto list = it.value();
		if (list.count() == 0)
			continue;

		QTreeWidgetItem* top_item = new QTreeWidgetItem(tree);
		top_item->setText(0,it.key());
		tree->addTopLevelItem(top_item);
		
		foreach(auto node, list)
		{
			auto comm_node = node.toElement();
			if(comm_node.tagName() == "Cmd")
			{
				auto comm_text = comm_node.firstChildElement("CmdName").text();
				if (comm_text.endsWith("Response",Qt::CaseInsensitive) || comm_text.endsWith("Confirm",Qt::CaseInsensitive) 
					|| comm_text.endsWith("event",Qt::CaseInsensitive) || comm_text.endsWith("Indication",Qt::CaseInsensitive))
				{							
					continue;
				}
				auto comm_item = new QTreeWidgetItem(top_item);				
				comm_item->setText(0,comm_text);
				top_item->addChild(comm_item);
				comm_item->setData(0,ItemPointerData,(quintptr) new QDomElement(comm_node));
			}
		}
		it++;
	}
}

void FLZigbeeMonitor::excute_script_command(QDomNode script_node)
{
	auto name = script_node.toElement().text();
	auto name_list = name.split(" ");
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
		set_item(tree_item, QStringList() << "Len" << "[1 BYTE]" << QString::number(ch_len, 16));
		arr.append(Checksum(arr, arr.length()));
		m_serial->write(arr);
	}
}

void FLZigbeeMonitor::parse_rec_data(QByteArray arr)
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
		else if(rec_list[1] == "9B" && rec_list[2] == "40")
		{
			//qDebug() << "NLDE-Data.Indication-1";
			//ProcessData(arr);
		}
		else if (rec_list[1] == "9B" && rec_list[2] == "3F")
		{
			int ms = lapseTimer.elapsed();


			qDebug() << ms  << "NLDE-Data.Confirmation";
			
		}

		bool is_confim_req = false;

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

void FLZigbeeMonitor::add_child_item(QTreeWidgetItem* item, QStringList &lt)
{
	QTreeWidgetItem* ch_item = new QTreeWidgetItem();
	set_item(ch_item, lt);
	item->addChild(ch_item);
}

void FLZigbeeMonitor::parse_cmd_content(QTreeWidgetItem* item, QDomNode command, QStringList& cmd_list)
{
	

}

void FLZigbeeMonitor::on_comm_msg(int node, QStringList slt, QString cmd)
{
	if (ui.treeWidget_2->topLevelItemCount() > 200)
	{
		ui.treeWidget_2->clear();
	}

	auto cmd_node = rec_comm_list[node];

	QTreeWidgetItem* item;

	if (!OTA_active) {
		item  = new QTreeWidgetItem(ui.treeWidget_2);
		ui.treeWidget_2->addTopLevelItem(item);
		QLabel* top_label = new QLabel("RX: " + cmd, this);
		top_label->setWordWrap(true);
		ui.treeWidget_2->setItemWidget(item, 0, top_label);
		
		
	}
	parse_cmd_content(item, cmd_node, slt);
}

void FLZigbeeMonitor::on_data_msg(QStringList slt, QString cmd)
{
	data2frame(slt);
}

void FLZigbeeMonitor::on_expand()
{
	QPushButton* bt = (QPushButton*)sender();
	if (bt == ui.p_expand_all)
	{
		set_tree_state(ui.treeWidget,bt, !ui.treeWidget->isVisible());		
		if (!ui.treeWidget->isVisible() && !ui.tree_comm_mostused->isVisible())
			set_tree_state(ui.tree_comm_mostused, ui.pb_eapand_mostly, true);
	}
	else
	{
		set_tree_state(ui.tree_comm_mostused, bt, !ui.tree_comm_mostused->isVisible());		
		if (!ui.treeWidget->isVisible() && !ui.tree_comm_mostused->isVisible())
			set_tree_state(ui.treeWidget, ui.p_expand_all,true);
	}
}

void FLZigbeeMonitor::set_tree_state(QTreeWidget * tree, QPushButton * bt, bool visible)
{
	QIcon icon;
	if (visible)
		icon.addFile(QStringLiteral(":/firelink/Resources/icons/play-button.png"), QSize(), QIcon::Normal, QIcon::Off);
	else
		icon.addFile(QStringLiteral(":/firelink/Resources/icons/play-button1.png"), QSize(), QIcon::Normal, QIcon::Off);		
	bt->setIcon(icon);
	tree->setVisible(visible);
}

void FLZigbeeMonitor::data2frame(QStringList lt)
{
	if (lt.count() < 4)
		return;
	int index = ui.tb_monitor->rowCount();
	if (index >200)
	{
		ui.tb_monitor->clear();
		ui.tb_monitor->setRowCount(0);
		index = 0;
		ui.tb_monitor->setHorizontalHeaderLabels(QStringList() << "TimeStamp" << "Channel" << "Length"
			<< "Stack" << "Layer" << "Message" << "MAC Source" << "MAC Destination" << "MAC Sequence" << "Information");
	}

	int col = 0;
	ui.tb_monitor->insertRow(index);
	foreach(auto str, lt)
		ui.tb_monitor->setItem(index, col++, new QTableWidgetItem(str));
	ui.tb_monitor->scrollToBottom();

}

void FLZigbeeMonitor::excute_command(QString cmd)
{
	auto name = cmd;
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
		
		set_item(tree_item, QStringList() << "Len" << "[1 BYTE]" << QString::number(ch_len, 16));

		arr[3] = ch_len;
		arr.append(Checksum(arr, arr.length()));
		m_serial->write(arr);

		

	}
}

void FLZigbeeMonitor::onTimer()
{
	QString address = "0x0000";
	excute_command("NLDE-DATA.Request 0x02 "+ WIOAddress +" 0x02 0x00 0x06 0x06 0x00 0x01 0xA0 0x01");
	_timer.start(50000);
}

void FLZigbeeMonitor::onStartDevice()
{
	
}

void FLZigbeeMonitor::onStopDevice()
{
	
}

void FLZigbeeMonitor::ProcessData(QByteArray arr)
{

}

void FLZigbeeMonitor::onOpenJob()
{


}

void FLZigbeeMonitor::onOpenFW()
{


}

void FLZigbeeMonitor::onCancelFW()
{
	
}

void FLZigbeeMonitor::onWriteFW()
{

}

void FLZigbeeMonitor::onOTA_Timer()
{
	

}

int FLZigbeeMonitor::packQR(QByteArray *arr) {
	static OTA_QueryResponse packet = {

		{ 0x40, 0x08, 0x0019, 0x0109, 0x08, 0xFC },					//APS header	
		{ 0x19, 0x81, 0x02 },										//ZCL header
		{ 0x00, 0x1004, 0x0000, 0x30103010, 0x00000000 }			//ZCL Payload

	};

	packet.zclHeader.transSN = packet_number;
	packet.apsHeader.apsCounter = packet_number;


	packet.zclPayload.imageSize = bytetoRead;

	arr->append((char*)&packet, sizeof(packet));
	return sizeof(packet);
}

int FLZigbeeMonitor::packIN(QByteArray *arr) {
	
	return sizeof(OTA_ImageNotify);
}

int FLZigbeeMonitor::packUER(QByteArray *arr) {
	
	return 0;
}

int FLZigbeeMonitor::packIBR(QByteArray *arr) {

	
	
	return 0;
}

void FLZigbeeMonitor::prepareHeader(QByteArray *arr)
{
	
}

void FLZigbeeMonitor::sendPacket(int type)
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

		m_serial->write(arr);

		packet_number++;
	}
	

}

void FLZigbeeMonitor::nextPacket()
{
	


}

