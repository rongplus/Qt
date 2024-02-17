#include "flnetview.h"
#include <QSettings>
#include <QDate>
#include <QTime>
#include <QDesktopServices>

flnetview::flnetview(SnifferMain *sm, QWidget *parent)
	: QWidget(parent), snifferMain(sm)
{
	ui.setupUi(this);

	ui.labelNever->setStyleSheet("QLabel { background-color : #00ffff}");
	ui.labelLeft->setStyleSheet("QLabel { background-color : #ff00ff}");
	ui.labelJoined->setStyleSheet("QLabel { background-color : #00ff00}");
	ui.labelRecent->setStyleSheet("QLabel { background-color : #ff0000}");
	ui.tableWidget->setStyleSheet("QHeaderView::section { border-color:black }");

	_b_fromfile = false;
	itemIndex = 0;
	_bInit = false;
	

	sp = new QSplitter(this);
	sp->setOrientation(Qt::Horizontal);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(sp);

	leftsp = new QSplitter(this);
	leftsp->setOrientation(Qt::Vertical);

	leftsp->addWidget(ui.tableWidget);
	leftsp->addWidget(ui.tabWidget);
	
	QSplitter *btsplitter = new QSplitter(this);

	btsplitter->addWidget(ui.frameDecodeView);
	btsplitter->addWidget(ui.dataText);
	btsplitter->setOrientation(Qt::Horizontal);


	ui.tabWidget->addTab(btsplitter, "MAC Frame");

	ui.tabWidget->setTabPosition(QTabWidget::South);

	FLMessageView *mv = new FLMessageView(this);

	ui.tabWidget->addTab(mv, "Packet");

	sp->addWidget(leftsp);

	pfloorView = new FLFloorView(sp);	

	FLFloor * floor = new FLFloor(0);

	FLZoneControllor* zc = new FLZoneControllor(0);
	zc->Init();
	zc->InitDefaultIO();
	zc->addProperty("Name", "Unnamed ZC");
	floor->addZC(zc);

	pfloorView->setFloor(floor);
	

	pfloorView->setMoveable(false);

	
	//FLObjectPropertys p1 = node->getProperty("Name");
	sp->addWidget(pfloorView);

	ui.gridLayout->addWidget(sp, 1, 0, 3, 2);

	ui.tableWidget->clear();
	ui.tableWidget->setRowCount(0);
	
	nList.append("Id");
	nList.append("Time Stamp");
	nList.append("0xFFFF");
	nList.append("0x0000");
	
	ui.tableWidget->setColumnCount(nList.length());
	ui.tableWidget->setHorizontalHeaderLabels(nList);
	
	QTableWidgetItem* item = new QTableWidgetItem("0x0000");
	item->setIcon(*(new QIcon(":/firelink/Resources/icons/zc-icon-48.png")));
	ui.tableWidget->setHorizontalHeaderItem(3, item);

	connect(ui.tableWidget->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(hheaderclicked(int)));
	connect(ui.tableWidget, &QTableWidget::cellClicked, this, &flnetview::selectedMACFrameItem);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_create_job()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_clear_table()));
	scrollSwitch = false;
	
	int w = width();

	sp->setSizes(QList<int>() << w*0.5 << w*0.5);

	
}

flnetview::~flnetview()
{

}


void flnetview::selectedMACFrameItem(int row, int col)
{
	Q_UNUSED(col)
		int idx = (ui.tableWidget->item(row, 0)->text().toUInt()) - 1;
	addMACFrameDecode(idx);
	ui.dataText->setPlainText(sdata[row]);
	ui.tableWidget->selectRow(row);
}

void flnetview::resizeEvent(QResizeEvent * event)
{
	QWidget::resizeEvent(event);
	int w = width();
/*
	sp->setSizes(QList<int>() << w*0.5 << w*0.5);

	int h = height(); 
	leftsp->setSizes(QList<int>() << h*0.7 <<  h*0.3);*/
	qDebug() << "resize";
}


void flnetview::insertItem(QList<QString> list, QString data)
{	

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
	_mutex.lock();
	if (  ui.tableWidget->rowCount() > 1000)
	{
		for (size_t i = ui.tableWidget->rowCount(); i >0; i--)
		{
			ui.tableWidget->removeRow(i - 1);
		}
		itemIndex = 0;
		sdata.clear();
	}

	int sIndex;//dest = dest.toUpper();
	//src = src.toUpper();
		
	int index = itemIndex++;
	sdata.append(data);

	ui.tableWidget->insertRow(index);
	//ui.tableWidget->setRowCount(index);

	QString sTime = list[1];
	QString msg = list[6];
	QString src = list[7];
	QString dest = list[8];
	QString seq = list[9];
	QString type = list[10];
	QString lMac = list[12];

	insertCell("Id", QString::number(index + 1), index, QColor(255, 255, 255, 0x50));
	insertCell("Time Stamp", sTime, index, QColor(255, 255, 255, 0x50));

	if (1)//dest.mid(2).toUpper() != "FFFF")
	{
		if (msg == "Leave")
			sIndex = insertCell(src, msg, index, QColor(255, 0, 255, 0xff));
		if (msg == "Association Request") 
		{
			src = eMacs.key(src, "");
			if(src.length() == 6)
				sIndex = insertCell(src, "Tx:" + msg + " " + src, index, QColor(0, 255, 0, 0xff));

			insertCell(dest, "Rx:"+ msg + " " + src, index, QColor(0, 255, 0, 0xff));
		}
		/*else if( msg == "Network Status")
		{

		}*/
		else if(type != "Acknowledge")
		{
			if (src.length() == 6)
			{
				if (dest != "0xFFFF")
					sIndex = insertCell(src, "Tx: " + type + " Seq: " + seq, index, QColor(255, 0, 0, 0x50));
				else
				{
					if (type.contains("LinkStatus"))
					{
						qDebug() << "Links";
					}
					else

						sIndex = insertCell(src, "Bc: " + type + " Seq: " + seq, index, QColor(0, 0, 255, 0x50));
				}

			}

			if (dest.length() == 6)
				insertCell(dest, "Rx: " + type, index, QColor(0, 255, 0, 0x50));
		}
		else
			insertCell("0xFFFF", "Rx: ACK", index, QColor(0, 255, 0, 0x50));
	}
	

	if (list[12] != "") 
	{
		QString value = list[12];
		QString key = eMacs.key(value, "");
		qDebug() << "eMAC: " << src << value << key;

		if (value.contains("00:72:72:00:00:00:00:24"))
			value = value;
		if (key != src)
		{
			//eMacs.remove(key);
			int index = nList.indexOf(src);

			int i = index;
			if (i != -1)
			{
				QTableWidgetItem* item = new QTableWidgetItem(nList[i]);
				if (i == 3)
					item->setIcon(*(new QIcon(":/firelink/Resources/icons/zc-icon-48.png")));
				else
					item->setIcon(*(new QIcon(":/firelink/Resources/icons/wio-icon-48.png")));
				item->setToolTip(value);
				ui.tableWidget->setHorizontalHeaderItem(i, item);
			}
			

			qDebug() << "Insert: " << nList[i] << value;
		}
		if (value != "")
			eMacs[src] = value;
	}
	

	if (sIndex >= 4 && eMacs.contains(src))
	{
		QString macAddress = eMacs[src];// QString("FF:FF:FF:FF:FF:FF:FF:%1").arg(eMacs[src].mid(21, 2));

		qDebug() << src << macAddress;

		if (msg == "Leave")
			pfloorView->activeDevice(macAddress, 10, src, msg, QColor(255,0, 250), QColor(255, 0, 250));
		if (type.contains("Beacon"))
			pfloorView->activeDevice(macAddress, 10, src, type, QColor(255, 195, 0), QColor(255, 195, 0));
		else
			pfloorView->activeDevice(macAddress, 10, src, type, Qt::red);
	}
	else if (msg == "Association Request")
	{
		pfloorView->activeDevice(src, 10, msg+ " " +src, msg, Qt::green);		
	}

	
	if (scrollSwitch)
		ui.tableWidget->scrollToBottom();
	_mutex.unlock();

}

void  flnetview::clear_table()
{
	_mutex.lock();
	for (size_t i = ui.tableWidget->rowCount(); i >0; i--)
	{
		ui.tableWidget->removeRow(i - 1);
	}
	itemIndex = 0;
	_mutex.unlock();
}

void flnetview::print_table(QString name)
{
	QSettings st("Mircom", "FireLink");
	QString str = st.value("Default_PCAP_Path").toString();

	if (str.isNull()) {
		//onSaveDir();
		str ="";
	}

	QString text = QDate::currentDate().toString("yyyy-MM-dd") + "-" + QTime::currentTime().toString().replace(":", ".");

	const QString htmlFileName = QString("%1/%2").arg(str).arg(text + "_network.html");
	const QString jpgFileName = QString("%1/%2").arg(str).arg(text + "_network.jpg");
	// make a html-dump of table view

	QTableWidget *table = getTable();
	
	if (table) {
		QFile file(htmlFileName);

		if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			//MSG(QString("Can`t create file %1").arg(htmlFileName));
			//return false;
		}

		QTextStream out(&file);

		const int rowCount = table->model()->rowCount();
		const int columnCount = table->model()->columnCount();

		QPixmap pixMap = QPixmap::grabWidget(getFloorView());
		/*
		QImage image(qApp->applicationDirPath()+"/network.jpg");
		QPainter painter(&image);
		painter.setRenderHint(QPainter::Antialiasing);
		pfloorView->render(&painter);
		image.save("file_name.png");*/

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
				out << QString("<th>%1<p style='font-size:10px'>%2</p></th>").arg(text).arg(geteMac(text));
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


					out << QString("<td  %1 %3>%2</td>").arg(col).arg((!data.isEmpty()) ? data : QString("&nbsp;")).arg((!data.isEmpty()) ? "onclick='alert(\"" + getsData()[row] + "\")'" : "");
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

void flnetview::set_table_rowcount(int nRow)
{
	getTable()->setRowCount(nRow);
}


int flnetview::insertCell(QString text, QString type, int i, QColor col)
{
	
	int sIndex = nList.indexOf(text);

	if (sIndex == -1)
	{
		
		nList.append(text);
		sIndex = nList.indexOf(text);

		ui.labelnWIO->setText(QString("WIOs with distinct Adrs: %1").arg(sIndex - 3));

		ui.tableWidget->setColumnCount(nList.length());
		//ui.tableWidget->setHorizontalHeaderLabels(nList);
		
		QTableWidgetItem* item = new QTableWidgetItem(text);
		auto icon = QIcon(":/firelink/Resources/icons/wio-icon-48.png");
		item->setIcon(icon);
		ui.tableWidget->setHorizontalHeaderItem(sIndex, item);
		item->setToolTip(eMacs[text]);
	}
	
	

	QTableWidgetItem *pCell = ui.tableWidget->item(i, sIndex);

	if (!pCell)
	{
		pCell = new QTableWidgetItem;
		pCell->setBackgroundColor(col);
		ui.tableWidget->setItem(i, sIndex, pCell);
	}
	
	pCell->setText(type);

	return sIndex;
}


void flnetview::addMACFrameDecode(int idx)
{
	ui.frameDecodeView->clear();

	if (_b_fromfile)
	{
		MACFrame& frame = MACFrame(SavedFrames::instance()->frame(idx + 1).bin);
		snifferMain->addMACFrameDecode(frame, ui.frameDecodeView);
	}
	else
	{
		const MACFrame& frame = SavedFrames::instance()->macFrame(idx);
		snifferMain->addMACFrameDecode(frame, ui.frameDecodeView);
	}
	//const MACFrame& frame(SavedFrames::instance()->ztcFrame(idx).payload());
	
}

void flnetview::hideCoulmns(bool hide)
{
	for (int j = 4; j < ui.tableWidget->columnCount(); ++j)
	{
		ui.tableWidget->setColumnHidden(j, hide);
	}
}

void flnetview::showEvent(QShowEvent * event)
{
	if(_bInit == false)
		pfloorView->reset_rect(pfloorView->rect());

}

void flnetview::onSearch()
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
		int index = nList.indexOf(filter);
		if (index >= 0)
			ui.tableWidget->setColumnHidden(index, false);
	}
	else if (filter.length() == 23)
	{
		hideCoulmns(true);
		QList<QString> list = eMacs.keys(filter);

		

		for (int i=0; i< list.size(); i++)
		{
			int index = nList.indexOf(list[i]);

			if (index >= 0)
				ui.tableWidget->setColumnHidden(index, false);

			
		}
	}

	
	
	/*
	for (int i = 0; i < ui.tableWidget->rowCount(); ++i)
	{
		bool match = false;
		for (int j = 0; j < ui.tableWidget->columnCount(); ++j)
		{
			QTableWidgetItem *item = ui.tableWidget->item(i, j);
			if (item->text().contains(filter))
			{
				match = true;
				break;
			}
		}
		ui.tableWidget->setRowHidden(i, !match);
	}*/
}

void flnetview::hheaderclicked(int index)
{
	if (index > 3) {
		QString strIP = eMacs[nList[index]];
		//QToolTip::showText(ui.tableWidget->horizontalHeaderItem(index)->mapToGlobal(QPoint(0, 0)), strIP);
	}

}

void flnetview::on_create_job()
{
	pfloorView->create_new_job();
}

void flnetview::on_clear_table()
{
	_mutex.lock();
	for (size_t i = ui.tableWidget->rowCount(); i >0; i--)
	{
		ui.tableWidget->removeRow(i - 1);
	}
	itemIndex = 0;
	sdata.clear();
	SavedFrames::instance()->clearAll();
	_mutex.unlock();
}

QString flnetview::geteMac(QString src)
{ 
		return eMacs.value(src, ""); 
}