#include "mysettingwidget.h"
#include "ui_mysettingwidget.h"

MySettingWidget::MySettingWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MySettingWidget)
{
	ui->setupUi(this);

	animation.setPrameters(100,UP_TO_DOWN,0);

	ui->listWidget->addItem("1");
	ui->listWidget->addItem("2");
}

MySettingWidget::~MySettingWidget()
{
	delete ui;
}


void MySettingWidget::add_widget(QString title, QWidget *w)
{
	ui->listWidget->addItem(title);
	ui->stackedWidget->addWidget(w);
}


void MySettingWidget::on_listWidget_currentRowChanged(int currentRow)
{
	int previous=this->currentIndex;
	this->currentIndex=currentRow;
	animation.start(ui->stackedWidget->widget(currentRow),ui->stackedWidget->widget(previous));
}

QWidget* MySettingWidget::current_widget()
{
	return ui->stackedWidget->widget(currentIndex);
}
