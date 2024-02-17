#include "myinputdialog.h"
#include "ui_myinputdialog.h"

MyInputDialog::MyInputDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MyInputDialog)
{
	ui->setupUi(this);
	//ui->verticalLayout->insertWidget();
}

MyInputDialog::~MyInputDialog()
{
	delete ui;
}


void MyInputDialog::insertWidget(QWidget* w)
{
	//button group and space item
	ui->verticalLayout->insertWidget(ui->verticalLayout->count()-2,w);
}
