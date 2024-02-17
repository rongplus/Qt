#include "df.h"
#include "ui_df.h"
#include <QDebug>
DF::DF(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::DF)
{
	ui->setupUi(this);
	for(int n=0; n<3; n++)
	{
		QTreeWidgetItem* it = new QTreeWidgetItem(ui->treeWidget);
		it->setText(0,"Center " + QString::number(n));
		it->setData(0,Qt::UserRole+ 3, 0);
		ui->treeWidget->addTopLevelItem(it);
		for(int m=0; m<3; m++)
		{
			QTreeWidgetItem* it1 = new QTreeWidgetItem(it);
			it1->setData(0,Qt::UserRole+ 3, 1);
			it1->setText(0,"Subject Group " + QString::number(m) );
			it1->setFlags(it1->flags() | Qt::ItemIsEditable);
			it1->setText(1," " + QString::number(m*100) + " - " + QString::number(m*100 + (m+2)*10));
			it->addChild(it1);
		}
	}
}

DF::~DF()
{
	delete ui;
}

void DF::on_btnAdd1_clicked()
{

	QTreeWidgetItem* it = new QTreeWidgetItem(ui->treeWidget);
	it->setText(0,"Center " + QString::number( ui->treeWidget->topLevelItemCount() + 1 ));
	it->setData(0,Qt::UserRole+ 3, 0);
	ui->treeWidget->addTopLevelItem(it);
}

void DF::on_btnAdd2_clicked()
{
	QTreeWidgetItem *it = ui->treeWidget->currentItem();
	QTreeWidgetItem* it1 = new QTreeWidgetItem(it);
	it1->setData(0,Qt::UserRole+ 3, 1);
	int m = it->childCount() + 1;
	it1->setText(0,"Subject Group " + QString::number(m ) );
	it1->setFlags(it1->flags() | Qt::ItemIsEditable);
	it1->setText(1," " + QString::number(m*100) + " - " + QString::number(m*100 + (m+2)*10));
	it->addChild(it1);
}

void DF::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
	qDebug() << "enter";
	//if( ui->treeWidget->isTopLevel())
	if(item->data(0,Qt::UserRole+3) == 0)
	{
		ui->btnAdd2->setEnabled(true);
		ui->stackedWidget->setCurrentIndex(0);
	}
	else
	{
		ui->btnAdd2->setEnabled(false);
		ui->stackedWidget->setCurrentIndex(1);
		if(column == 1)
			ui->treeWidget->editItem(item,1);
	}
}
