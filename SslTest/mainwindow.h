#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "SslServer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void new_client();

protected slots:
	void slot_newConnection();
	void slot_server_data_ready();
	void errorOccured(const QList<QSslError> &);
	void slot_disconnected();

	void slot_client_data_ready();
	void slot_client_disconnected();

	void slot_eok();
	void slot_ok();


private slots:
	void on_pushButton_clicked();

private:
	Ui::MainWindow *ui;
	SslServer sslServer;
};

#endif // MAINWINDOW_H
