#include "fljobpropertydlg.h"

FLJobPropertyDlg::FLJobPropertyDlg(FLJob* job,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	_job = job;
	Q_ASSERT(_job);
	ui.spinBox->setValue(_job->DailyAdjust());

	FLJob::DayLight start = _job->start_daylight();
	

	FLJob::DayLight end = _job->end_daylight();
	

	if (start == end)
	{
		ui.groupBox_2->setChecked(false);
	}
	else
	{
		ui.timeEdit->time().setHMS(start.hour, start.min, 0);
		ui.comboBox_2->setCurrentIndex(start.week);
		ui.comboBox_3->setCurrentIndex(start.dayOfWeek);
		ui.comboBox_5->setCurrentIndex(start.month);

		ui.timeEdit_2->time().setHMS(end.hour, end.min, 0);
		ui.comboBox_4->setCurrentIndex(end.week);
		ui.comboBox_6->setCurrentIndex(end.dayOfWeek);
		ui.comboBox_7->setCurrentIndex(end.month);
	}
	ui.lineEdit->setText(_job->getProperty("Name").value.toString());
	ui.lineEdit_2->setText(_job->getProperty("JobPath").value.toString());
	ui.lineEdit_3->setText(_job->getProperty("Comments").value.toString());

	ui.comboBox_4->setCurrentText(_job->getProperty("Standard").value.toString());

	ui.lineEdit_2->setEnabled(false);

	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_ok()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_cancel()));

}

FLJobPropertyDlg::~FLJobPropertyDlg()
{

}

void FLJobPropertyDlg::on_ok()
{
	Q_ASSERT(_job);
	_job->set_daily_adjust(ui.spinBox->value());
	
	if (ui.groupBox->isChecked())
	{
		FLJob::DayLight start;
		start.hour = ui.timeEdit->time().hour();
		start.min = ui.timeEdit->time().minute();
		start.week = ui.comboBox_2->currentIndex();
		start.dayOfWeek = ui.comboBox_3->currentIndex();
		start.month = ui.comboBox_5->currentIndex();

		FLJob::DayLight end;
		end.hour = ui.timeEdit_2->time().hour();
		end.min = ui.timeEdit_2->time().minute();
		end.week = ui.comboBox_4->currentIndex();
		end.dayOfWeek = ui.comboBox_6->currentIndex();
		end.month = ui.comboBox_7->currentIndex();
		_job->set_daylighttime(start, end);
	}
	else
	{
		FLJob::DayLight start;
		FLJob::DayLight end;
		_job->set_daylighttime(start, end);
	}
	_job->addProperty("Name", ui.lineEdit->text());
	_job->addProperty("Comments", ui.lineEdit_2->text());
	_job->addProperty("Standard", ui.comboBox->currentText());

	QDialog::accept();
}
void FLJobPropertyDlg::on_cancel()
{
	QDialog::reject();
}