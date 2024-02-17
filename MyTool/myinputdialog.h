#ifndef MYINPUTDIALOG_H
#define MYINPUTDIALOG_H

#include <QDialog>

namespace Ui {
class MyInputDialog;
}

class MyInputDialog : public QDialog
{
	Q_OBJECT

public:
	explicit MyInputDialog(QWidget *parent = 0);
	~MyInputDialog();

	void insertWidget(QWidget* w);
private:
	Ui::MyInputDialog *ui;
};

#endif // MYINPUTDIALOG_H
