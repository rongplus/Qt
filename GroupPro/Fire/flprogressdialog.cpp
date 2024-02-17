#include "flprogressdialog.h"
#include <QMessageBox>

FLProgressDialog::FLProgressDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
}

FLProgressDialog::~FLProgressDialog()
{

}

void FLProgressDialog::send_data(QSerialPort* serial,QByteArray & data)
{
	_serial = serial;
	_data = &data;
}

void FLProgressDialog::showEvent(QShowEvent * event)
{
	FLZCSerialPort port;
	connect(&port, SIGNAL(progress(int)), this, SLOT(show_progress(int)));
	
}

void FLProgressDialog::show_progress(int v)
{
	ui.progressBar->setValue(v);
}
