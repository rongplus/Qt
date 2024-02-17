#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
}

Dialog::~Dialog()
{
	delete ui;
}


void Dialog::setWidget(QWidget *pW)
{
	w = pW;
	w->setGeometry(10,10,100,30);
	w->setParent(this);
	w->show();
}
