#include "flserialwidget.h"

#include <QMessageBox>
#include <QSerialPortInfo>

FLSerialWidget::FLSerialWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.serialPortInfoListBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(on_connect()));
	connect(ui.pbFresh, SIGNAL(clicked()), this, SLOT(on_fresh_port()));
	serial = nullptr;
	on_fresh_port();
}

FLSerialWidget::~FLSerialWidget()
{
	if (serial)
	{
		serial->close();
		delete serial;
	}
}

QString FLSerialWidget::get_port_name()
{
	return ui.serialPortInfoListBox->itemData(ui.serialPortInfoListBox->currentIndex()).toString();
}

void FLSerialWidget::set_enable_connect(bool enable)
{
	ui.pushButton_5->setEnabled(enable);
	if (enable)
		ui.pushButton_5->setText("Connect");
	else
		ui.pushButton_5->setToolTip("Input password to enable connect.");
}

void FLSerialWidget::on_connect()
{
	if (serial)
	{
		serial->close();
	}
	serial = new QSerialPort(get_port_name(), this);
	if (true) {


		//serial.setPortName(name);
		serial->setBaudRate(QSerialPort::Baud38400);
		//serial.setBaudRate(info.baudRate);
		//serial.setDataBits(info.dataBits);
		//serial.setParity(info.parity);
		serial->setFlowControl(QSerialPort::NoFlowControl);
	}
	else
	{
		//const SavedPreference::DeviceInfo& info = SavedPreference::instance()->serialInfo();
		//serial.setPortName(name);
		serial->setBaudRate(921600);
		serial->setDataBits(QSerialPort::Data8);
		serial->setParity(QSerialPort::NoParity);
		serial->setFlowControl(QSerialPort::NoFlowControl);
	}

	if (!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Error"), serial->errorString());
		delete serial;
		serial = nullptr;
		return;
	}	
	//ui.pushButton_5->setEnabled(false);
	ui.pushButton_5->setText("Disconnect");
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(on_disconnect()));
	emit state_change(1);
}

void FLSerialWidget::on_disconnect()
{
	if (serial)
	{
		serial->close();
		delete serial;
		serial = nullptr;
	}
	ui.pushButton_5->setText("Connect");	
	connect(ui.pushButton_5, SIGNAL(clicked()), this, SLOT(on_connect()));
	emit state_change(0);
}

void FLSerialWidget::on_fresh_port()
{
	ui.serialPortInfoListBox->clear();
	InitPort();
}

void FLSerialWidget::fillPortsInfo()
{
	int idx = 0;
	QString blankString = "";
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

		ui.serialPortInfoListBox->addItem(list.first(), list);

		// hard cording
		if (info.vendorIdentifier() == 0x403 && info.productIdentifier() == 0x6001)
			idx = i;
	}
	ui.serialPortInfoListBox->addItem(tr("Custom"));
	ui.serialPortInfoListBox->setCurrentIndex(idx);
}

void FLSerialWidget::showPortInfo(int idx)
{
	if (idx == -1)
		return;
	QString blankString = "";
	
	const auto infos = QSerialPortInfo::availablePorts();

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

void FLSerialWidget::InitPort()
{
	QString description;
	QString manufacturer;
	QString serialNumber;
	QString blankString = "No description";
	const auto infos = QSerialPortInfo::availablePorts();
	foreach(const QSerialPortInfo &info, infos)
	{
		if (info.isBusy())
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

		ui.serialPortInfoListBox->addItem(list.first(), info.portName());
	}

}
