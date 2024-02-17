#include "edcwindow.h"
#include "ui_edcwindow.h"

EDCWindow::EDCWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::EDCWindow)
{
	ui->setupUi(this);
}

EDCWindow::~EDCWindow()
{
	delete ui;
}
