#include "flzctimedlg.h"
#include <QTime>

FLZCTimeDlg::FLZCTimeDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	ui.timeEdit->setTime(QTime::currentTime());
	ui.timeEdit_2->setTime(QTime::currentTime());
}

FLZCTimeDlg::~FLZCTimeDlg()
{

}
void FLZCTimeDlg::on_cancel()
{
	QDialog::reject();
}

void FLZCTimeDlg::on_update_to_current()
{
}

void FLZCTimeDlg::on_update_to_new()
{
}

void FLZCTimeDlg::on_connected()
{
	_port = ui.widget->get_port();
	ui.pushButton_2->setEnabled(true);
	ui.pushButton_3->setEnabled(true);
	//todo : get time from zc
	ui.timeEdit_2->setTime(QTime::currentTime());
}
