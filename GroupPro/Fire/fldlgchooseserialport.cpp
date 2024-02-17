#include "fldlgchooseserialport.h"

#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMessageBox>
#include "flzcserialport.h"
#include <QDebug>

FLDlgChooseSerialPort::FLDlgChooseSerialPort(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton,SIGNAL(clicked()),this, SLOT(On_OK()));
	connect(ui.pushButton_2,SIGNAL(clicked()),this, SLOT(On_Cancel()));

	connect(ui.serialPortInfoListBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showPortInfo(int)));	
	// fill infomation
	fillPortsParameters();
	//fillPortsInfo();
	InitPort();
	// fill current parameters
	updateParameters();
	_data = nullptr;
	_type = 0;

}

FLDlgChooseSerialPort::~FLDlgChooseSerialPort()
{

}

void FLDlgChooseSerialPort::showPortInfo(int idx)
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

void FLDlgChooseSerialPort::InitPort()
{
	QString description;
	QString manufacturer;
	QString serialNumber;
	QString blankString = "No description";
	const auto infos = QSerialPortInfo::availablePorts();
	foreach(const QSerialPortInfo &info , infos)
	{
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

		ui.serialPortInfoListBox->addItem(list.first() , info.portName());
	}

	ui.serialPortInfoListBox->addItem(tr("Custom"));
}

void FLDlgChooseSerialPort::On_OK()
{	
	if (_type == 0)
		send_zc_data();
}


void FLDlgChooseSerialPort::onReadyRead_zc()
{
	
	
}

void FLDlgChooseSerialPort::On_Cancel()
{
	QDialog::reject();
}

QString FLDlgChooseSerialPort::get_port_name()
{
	return ui.serialPortInfoListBox->itemData( ui.serialPortInfoListBox->currentIndex() ).toString();
}

QString FLDlgChooseSerialPort::get_port_fullname()
{
	return ui.serialPortInfoListBox->currentText();
}

void FLDlgChooseSerialPort::set_datatype(int type)
{
	_type = type;
}

void FLDlgChooseSerialPort::set_data(QByteArray & data)
{
	_data = &data;
}



void FLDlgChooseSerialPort::On_Update()
{
	ui.serialPortInfoListBox->clear();
	InitPort();
}




void FLDlgChooseSerialPort::fillPortsParameters()
{
	/*
	for (int i = 11; i <= 26; i++)
		ui.channelBox->addItem(tr("%1").arg(i));
	ui.channelBox->setCurrentIndex(4);

	ui.baudRateBox->addItem("9600", QSerialPort::Baud9600);
	ui.baudRateBox->addItem("19200", QSerialPort::Baud19200);
	ui.baudRateBox->addItem("38400", QSerialPort::Baud38400);
	ui.baudRateBox->addItem("115200", QSerialPort::Baud115200);
	ui.baudRateBox->addItem("921600", 921600);
	ui.baudRateBox->addItem("Custom");
	ui.baudRateBox->setCurrentIndex(4);

	ui.dataBitsBox->addItem("5", QSerialPort::Data5);
	ui.dataBitsBox->addItem("6", QSerialPort::Data6);
	ui.dataBitsBox->addItem("7", QSerialPort::Data7);
	ui.dataBitsBox->addItem("8", QSerialPort::Data8);
	ui.dataBitsBox->setCurrentIndex(3);

	ui.parityBox->addItem("None", QSerialPort::NoParity);
	ui.parityBox->addItem("Even", QSerialPort::EvenParity);
	ui.parityBox->addItem("Odd", QSerialPort::OddParity);
	ui.parityBox->addItem("Mark", QSerialPort::MarkParity);
	ui.parityBox->addItem("Space", QSerialPort::SpaceParity);

	ui.stopBitsBox->addItem("1", QSerialPort::OneStop);
#ifdef Q_OS_WIN
	ui.stopBitsBox->addItem("1.5", QSerialPort::OneAndHalfStop);
#endif
	ui.stopBitsBox->addItem("2", QSerialPort::TwoStop);

	ui.flowControlBox->addItem("None", QSerialPort::NoFlowControl);
	ui.flowControlBox->addItem("RTS/CTS", QSerialPort::HardwareControl);
	ui.flowControlBox->addItem("XON/XOFF", QSerialPort::SoftwareControl);
	*/
}

void FLDlgChooseSerialPort::fillPortsInfo()
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

void FLDlgChooseSerialPort::updateParameters()
{
	
}

void FLDlgChooseSerialPort::send_zc_data()
{
	
}

void FLDlgChooseSerialPort::send_wio_data()
{
}
