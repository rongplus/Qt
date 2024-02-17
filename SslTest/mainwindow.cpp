#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>
#include <QSslSocket>
#include <QSsl>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QHostAddress address = QHostAddress::Any;
	quint16 port = 22333;

	sslServer.setSslLocalCertificate("/devel/sslserver.pem");
	sslServer.setSslPrivateKey("/devel/sslserver.key");
	sslServer.setSslProtocol(QSsl::TlsV1_2);

	if (sslServer.listen(address, port))
		qDebug().nospace() << "Now listening on " << qPrintable(address.toString()) << ":" << port;
	else
		qDebug().nospace() << "ERROR: could not bind to " << qPrintable(address.toString()) << ":" << port;

     connect(&sslServer, SIGNAL(newConnection()), this, SLOT(slot_newConnection()));




}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::new_client()
{
    QString hostName = "127.0.0.1";    // DO NOT CHANGE THIS AS IT MUST MATCH THE FQDN OF THE CERTIFICATE (you MUST create your own certificate in order to change this)
        quint16 port = 22333;

        QSslSocket *sslSocket = new QSslSocket(this);
		sslSocket->addCaCertificates("/devel/sslserver.pem");
        sslSocket->connectToHostEncrypted(hostName, port);
        connect(sslSocket, SIGNAL(connected()), this, SLOT(slot_ok()));
        connect(sslSocket, SIGNAL(encrypted()), this, SLOT(slot_eok()));

}

void MainWindow::slot_newConnection()
{
	qDebug() << "New connection 132";
	QSslSocket *sslSocket = dynamic_cast<QSslSocket*>(sslServer.nextPendingConnection());
    qDebug() << "Message:" << QString( sslSocket->peerName());
	connect(sslSocket, SIGNAL(readyRead()), this, SLOT(slot_server_data_ready()));
	connect(sslSocket, SIGNAL(sslErrors(const QList<QSslError> &)),this, SLOT(errorOccured(const QList<QSslError> &)));
	connect(sslSocket, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
	sslSocket->write("welcome you");

}

void MainWindow::slot_server_data_ready()
{
	QSslSocket *sslSocket = dynamic_cast<QSslSocket*>(sender());
	QByteArray message = sslSocket->readAll();    // Read message
	qDebug() << "client said:" << QString(message);
	sslSocket->write("Got it!");


}

void MainWindow::errorOccured(const QList<QSslError> &lt)
{
	foreach(auto l, lt)
	{
		qDebug() << l.errorString();
	}
}

void MainWindow::slot_disconnected()
{
	QSslSocket *sslSocket = dynamic_cast<QSslSocket*>(sender());
	qDebug() << "Message:" << QString( sslSocket->peerName());
}

void MainWindow::slot_client_data_ready()
{
	QSslSocket *sslSocket = dynamic_cast<QSslSocket*>(sender());
	QByteArray message = sslSocket->readAll();    // Read message
	qDebug() << "Server said:" << QString(message);
}

void MainWindow::slot_client_disconnected()
{
	QSslSocket *sslSocket = dynamic_cast<QSslSocket*>(sender());
	qDebug() << "Message:" << QString( sslSocket->peerName());
}
void MainWindow::slot_eok()
{
    QSslSocket *sslSocket = dynamic_cast<QSslSocket*>(sender());
    qDebug() << "Connected";
    sslSocket->write("Hello, World!");    // Send message to the server

    if (sslSocket->waitForBytesWritten(-1))    // Wait until message is sent (also makes QSslSocket flush the buffer)
        qDebug() << "Message sent";
    else
        qDebug().nospace() << "ERROR: could not send message (" << qPrintable(sslSocket->errorString()) << ")";

}

void MainWindow::slot_ok()
{
	QSslSocket *sslSocket = dynamic_cast<QSslSocket*>(sender());
    connect( sslSocket, SIGNAL(readyRead()), this, SLOT(slot_client_data_ready()));
    connect( sslSocket, SIGNAL(disconnected()), this, SLOT(slot_client_disconnected()));
}


void MainWindow::on_pushButton_clicked()
{
	new_client();
}
