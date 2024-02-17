#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();

	void setWidget(QWidget* pW);

private:
	Ui::Dialog *ui;
	QWidget* w;
};

#endif // DIALOG_H
