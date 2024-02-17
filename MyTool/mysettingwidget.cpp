#include "mysettingwidget.h"
#include "ui_mysettingwidget.h"

#include "mymodel.h"
#include "mystyledelegate.h"

MySettingWidget::MySettingWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::MySettingWidget)
{
	ui->setupUi(this);

	animation.setPrameters(300,UP_TO_DOWN,QEasingCurve::InElastic);

	ui->listWidget->addItem("1");
	ui->listWidget->addItem("2");

	connect(&this->animation.animation, SIGNAL(finished()), this, SLOT(animationFinished()));


	MyModel* md = new MyModel(this);
	MyDB* db = new MyDB();
	md->setDB(db);
/*
	md->setColumnCount(10);
	md->setRowCount(11);

	for(int n=0; n<10; n++)
	{
		md->setHeaderData( n,Qt::Horizontal,QString::number(n) + " Col");
		md->setHeaderData( n,Qt::Vertical,QString::number(n) + " Row");
	}

	for(int n=0; n<10; n++)
		for(int m=0; m<10; m++)
		{
			QModelIndex in = md->index(m,n);//QAbstractItemModel::createIndex(m,n);
			md->setData(in,QString::number(n) + " * " + QString::number(m) );
		}


	QModelIndex in = md->index(10,0);//QAbstractItemModel::createIndex(m,n);
	md->setData(in,QColor(Qt::red) );
	md->setData( md->index(10,1), QDate::currentDate());
*/
	ui->tableView->setModel(md);
	MyStyleDelegate* st = new MyStyleDelegate(this);
	ui->tableView->setItemDelegate(st);
	ui->dateEdit->setDate(QDate::fromString("2010/02/02","yyyy/MM/dd"));
	ui->dateEdit_2->setDate(QDate::currentDate());
	ui->dateEdit->setCurrentSectionIndex(1);

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

void MySettingWidget::animationFinished()
{
	this->ui->stackedWidget->setCurrentIndex(this->currentIndex);
}
