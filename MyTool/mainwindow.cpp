#include "mainwindow.h"
#include "ui_mainwindow.h"



#include <QDesktopWidget>
#include <QStandardItemModel>
#include <QStandardPaths>

//#include <windows.h>
#include <QDebug>
#include <QDir>
#include <QDirModel>

#include "mymodel.h"
#include "dialog.h"
#include "myinputdialog.h"
#include "mtthread.h"

#include <vector>
#include <QPrinter>
#include <QCalendarWidget>
#include <QWidgetAction>

#include "testtoollib.h"
#include "testclass.h"
using namespace  std;
#include "dialog.h"

#include "edcwindow.h"
#include "splitphotodialog.h"

#define O_Ver tr("Version")

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);


	QDirModel *modeldir = new QDirModel(this);
	modeldir->setFilter(QDir::AllDirs|QDir::Hidden|QDir::System);
	modeldir->setReadOnly(false);

		// Tie TreeView with DirModel
		// QTreeView::setModel(QAbstractItemModel * model)
		// Reimplemented from QAbstractItemView::setModel().
	 ui->treeView->setModel(modeldir);


	 QPair<QIcon, QString> para;
	 QFileIconProvider *provider = new QFileIconProvider;
	 para.second = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).first();//QDesktopServices::storageLocation(QDesktopServices::DesktopLocation);
	 para.first = provider->icon(QFileInfo(para.second));
	 ui->comboBox->addItem(para.first, para.second);


	 para.second = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first();
 //    para.second = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
	 para.first = provider->icon(QFileInfo(para.second));
	 ui->comboBox->addItem(para.first, para.second);

	 QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first();
 //    para.second = QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
	 para.first = provider->icon(QFileInfo(para.second));
	 ui->comboBox->addItem(para.first, para.second);

	 QFileInfoList lista = QDir::drives();
	 while(!lista.isEmpty())
	 {
		 para.second = lista.takeFirst().filePath();
		 para.first = provider->icon(QFileInfo(para.second));
		 ui->comboBox->addItem(para.first, para.second);
	 }
	 delete provider;

//[a-zA-Z0-9]{1,100}
	 QRegExpValidator* vpro = new QRegExpValidator(QRegExp("[0-9]{4}/[0-9]{1,2}/[0-9]{1,2}"),this);
	 ui->lineEdit ->setValidator(vpro);
	 //ui->lineEdit_2 ->setValidator(new QRegExpValidator(QRegExp("[\\w]{1,100}"),this));


	//connect(ui->dateEdit,SIGNAL(dateChanged(QDate)), this ,SLOT(on_dateEdit_dateChanged(QDate)));


	 QDate dt = QDate::fromString("2012/11/02","yyyy/M/d");
	 //if(dt == QDate() )qDebug() << dt;
	 qDebug() << dt.toString("yyyy/MM/dd");
	 dt = QDate();

	 MyInputDialog dlg;

	 QButtonGroup* gr = new QButtonGroup(this);
	 QRadioButton* r1 = new QRadioButton( "Choise 1", this);
	 QRadioButton* r2 = new QRadioButton("Choise 2", this);
	 gr->addButton(r1);
	 gr->addButton(r2);

	 dlg.insertWidget(r1);
	 dlg.insertWidget(r2);
	 //dlg.exec();

	 vector<int> iVector;
	 for(int i=0; i<20; i++)
		 iVector.push_back(i*4);

	 for_each( begin(iVector), end(iVector), [](int n){
		 //if(n%2==0)
			//qDebug() << n;
	 });

	 for (auto itm : iVector) {
		//qDebug() << itm;
		 }

	// qDebug() << "age = " << my_age;
	 float ourArray[5];

	 for(auto& i: ourArray)
	 {
		i = 10.004f;
	 }

	 for(auto& i: ourArray)
	 {
		//qDebug() << i;
	 }
	/*MtThread th;
	th.pa = this;
	th.thread1();
	th.thread2();
*/

	std::unique_ptr<long> p(new long);

	*p = 10;

	QPushButton *button=new QPushButton;
	button->setText("click me");
	QListWidgetItem *item=new QListWidgetItem;
	ui->listWidget->insertItem(0,item);
	ui->listWidget->setItemWidget(item,button);

	QStandardItemModel* m= new QStandardItemModel();
	m->setColumnCount(2);
	m->setRowCount(2);
	ui->listView->setModel(m);
	//ui->listView->model()->insertRow(0);

	QPrinter printer;
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setFullPage(false);
	printer.setPageSize(QPrinter::Letter);

	QPageLayout lo;
	lo.setOrientation(QPageLayout::Landscape);
	//printer.setPageLayout(lo);
	printer.setPageOrientation(QPageLayout::Landscape);
	QRect papersize = printer.paperRect();
	QRect pagesize = printer.pageRect();

	//ui->checkBox->setPixmap( ":/mytool/img/face-smile 2.png" );
	//ui->checkBox->che

	QCalendarWidget *cal_w = new QCalendarWidget();
	cal_w->setDateRange(QDate(1900, 1, 1), QDate(2099, 12, 31));
	cal_w->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
	cal_w->setGridVisible(true);
	cal_w->setDateEditEnabled(false);

	connect(cal_w,SIGNAL(clicked(const QDate &)),this , SLOT(slot_clicked(const QDate &)));
	QMenu* m_cal_menu = new QMenu(this) ;
	QWidgetAction* m_calact = new QWidgetAction(m_cal_menu);
	m_calact->setDefaultWidget(cal_w);
	m_cal_menu->addAction(m_calact);
	ui->btnCalendar->setMenu(m_cal_menu);

	//Dialog dlg11;
	//dlg11.exec();
	//setMenuBar( ui->menuBar);
	//ui->menuBar->show();
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_dateEdit_dateChanged(const QDate& date)
{
	QString str = date.toString("yyyy/MM/dd");
	qDebug() << str;
}

void MainWindow::on_toolButton_clicked(){}



void MainWindow::on_btnCalendar_clicked()
{

	QDate dt;
	dt.setDate(2009,3,9);
	QWidgetAction* wa = qobject_cast<QWidgetAction*>(ui->btnCalendar->menu()->actions()[0]);
	QCalendarWidget* cw = qobject_cast<QCalendarWidget*>( wa->defaultWidget() );
	if(cw)
		//cw->setCurrentPage(2011,3);
		cw->setSelectedDate(dt);
	cw->setFocus();
	ui->btnCalendar->showMenu();

	//ui->lineEdit->setText( cal_w->selectedDate().toString("yyyy/MM/dd"));


}

void MainWindow::slot_clicked(const QDate &date)
{
	ui->lineEdit->setText( date.toString("yyyy/MM/dd"));

}

void MainWindow::on_actionEDC_triggered(bool checked )
{
	qDebug() << "wwwww";
	EDCWindow *window = new EDCWindow(this);
	window->show();
}

void MainWindow::on_actionSplit_triggered(bool checked)
{
	SplitPhotoDialog dlg;
	dlg.exec();
}

void MainWindow::addItem(QString str)
{
	//ui->comboBox_2->addItem(str);
}
