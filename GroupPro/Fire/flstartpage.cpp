#include "flstartpage.h"
#include <QSettings>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QFile>
#include <QMessageBox>
#include <QDesktopWidget>

FLStartPage::FLStartPage(QWidget *parent)
	: QWidget(parent)
{
	int N_WIDTH = 168;
	int N_HEIGHT = 168;

	ui.setupUi(this);
	QPixmap p = QPixmap(QStringLiteral(":/firelink/Resources/graphics/logo.png")).scaled(width(), 100, Qt::IgnoreAspectRatio, Qt::FastTransformation );
	ui.imgLogo->setPixmap(p);

	p = QPixmap(":/firelink/Resources/icons/file-manager-icon.png").scaled(N_WIDTH, N_HEIGHT);
	ui.imgLabel_1->setPixmap(p);

	p = QPixmap(":/firelink/Resources/icons/terminal.png").scaled(N_WIDTH, N_HEIGHT);
	ui.imgLabel_2->setPixmap(p);

	p = QPixmap(":/firelink/Resources/icons/zigbee-test-icon.png").scaled(N_WIDTH, N_HEIGHT);
	ui.imgLabel_3->setPixmap(p);

	p = QPixmap(":/firelink/Resources/icons/network.png").scaled(N_WIDTH, N_HEIGHT);
	ui.imgLabel_4->setPixmap(p);

	p = QPixmap(":/firelink/Resources/icons/wireless.png").scaled(N_WIDTH, N_HEIGHT);
	ui.imgLabel_5->setPixmap(p);



	p = QPixmap(":/firelink/Resources/icons/wifi-4.png").scaled(N_WIDTH, N_HEIGHT);
	ui.imgLabel_6->setPixmap(p);

	p = QPixmap(":/firelink/Resources/icons/OTA_logo_400x400.png").scaled(N_WIDTH, N_HEIGHT);
	ui.imgLabel_7->setPixmap(p);

	connect(ui.btnSS,SIGNAL(clicked()), this, SLOT(onSS()));

	connect(&_timer, SIGNAL(timeout()), this, SLOT(update_port()));
	_timer.start(2000);

	connect(ui.listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_jobitemDoubleClicked(QListWidgetItem*)));
	connect(ui.listWidget_2, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_usb_itemDoubleClicked(QListWidgetItem*)));
	connect(ui.listWidget_3, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_Zigbee_debug_itemDoubleClicked(QListWidgetItem*)));
	connect(ui.listWidget_4, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_zigbee_network_itemDoubleClicked(QListWidgetItem*)));
	connect(ui.listWidget_5, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_wireless_itemDoubleClicked(QListWidgetItem*)));
	connect(ui.listWidget_6, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(on_sit_survey_itemDoubleClicked(QListWidgetItem*)));

	init();

	connect(ui.btnOTA, SIGNAL(clicked()), this, SLOT(onOTA()));

	/*FlowLayout* lay = new FlowLayout(this);
	
	QDesktopWidget*desk = QApplication::desktop();
	ui.imgLogo->setMinimumWidth( desk->screenGeometry().width());
	
	lay->addWidget(ui.imgLogo);
	auto list = findChildren<QGroupBox*>();
	for each (auto g in list)
	{
		lay->addWidget(g);
	}
	
	setLayout(lay);*/
}

FLStartPage::~FLStartPage()
{

}

void FLStartPage::resizeEvent(QResizeEvent * event)
{
}


void FLStartPage::on_jobitemDoubleClicked(QListWidgetItem *item)
{
	QString filename = item->text();
	if (filename == "")
		return ;
	QSettings st("Mircom", "FireLink");
	auto job_list = st.value("Recently_Job_Path").toStringList();
	QFile fi(filename);
	if (fi.exists() == false)
	{
		QMessageBox::warning(nullptr, "Error Job file", "Please choose right job file.");
		job_list.removeOne(filename);
		st.setValue("Recently_Job_Path", job_list);
		ui.listWidget->takeItem( ui.listWidget->currentRow());		
		return;
	}

	emit openJob(item->text());
}

void FLStartPage::on_usb_itemDoubleClicked(QListWidgetItem *item)
{
	_mutex.lock();
	emit usbTerminal(item->text());		
	_mutex.unlock();
	init();
}

void FLStartPage::on_Zigbee_debug_itemDoubleClicked(QListWidgetItem *item)
{
	_mutex.lock();
	emit zigBeeTerminal(item->text());	
	_mutex.unlock();
	init();
}

void FLStartPage::on_zigbee_network_itemDoubleClicked(QListWidgetItem *item)
{
	_mutex.lock();
	emit networkMonitor(item->text(),ui.channelBox->currentIndex());	
	_mutex.unlock();
	init();
}

void FLStartPage::on_wireless_itemDoubleClicked(QListWidgetItem *item)
{
	_mutex.lock();
	emit wirelessMonitor(item->text());	
	_mutex.unlock();
	init();
}

void FLStartPage::on_sit_survey_itemDoubleClicked(QListWidgetItem *item)
{
	_mutex.lock();
	emit openSS(item->text());	
	_mutex.unlock();
	init();
}

void FLStartPage::update_port()
{
	_mutex.lock();
	init();
	_mutex.unlock();
}

void FLStartPage::onSS()
{
	emit openSS("");	
}

void FLStartPage::onOTA()
{
	emit ota();
}

void FLStartPage::onWM(void)
{
	emit wirelessMonitor("");
}

void FLStartPage::onNM(void)
{
	emit networkMonitor("",0);
}

void FLStartPage::onZDT(void)
{
	emit zigBeeTerminal("");
}

void FLStartPage::onUDT(void)
{
	emit usbTerminal("");
}

void FLStartPage::onCJ(void)
{
	emit createJob();
}

void FLStartPage::onOJ(void)
{
	emit openJob("");
}

void FLStartPage::init()
{
	QSettings st("Mircom", "FireLink");
	auto job_list = st.value("Recently_Job_Path").toStringList();
	
	ui.listWidget->clear();
	ui.listWidget_2->clear();
	ui.listWidget_3->clear();
	ui.listWidget_4->clear();
	ui.listWidget_5->clear();
	ui.listWidget_6->clear();

	ui.listWidget->addItems(job_list);
	

	QString description;
	QString manufacturer;
	QString serialNumber;
	QString blankString = "No description";
	const auto infos = QSerialPortInfo::availablePorts();
	foreach(const QSerialPortInfo &info, infos)
	{		
		if (info.isValid() == false || info.isBusy() )
			continue;
		ui.listWidget_2->addItem( info.portName());
		ui.listWidget_3->addItem(info.portName());
		ui.listWidget_4->addItem(info.portName());
		ui.listWidget_5->addItem(info.portName());
		ui.listWidget_6->addItem(info.portName());
	}
	ui.channelBox->clear();
	for (int i = 11; i <= 26; i++)
		ui.channelBox->addItem(tr("%1").arg(i));
}