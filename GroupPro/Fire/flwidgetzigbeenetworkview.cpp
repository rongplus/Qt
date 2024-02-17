#include "flwidgetzigbeenetworkview.h"
#include <QDebug>
#include <QSettings>
#include <QDate>
#include <QDesktopServices>
#include <QSplitter>
#include <glog\logging.h>

#include "flfloor.h"
#include "flzonecontrollor.h"
#include "Helper.h"
#include "SavedPreference.h"

FLWidgetZigbeeNetworkView::FLWidgetZigbeeNetworkView(QString panid,QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	_pan_id = panid;
	init();		
}

FLWidgetZigbeeNetworkView::FLWidgetZigbeeNetworkView( QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);	
	_pan_id = "NoID";
	init();
}

void FLWidgetZigbeeNetworkView::init()
{
	FLFloor * floor = new FLFloor(0);
	_record = nullptr;
	_save_data = new SavedFrames();
	FLZoneControllor* zc = nullptr;

	{
		zc = new FLZoneControllor(0);
		zc->Init();
		zc->addProperty("PAN ID", _pan_id);
		zc->addProperty("Name", "ZC 0x0000");
		zc->InitDefaultIO();
		floor->addZC(zc);
	}
		
	zc->setProperty("Short Address", "0x0000");		
	ui.widget->setFloor(floor);
	
	ui.widget->setMoveable(false);
	
	add_col(0, "ID");
	add_col(1, "Time Stamp");
	add_col(2, "0xFFFF");
	add_col(3, "0x0000");

	//_fi.setFileName ("e:/log/record.txt");
	//if(_fi.open(QIODevice::ReadWrite) == true)
	//	_record = new QTextStream(&_fi);

	connect(ui.tableWidget, &QTableWidget::cellClicked, this, &FLWidgetZigbeeNetworkView::selectedMACFrameItem);
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_clear_table()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_create_job()));
	connect(ui.btnSearch, SIGNAL(clicked()), this, SLOT(onSearch()));
	connect(ui.pushButton_3, SIGNAL(clicked()), this, SLOT(on_fitview()));

	snifferMain = new SnifferMain(this);
	
	auto sp = new QSplitter(this);
	sp->setOrientation(Qt::Horizontal);

	auto leftsp = new QSplitter(this);
	leftsp->setOrientation(Qt::Vertical);
	leftsp->addWidget(ui.groupBox_2);
	leftsp->addWidget(ui.tableWidget);
	leftsp->addWidget(ui.tabWidget);
/**/
	sp->addWidget(leftsp);
	sp->addWidget(ui.groupBox);

	ui.gridLayout_5->addWidget(sp, 1, 0, 3, 12);
	
}

FLWidgetZigbeeNetworkView::~FLWidgetZigbeeNetworkView()
{
	_fi.close();
}

QString FLWidgetZigbeeNetworkView::pan_id()
{
	return _pan_id;
}


/*
QList<QString>()
<< toNum(idx)                0          // no.
<< sTime                     1         // timestamp
<< toNum(14)			     2		  // channel
<< toNum(data.size())        3         // length
<< sStack                    4        // stack
<< sLayer                    5       // layer
<< sMsg                       6        // message
<< sMacSrc                    7        // mac source
<< sMacDst                    8        // mac destination
<< sMacSeq                    9        // mac sequence
<< sInfo                      10         // information
<< sPanID                     11
<< eMac                       12
*/
void FLWidgetZigbeeNetworkView::process_data(QList<QString> list, QString data)
{
	if (list[7] == "-")
		return;
	
	_mutex.lock();
	fill_table(list, data);
	update_floor(list, data);		
	_mutex.unlock();
}

void FLWidgetZigbeeNetworkView::set_item(int row, int col, QString text, QColor clr, QString lable)
{	
	auto pCell = new QTableWidgetItem;
	pCell->setBackgroundColor(clr);
	ui.tableWidget->setItem(row, col, pCell);
	pCell->setText(text);
}

void FLWidgetZigbeeNetworkView::add_col(int col, QString text)
{	
	if (ui.tableWidget->columnCount() < col + 1)
		ui.tableWidget->setColumnCount(col + 1);
	QTableWidgetItem* item = new QTableWidgetItem(text);
	//item->setIcon(*_icon);
	ui.tableWidget->setHorizontalHeaderItem(col, item);
	_wio_ids.append(text);
	
}

void FLWidgetZigbeeNetworkView::clear_table()
{	
	//ui.tableWidget->clear();
	ui.tableWidget->setRowCount(0);
	int n = 0;
	//for each (auto ttt in _wio_ids)
	{
		//QTableWidgetItem* item = new QTableWidgetItem(ttt);
		//item->setIcon(*_icon);
		//ui.tableWidget->setHorizontalHeaderItem(n++, item);
	}
	//ui.tableWidget->setColumnCount(_wio_ids.count());
	//for (size_t i = ui.tableWidget->rowCount(); i >0; i--)
	{
	//	ui.tableWidget->removeRow(i - 1);
	}
	_wio_data.clear();
	_save_data->clearAll();
}

void FLWidgetZigbeeNetworkView::selectedMACFrameItem(int row, int col)
{
	Q_UNUSED(col)

	int idx = (ui.tableWidget->item(row, 0)->text().toUInt()) - 1;
	addMACFrameDecode(idx);
	ui.dataText->setPlainText(_wio_data[row]);
	ui.tableWidget->selectRow(row);
}

void FLWidgetZigbeeNetworkView::on_clear_table()
{
	_mutex.lock();
	clear_table();
	
	_mutex.unlock();
}

void FLWidgetZigbeeNetworkView::on_create_job()
{
	ui.widget->create_new_job();
}

void FLWidgetZigbeeNetworkView::fill_table(QList<QString> list, QString data)
{
	if (ui.tableWidget->rowCount() > 1000)
		clear_table();

	int index = ui.tableWidget->rowCount();
	_wio_data.append(data);

	ui.tableWidget->insertRow(index);

	QString sTime = list[1];
	QString msg = list[6];
	QString sMac = list[7];
	QString dest = list[8];
	QString seq = list[9];
	QString type = list[10];
	QString lMac = list[12];

	
	auto pCell = new QTableWidgetItem;
	pCell->setText(QString::number(index + 1));
	ui.tableWidget->setItem(index, 0, pCell);

	pCell = new QTableWidgetItem;
	pCell->setText(sTime);
	ui.tableWidget->setItem(index, 1, pCell);

	auto col = _wio_ids.indexOf(sMac);
	if (col == -1)
	{
		add_col(ui.tableWidget->columnCount(), sMac);
	}
	col = _wio_ids.indexOf(sMac);

	ui.labelnWIO->setText("Active WIO Index:" + QString::number(col) +  " Addr: " + sMac);

	if (msg == "Leave")
		set_item(index, col, msg, QColor(255, 0, 255, 0xff), sMac);
	else if (msg == "Association Request")
	{
		sMac = eMacs.key(sMac, "");
		if (sMac.length() == 6)
			set_item(index, _wio_ids.indexOf(sMac), "Tx:" + msg + " " + sMac, QColor(0, 255, 0, 0xff), sMac);
		int dd = _wio_ids.indexOf(dest);
		set_item(index, dd, "Rx:" + msg + " " + sMac, QColor(0, 255, 0, 0xff), dest);
	}
	else if (type != "Acknowledge")
	{
		if (sMac.length() == 6)
		{
			if (dest != "0xFFFF")
				set_item(index, col, "Tx: " + type + " Seq: " + seq, QColor(255, 0, 0, 0x50), sMac);
			else
			{
				if (type.contains("LinkStatus"))
				{
					qDebug() << "Links";
				}
				else
					set_item(index, col, "Bc: " + type + " Seq: " + seq, QColor(0, 0, 255, 0x50), sMac);
			}

		}

		if (dest.length() == 6)
			set_item(index, col, "Rx: " + type, QColor(0, 255, 0, 0x50), dest);
	}
	else
		set_item(index, 2, "Rx: ACK", QColor(0, 255, 0, 0x50), "0xFFFF");

	if (scrollSwitch)
		ui.tableWidget->scrollToBottom();
}

void FLWidgetZigbeeNetworkView::update_floor(QList<QString> list, QString data)
{
	//todo find wio from floor
	QString msg = list[6];
	QString sMac = list[7];
	QString dest = list[8];
	QString type = list[10];
	QString lMac = list[12];
	QString FType = list[13];

	if (sMac == "" && lMac == "")
		return;

	//todo
	//step 1: ZC or WIO
	//step 2: broad cast or not
	//step 3: status
	

	DeviceStatus d_status;
	auto zc = find_zc("");
	if (zc == nullptr)
		return;
	FLObject * obj = nullptr;
	if(sMac == "0x0000")
		obj = zc;
	else
	{
		auto wio = find_wio(sMac, lMac, ui.widget->Floor());
		if (wio == nullptr && sMac != "")
		{
			if (zc == nullptr)
				return;
			wio = new FLWio(0);
			wio->Init();
			wio->setProperty("Short Address", sMac);
			wio->addProperty("Name", sMac);
			wio->setMacAddress(lMac);
			//zc->addWio(wio);
			ui.widget->AddWIOToFloor(zc, wio, 0);
			_dongle_wio.append(wio);
		}
		obj = wio;
	}
		
	if (obj == nullptr)
		return;

	if (msg == "Association Request")
		d_status = DeviceStatus::Device_AssociationRequest;
	else if (msg.contains("Link Status"))
		d_status = DeviceStatus::Device_LinkStatus;
	else if (msg == "Beacon")
		d_status = DeviceStatus::Device_BC;
	else if (msg == "Leave")
		d_status = DeviceStatus::Device_Leave;
	else
		{
			if(FType == "MAC")
				d_status = DeviceStatus::Device_MAC;
			else if (FType == "NWK")
				d_status = DeviceStatus::Device_NWK;
			else if (sMac.size() == 6)
				d_status = DeviceStatus::Device_Tx;
		}

	
	ui.widget->activeDevice(obj, 10, sMac, d_status, lMac, sMac);
	if (dest != "0xFFFF" && dest != "0xffff")
	{
		auto dest_device = find_wio(dest, lMac, ui.widget->Floor());
		//Active(dest, DeviceStatus::Device_Rx); // activare receiver 
		if(dest_device)
			ui.widget->activeDevice(dest_device, 10, sMac, d_status, lMac, sMac);
	}	

	if(d_status == DeviceStatus::Device_LinkStatus)
	{
		//todo link status
		auto lt = type.split(",");
		QRegExp numer_exp("(\\d+)");
		int pos;
		if( numer_exp.indexIn(lt[0], pos) != -1)
			numer_exp.cap(1);
		for (size_t i = 1; i < lt.size(); i++)
		{
			pos = 0;
			auto d_device = lt[i].split(" ", QString::SkipEmptyParts);
			QList<int> list;
			while ((pos = numer_exp.indexIn(d_device[1], pos)) != -1) {
				list << numer_exp.cap(1).toInt();
				pos += numer_exp.matchedLength();
			}
			if( list.size() >=2)
				ui.widget->link_wio(d_device[0], list[0], sMac, list[1]);
		}
		
	}
}

void FLWidgetZigbeeNetworkView::addMACFrameDecode(int idx)
{
	ui.frameDecodeView->clear();		
	//const MACFrame& frame = _save_data->macFrame(idx);
	//snifferMain->addMACFrameDecode(frame, ui.frameDecodeView);

	MACFrame& frame = MACFrame(_save_data->frame(idx + 1).bin);
	
	snifferMain->addMACFrameDecode(frame, ui.frameDecodeView);

}

FLZoneControllor * FLWidgetZigbeeNetworkView::find_zc(QString mac)
{
	auto fl = ui.widget->Floor();
	auto fl_count = fl->countZC();
	for (size_t i = 0; i < fl_count; i++)
	{
		auto zc = fl->getZC(i);
		qDebug() << "pan id" << zc->getProperty("PAN ID").value.toString();
		if (zc->getProperty("PAN ID").value.toString().right(4) == _pan_id.right(4))
		{
			return zc;
		}
	}
	return nullptr;
}

FLWio * FLWidgetZigbeeNetworkView::find_wio(QString mac1, QString mac2, FLFloor* fl)
{
	auto fl_count = fl->countZC();
	for (size_t i = 0; i < fl_count; i++)
	{
		auto zc = fl->getZC(i);
		auto wio_count = zc->countWio();
		for (size_t j = 0; j < wio_count; j++)
		{
			auto wio = zc->getWio(j);
			auto sM = wio->property("Short Address").toString();
			if (sM  == mac1)
				return wio;
			//if (wio->MacAddress() == mac2 && mac2 != "FF:FF:FF:FF:FF:FF:FF:FF")
			//	return wio;
			//wio->setProperty("Short Address", ShrtAdd);
		}
	}
	return nullptr;
}

ZBInt32 FLWidgetZigbeeNetworkView::addFrame(ZBByte type, const ZBBytes& bin)
{
	return _save_data->addFrame(type, bin);	
}

void FLWidgetZigbeeNetworkView::set_floor(FLFloor * floor)
{
	//todo add existed wio to floor
	
	auto fl = floor;
	auto fl_count = fl->countZC();
	bool bFound = false;

	for each (auto obj in _dongle_wio)
	{
		auto wio = find_wio( obj->property("Short Address").toString(), obj->MacAddress(), floor);
		if (wio == nullptr)
		{
			//todo add to floor
			floor->getZC(0)->addWio(obj);
		}
	}		
	ui.widget->setFloor(floor);
	ui.label_job->setText(floor->getProperty("Name").value.toString());
	//ui.widget->fit_window();
}

void FLWidgetZigbeeNetworkView::snapshot(QString name)
{
	QSettings st("Mircom", "FireLink");
	QString str = st.value("Default_PCAP_Path").toString();

	if (str.isNull()) {
		//onSaveDir();
		str = "";
	}

	QString text = QDate::currentDate().toString("yyyy-MM-dd") + "-" + QTime::currentTime().toString().replace(":", ".");

	const QString htmlFileName = QString("%1/%2").arg(str).arg(text + "_network.html");
	const QString jpgFileName = QString("%1/%2").arg(str).arg(text + "_network.jpg");
	// make a html-dump of table view

	QTableWidget *table = ui.tableWidget;

	if (table) {
		QFile file(htmlFileName);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			//MSG(QString("Can`t create file %1").arg(htmlFileName));
			//return false;
		}

		QTextStream out(&file);

		const int rowCount = table->model()->rowCount();
		const int columnCount = table->model()->columnCount();

		QPixmap pixMap = QPixmap::grabWidget(ui.widget);
		

		QFile imgfile(jpgFileName);
		imgfile.open(QIODevice::WriteOnly);
		pixMap.save(&imgfile, "JPG");
		imgfile.close();


		out << "<html>\n"
			"<head>\n"
			"<meta Content=\"Text/html; charset=Windows-1251\">\n"
			<< QString("<title>%1</title>\n").arg("Network View")
			<< "</head>\n"
			"<body bgcolor=#ffffff link=#5000A0>\n"
			"<img src=\'"
			<< text
			<< "_network.jpg\'/>\n"
			"<table border=1 cellspacing=0 cellpadding=2>\n";

		// headers
		out << "<tr bgcolor=#f0f0f0>";
		for (int column = 0; column < columnCount; column++)
			if (!table->isColumnHidden(column))
			{
				QString text = table->model()->headerData(column, Qt::Horizontal).toString();
				text = eMacs.value(text, "");
				out << QString("<th>%1<p style='font-size:10px'>%2</p></th>").arg(text).arg(text);
			}
		out << "</tr>\n";
		file.flush();

		// data table
		for (int row = 0; row < rowCount; row++) {
			out << "<tr>";
			for (int column = 0; column < columnCount; column++) {
				if (!table->isColumnHidden(column)) {
					QString col = "";
					if (table->item(row, column))
					{
						QColor c = table->item(row, column)->background().color();
						col = c.name();
						if (col != "#ffffff")
							col = "style=\'background-color:" + col + ";\'";
					}
					QString data = table->model()->data(table->model()->index(row, column)).toString().simplified();


					out << QString("<td  %1 %3>%2</td>").arg(col).arg((!data.isEmpty()) ? data : QString("&nbsp;")).arg((!data.isEmpty()) ? "onclick='alert(\"" + _wio_data[row] + "\")'" : "");
				}
			}
			out << "</tr>\n";
		}
		out << "</table>\n"
			"</body>\n"
			"</html>\n";

		file.close();
	}

	bool res = QDesktopServices::openUrl(QUrl::fromLocalFile(htmlFileName));
}

void FLWidgetZigbeeNetworkView::hideCoulmns(bool hide)
{
	for (int j = 4; j < ui.tableWidget->columnCount(); ++j)
	{
		ui.tableWidget->setColumnHidden(j, hide);
	}
}

void FLWidgetZigbeeNetworkView::onSearch()
{
	QString filter = ui.textSearch->text();

	if (filter == "")
	{
		hideCoulmns(false);
		return;
	}
	else if (filter.length() == 6)
	{
		hideCoulmns(true);
		int index = _wio_ids.indexOf(filter);
		if (index >= 0)
			ui.tableWidget->setColumnHidden(index, false);
	}
	else if (filter.length() == 23)
	{
		hideCoulmns(true);
		QList<QString> list = eMacs.keys(filter);

		for (int i = 0; i< list.size(); i++)
		{
			int index = _wio_ids.indexOf(list[i]);
			if (index >= 0)
				ui.tableWidget->setColumnHidden(index, false);

		}
	}	
}

void FLWidgetZigbeeNetworkView::on_fitview()
{
	ui.widget->fit_window();
}
