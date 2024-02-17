#include "edcwidget.h"
#include "ui_edcwidget.h"


EDCWidget::EDCWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::EDCWidget)
{
	ui->setupUi(this);
	//connect(ui->btn1,SIGNAL(clicked()), this, SLOT( on_btn1_clicked()));
}

EDCWidget::~EDCWidget()
{
	delete ui;
}

void EDCWidget::on_btn1_clicked()
{
	qDebug() << "send User event";
	QEvent * e = new QEvent (QEvent::User);
	QApplication::sendEvent(this, e);


	QNetworkAccessManager* _uploadManager = new QNetworkAccessManager(this);
	  connect(_uploadManager,SIGNAL(finished(QNetworkReply*)),SLOT(replyFinished(QNetworkReply*)));
	 char* m_buf = NULL;

	  QFile file("/Users/rong/Desktop/Work/p3.png");
	  file.open(QIODevice::ReadOnly);
	  int file_len = file.size();
	  QDataStream in(&file);
	  m_buf = new char[file_len];
	  in.readRawData( m_buf, file_len);
	  file.close();

	  QNetworkRequest request(QUrl(tr("http://localhost/save.php?act=up&filename=Get_from.png")));
	  request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");
	  QByteArray arr = QByteArray(m_buf, file_len);
	  QNetworkReply* _reply = _uploadManager->post(request , arr);
	  connect(_reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(upLoadError(QNetworkReply::NetworkError)));
	  connect(_reply, SIGNAL(uploadProgress ( qint64 ,qint64 )), this, SLOT( OnUploadProgress(qint64 ,qint64 )));
}

void EDCWidget::upLoadError(QNetworkReply::NetworkError)
{
	qDebug() << "send error";
}

void EDCWidget::OnUploadProgress(qint64 a , qint64 b)
{
	qDebug() << a << b;
}

bool EDCWidget::event(QEvent *ev)
{
	if(ev->type() == QEvent::User )
	{
		qDebug() << "receive User event";
		return true;
	}
	return QWidget::event(ev);
}
