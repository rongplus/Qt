#include "jswidget.h"
#include "ui_jswidget.h"

#include <QGridLayout>
#include <QSplitter>
#include <QDebug>
#include <QApplication>
#include <QStandardItemModel>


#include <QTextStream>
#include <QFile>


JSWidget::JSWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::JSWidget)
{
	ui->setupUi(this);

	QGridLayout* gl = new QGridLayout(this);

	setLayout(gl);
	QSplitter* sp = new QSplitter(this);
	sp->setOrientation(Qt::Vertical);
	sp->addWidget(ui->widget);
	sp->addWidget(ui->tableView);

	gl->addWidget(sp);

	QStandardItemModel * md = new QStandardItemModel( this );
	md->setRowCount(10);
	md->setColumnCount(15);
	int nIndex = 0;
	md->setHeaderData(nIndex++,Qt::Horizontal,"x");
	md->setHeaderData(nIndex++,Qt::Horizontal,"Data 1");
	md->setHeaderData(nIndex++,Qt::Horizontal,"Data 2");
	md->setHeaderData(nIndex++,Qt::Horizontal,"Data 3");
	md->setHeaderData(nIndex++,Qt::Horizontal,"Data 4");
	md->setHeaderData(nIndex++,Qt::Horizontal,"Data 5");
	md->setHeaderData(nIndex++,Qt::Horizontal,"Data 6");

	md->setHeaderData(nIndex++,Qt::Horizontal,"x");
	md->setHeaderData(nIndex++,Qt::Horizontal,"data 1");
	md->setHeaderData(nIndex++,Qt::Horizontal,"data 2");

	md->setHeaderData(nIndex++,Qt::Horizontal,"x");
	md->setHeaderData(nIndex++,Qt::Horizontal,"data 1");
	md->setHeaderData(nIndex++,Qt::Horizontal,"data 2");

	qsrand(QTime::currentTime().second());

	for(int n=0; n< 10; n++)
	{
		md->setData( md->index(n,0), n*10 );

		for(int j = 1 ; j< 7; j++)
			md->setData( md->index(n,j), qrand() %200 );
		for(int k =0; k<3; k++)
		{
			md->setData( md->index(n,7 +k ), 8 * n + k*8);
			md->setData( md->index(n,10 +k ), 11 * n + k*12);
		}
	}



	ui->tableView->setModel(md);

	connect( md, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)),
			 this, SLOT(on_dataChanged(const QModelIndex &, const QModelIndex &, const QVector<int> &)));

	auto path = exePath();
	qDebug() << path;
	path = "file://" + path;
	path += "/test2.html";
	ui->widget->load( QUrl(path ));

}

JSWidget::~JSWidget()
{
	delete ui;
}

QString JSWidget::exePath()
{
	auto path = QApplication::applicationDirPath();
#ifdef Q_OS_WIN32
	ui->web->load( QUrl(path + "/test2.html"));
#else ifdef Q_OS_MAC
	//qDebug() << path;
	int n = path.lastIndexOf("/");
	path = path.left(n);
	//qDebug() << path;
	n = path.lastIndexOf("/");
	path = path.left(n);
	//qDebug() << path;
	n = path.lastIndexOf("/");
	path = path.left(n);
	//qDebug() << path;

#endif
	return path;
}

void JSWidget::on_dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles )
{
	QFile html_file( exePath() + "/test.html");
	if(html_file.open(QIODevice::ReadWrite) == false)
		return;
	QTextStream st( &html_file);

	QStandardItemModel* md = (QStandardItemModel*)ui->tableView->model();
	int nRow = md->rowCount();
	//add new row
	if(bottomRight.row() ==  nRow-1 )
	{
		int nCol = md->columnCount();
		for(int n = 0;n < nCol; n++)
		{
			if( md->data( md->index(nRow-1, n )).toString() != "" )
			{
				md->insertRow(nRow);
			}
		}
	}

	//refresh chart;

	QString html;
	html = R"delimiter(<!DOCTYPE html>
		   <html>
		   <head>
		   <link rel="stylesheet" type="text/css" href="/Users/rong/Desktop/projects/js/c3/c3.css">
		   <script src="http://d3js.org/d3.v3.min.js" charset="utf-8"></script>
		   <script src="/Users/rong/Desktop/projects/js/c3/c3.js"></script>
		   </head>
		   <body>
		   <h1>A Web Page</h1>
		   <p id="demo">A Paragraph</p>
			<div id="chart"></div>
			<div id="chart2"></div>
			<div id="chart3"></div>
			<div id="chart4"></div>
		   <script type="text/javascript">
		   var data_value = [];)delimiter";

	//format chart 1;
	QString data;
	QStringList title = QStringList() << "x" << "data -1" << "data -2" << "data -3 " << "data -4" << "data -5" << "data -6" ;
	QString head = "data_value[%1] = ['%2' ";
	for(int j = 0 ; j< 7; j++)//data1
	{
		data += head.arg(j).arg(title[j]);
		for(int n=0; n< 10; n++)
		{
			data +=", " + md->data( md->index(n,j) ).toString();
		}
		data += "];";
	}
	//data += "data_value[7] = ['y',50,60,80];";
	html += data;

	QString chart1 = R"DDD(var chart = c3.generate({
					  bindto: '#chart',
				   data: {
					x:'x',
					 columns:    data_value
				   },
				   type:'spline',
					axes: {
					   data1: 'y',
					   data2: 'y2',
					 },
					  axis: {
					 x: {
					   label: 'X Label'
					 },
					 y: {
					   label: {
						 text: 'Y Axis Label',
						 position: 'outer-middle'
					   },
					   min:-100,
					   max:400
					 },
					 y2: {
					   show: true,
					   label: {
						 text: 'Y2 Axis Label',
						 position: 'outer-middle'
					   }
					 }
				   }
				 });
					  </script>)DDD";

	html += chart1;

	head = "";
	data = "";
	chart1 = "";

	head = R"DDD(<script type="text/javascript">)DDD";
	QList<int> resuslt_lt ;
	resuslt_lt.reserve(7);

	QList<int> lt0_70;
	QList<int> lt70_140;
	QList<int> lt140_200;

	for(int j = 0 ; j< 7; j++)//data2//col
	{
		lt0_70.append(0);
		lt70_140.append(0);
		lt140_200.append(0);
		for(int n=0; n< 10; n++)
		{
			if(md->data( md->index(n,j)).toInt() < 70)
				lt0_70[j] = lt0_70[j]+1;
			else if(md->data( md->index(n,j)).toInt() < 140)
				lt70_140[j] = lt70_140[j]+1;
			else
				lt140_200[j] = lt140_200[j]+1;
		}
	}
	QString s1,s2,s3;
	s1 = "['less70'";
	s2 = "['less140'";
	s3 = "['less200'";
	for(int n=0; n< 7; n++)
	{
		s1 += "," + QString::number(lt0_70[n]);
		s2 += "," + QString::number(lt70_140[n]);
		s3 += "," + QString::number(lt140_200[n]);
	}

	s1 += "]";
	s2 += "]";
	s3+= "]";
	data = s1+ "," +s2 + "," + s3;

	chart1 = "var chart4 = c3.generate({\
			bindto: '#chart4',\
			data: {\
			columns: ["
		+ data +
			"],types: {\
			data1: 'area',\
			data2: 'area-spline'\
			}\
			}\
			});\
			chart4.transform('pie') </script>";

	//chart2;

	html += head;
	//html += data;
	html += chart1;

	html += R"DDD(</body>
			</html>)DDD";

	//qDebug() << html;



	st << html;
	html_file.close();
	//ui->widget->setHtml(html);

	QString path = "file://" +  exePath() + "/test.html";

	ui->widget->load( QUrl(path ));



}
