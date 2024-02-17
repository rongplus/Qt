#include "flwidgetpcapview.h"
#include <QFileDialog>
#include "Helper.h"

FLWidgetPCAPView::FLWidgetPCAPView(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	snifferMain = new SnifferMain();
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_open_file()));
}

FLWidgetPCAPView::~FLWidgetPCAPView()
{

}

void FLWidgetPCAPView::on_analysis_file_start()
{
	if (reader->isPaused())
		reader->resumeReader();
	else
		reader->start();
	ui.pushButton_2->setIcon(QIcon(":/firelink/Resources/icons/pause.png"));
	ui.pushButton_2->disconnect();
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_analusis_file_pause()));
}

void FLWidgetPCAPView::on_analusis_file_pause()
{
	reader->pauseReader();
	ui.pushButton_2->disconnect();
	ui.pushButton_2->setIcon(QIcon(":/firelink/Resources/icons/play-button.png"));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_analysis_file_start()));
}

void FLWidgetPCAPView::on_open_file()
{
	//dialog.setNameFilter(tr("PACP (*.pcap)"));
	QString str_file_name = QFileDialog::getOpenFileName(this, "Choose a PCAP File ", "", tr("All PACP (*.pcap)"));
	if (str_file_name.isNull())
		return;


	ui.label->setText("File Name: \r\n" + str_file_name.mid(str_file_name.lastIndexOf('/')));

	reader = new PCAPReader(str_file_name, this);

	//auto res = connect(reader, &PCAPReader::sendPacket, this, &NetworkView::ProcessPacket);

	auto res = connect(reader, SIGNAL(sendPacket(QByteArray, ZBTime)), this, SLOT(ProcessPacket(QByteArray, ZBTime)));

	res = connect(reader, SIGNAL(Finished(int)), this, SLOT(readFinished(int)));

	//readPCAP(str_file_name);

	QDir d = QFileInfo(str_file_name).absoluteDir();
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_analysis_file_start()));

}

void FLWidgetPCAPView::readFinished(int packets)
{
	QMessageBox::critical(this, "File read", QString("Read complete total %1 packets").arg(packets));
}


void FLWidgetPCAPView::ProcessPacket(QByteArray arr, ZBTime zt)
{
	ZBBytes bytes = toBytes(arr);

	QString data = "";

	for (int i = 0; i< bytes.size(); i++)
		data += toHex(bytes[i]) + " ";
	
	QList<QString> list = snifferMain->getTrafficItem(0, bytes, zt);
	ui.widget->process_data(list, data);
	//addItem(list, data, panID);

	//if (list[12] != "")
	//	list[12] = list[12];
	//ZBInt32 idx = SavedFrames::instance()->addFrame(ZB_RX, bytes);
}