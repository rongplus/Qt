#include "macexplore.h"
#include "ui_macexplore.h"

MacExplore::MacExplore(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MacExplore)
{
	ui->setupUi(this);
}

MacExplore::~MacExplore()
{
	delete ui;
}
