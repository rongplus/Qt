#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSignalMapper>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
protected:
	void showEvent(QShowEvent *event);

	QImage barcodeImg();
	void reset_button();
protected slots:
	void on_btnApply_clicked();

	void on_btnCheckbox_clicked();
	void on_btnBarcode_clicked();
	void on_btnCombo_clicked();
	void on_btnRadiobox_clicked();
	void on_btnText_clicked();
	void X(QWidget*);

private:
	Ui::MainWindow *ui;

	bool isInitial;

	QSignalMapper* signalMapper ;
};

#endif // MAINWINDOW_H
