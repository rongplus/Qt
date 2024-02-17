#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "barcode.h"
#include <QCache>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QGraphicsItemGroup>
#include <QCheckBox>

#include "dataview.h"
#include "sharelib.h"

#define IMAGEVIEW_MIN_WIDTH  864
#define IMAGEVIEW_MIN_HEIGHT 140

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QGraphicsScene* pS = new QGraphicsScene(this);
	ui->graphicsView->setScene(pS);
	pS->setSceneRect( ui->le_x->text().toInt(), ui->le_y->text().toInt(),ui->le_width->text().toInt(),ui->le_height->text().toInt());


	isInitial = false;
	ui->widget->resize(ui->le_width->text().toInt(),ui->le_height->text().toInt());
	ui->scrollArea->setWidget(ui->widget);

	signalMapper = new QSignalMapper(this);

	connect(signalMapper, SIGNAL(mapped(QWidget*)), this, SLOT(X(QWidget*)));

	qDebug() << QT_VERSION;

	if(QT_VERSION >=0x050800)
		DataView* p = new DataView(this);

	if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
	{

	}

	Sharelib *aa = new Sharelib();




}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::showEvent(QShowEvent *event)
{

}


void MainWindow::on_btnApply_clicked()
{
	auto strPath = QApplication::applicationDirPath();
	QPixmap pix(strPath + "/113.png");
	QGraphicsPixmapItem* pB = new QGraphicsPixmapItem(pix.scaled(ui->graphicsView->width(),ui->graphicsView->height()));

	ui->graphicsView->scene()->addItem(pB);

}


QImage MainWindow::barcodeImg()
{
	QImage image(QSize(IMAGEVIEW_MIN_WIDTH,IMAGEVIEW_MIN_HEIGHT),QImage::Format_Mono);;
	QString str;
	image.fill(Qt::color1);
	QPainter painter(&image);
	painter.setPen(Qt::color0);
	QFont font;
	font.setStyleStrategy(QFont::NoAntialias);
	painter.setFont(font);
	BarCode bc;
	bc.drawBarCode(painter,100, 100, 100,
				"slabel 100", "plabel 100", "vlabel 100");
	painter.drawLine(49,49,49,25);
	painter.drawLine(49,49,25,49);
	painter.drawLine(49,112,25,112);
	painter.drawLine(49,112,49,136);
	// draw right corner markers
	painter.drawLine(800,49,800,25);
	painter.drawLine(800,49,824,49);
	painter.drawLine(800,112,824,112);
	painter.drawLine(800,112,800,136);


	QGraphicsPixmapItem* pB = new QGraphicsPixmapItem (QPixmap::fromImage( image));
	pB->setPos(0,0);
	pB->setZValue(10);
	pB->setEnabled(true);
	pB->setFlag(QGraphicsItem::ItemIsMovable, true);
	pB->setFlag(QGraphicsItem::ItemIsSelectable, true);
	ui->graphicsView->scene()->addItem(pB);
}

void MainWindow::reset_button()
{
	ui->btnCheckbox->setDown(false);
	ui->btnBarcode->setDown(false);
	ui->btnCombo->setDown(false);
	ui->btnRadiobox->setDown(false);
	ui->btnText->setDown(false);
	//ui->tbProperty->setRowCount(0);
}


void MainWindow::on_btnCheckbox_clicked()
{
	reset_button();
	ui->btnCheckbox->setDown(true);
	QGraphicsItemGroup* pG = new QGraphicsItemGroup();
	for(int n=0; n<3; n++)
	{
		QGraphicsPixmapItem* pP = new QGraphicsPixmapItem( QPixmap(":/app/img/checked-checkbox-512").scaled(36,36));
		pP->setEnabled(true);
		pP->setFlag(QGraphicsItem::ItemIsMovable, true);
		pP->setFlag(QGraphicsItem::ItemIsSelectable, true);
		pP->setPos(30+ n*100,0);
		pP->setZValue(10);

		int n1 = ui->tbProperty->rowCount();
		int n2 = ui->tbProperty->colorCount();
		QString str = ui->tbProperty->item(n,1)->text();
		qDebug() << "Out --" << n1 << n2 << str;
		QGraphicsTextItem *pT = new QGraphicsTextItem( str);
		pT->setEnabled(true);
		pT->setFlag(QGraphicsItem::ItemIsMovable, true);
		pT->setFlag(QGraphicsItem::ItemIsSelectable, true);
		pT->setPos(30+ 48 + n*100,0);
		pT->setZValue(10);
		QCheckBox* pCB = new QCheckBox(ui->widget);
		pCB->setText(str);
		pCB->setGeometry(38 + n*100,100,80,40);
		pCB->show();

		connect(pT, SIGNAL(xChanged()), signalMapper, SLOT(map()));
		signalMapper->setMapping(pT, pCB);

		pG->addToGroup(pP);
		//pG->addToGroup(pT);
		ui->graphicsView->scene()->addItem(pT);
	}

	pG->setEnabled(true);
	pG->setFlag(QGraphicsItem::ItemIsMovable, true);
	pG->setFlag(QGraphicsItem::ItemIsSelectable, true);
	pG->setPos(30,100);
	pG->setZValue(5);
	ui->graphicsView->scene()->addItem(pG);

}

void MainWindow::on_btnBarcode_clicked()
{
	reset_button();
	ui->btnBarcode->setDown(true);
	barcodeImg();

}

void MainWindow::on_btnCombo_clicked()
{
	reset_button();
	ui->btnCombo->setDown(true);
}

void MainWindow::on_btnRadiobox_clicked()
{
	reset_button();
	ui->btnRadiobox->setDown(true);
}

void MainWindow::on_btnText_clicked()
{
	reset_button();
	ui->btnText->setDown(true);
}


void MainWindow::X(QWidget *p)
{
	QCheckBox* pb = (QCheckBox*)p;
	qDebug() << pb->text();
	if( sender() == signalMapper)
		qDebug() << "really";
}
