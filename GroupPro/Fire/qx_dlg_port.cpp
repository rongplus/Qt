#include "qx_dlg_port.h"
#include <QMessageBox>;

PortDialog::PortDialog(WorkType type,QWidget *parent) : QDialog(parent)
{
	m_settings = new QSettings("Mircom", "FireLink");
	ui.setupUi(this);
	_type = type;

	QString previousPort = m_settings->value("Connection/Port", QString("COM1")).toString();
	int index = -1;
	int count = 0;
	foreach(QSerialPortInfo p, QSerialPortInfo::availablePorts())
	{
		ui.cbCommPorts->addItem(p.portName() + ": " + p.description());
		if (p.portName() == previousPort)
			index = count;
		count++;
	}

	if (index != -1)
		ui.cbCommPorts->setCurrentIndex(index);

	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(startWork()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(exitWork()));
}

QString PortDialog::port() {
	QString text = ui.cbCommPorts->currentText();
	int colonIndex = text.indexOf(":");
	text = text.left(colonIndex);
	m_settings->setValue("Connection/Port", text);
	return text;
}

void PortDialog::write_zigbee_config(QByteArray* ba)
{
	//login
	_port->write("login");
	_port->waitForBytesWritten(500);
	auto bDataReady = _port->waitForReadyRead(500);
	QByteArray ret_array;
	if (bDataReady)
	{
		ret_array = _port->readAll();
		
	}
}

void PortDialog::startWork()
{
	
	auto serial = new QSerialPort(port(), this);

	if (!serial->open(QIODevice::ReadWrite))
	{
		QMessageBox::critical(this, tr("Error"), serial->errorString());
		delete serial;
		return;
	}

	_port = serial;
	bool check = true;
	check = check &	_port->setBaudRate(QSerialPort::Baud38400);
	//check = check & m_serialPort->setDataBits(QSerialPort::Data8);
	//check = check & m_serialPort->setStopBits(QSerialPort::OneStop);
	//check = check & m_serialPort->setParity(QSerialPort::NoParity);
	check = check & _port->setFlowControl(QSerialPort::NoFlowControl);

	write_zigbee_config(_array);
}

void PortDialog::exitWork()
{

}

void PortDialog::set_data(QByteArray& ba)
{
	_array = &ba;
}