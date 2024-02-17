#include "fldaylightdlg.h"

FLDayLightDlg::FLDayLightDlg(FLJob* job,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	_job = job;
	init();
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_ok()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_cancel()));
}

FLDayLightDlg::~FLDayLightDlg()
{

}

void FLDayLightDlg::on_ok()
{
	Q_ASSERT(_job);
	_job->set_daily_adjust(ui.spinBox->value());
	FLJob::DayLight start;
	start.hour = ui.timeEdit->time().hour();
	start.min = ui.timeEdit->time().minute();
	start.week = ui.comboBox->currentIndex();
	start.dayOfWeek = ui.comboBox_3->currentIndex();
	start.month = ui.comboBox_5->currentIndex();

	FLJob::DayLight end;
	end.hour = ui.timeEdit_2->time().hour();
	end.min = ui.timeEdit_2->time().minute();
	end.week = ui.comboBox_2->currentIndex();
	end.dayOfWeek = ui.comboBox_4->currentIndex();
	end.month = ui.comboBox_6->currentIndex();
	if (ui.groupBox->isChecked())
		_job->set_daylighttime(start, end);

	QDialog::accept();
}
void FLDayLightDlg::on_cancel()
{
	QDialog::reject();
}

void FLDayLightDlg::init()
{
	Q_ASSERT(_job);
	ui.spinBox->setValue(_job->DailyAdjust());

	FLJob::DayLight start = _job->start_daylight();	
	ui.timeEdit->time().setHMS(start.hour, start.min,0);	
	ui.comboBox->setCurrentIndex(start.week);
	ui.comboBox_3->setCurrentIndex(start.dayOfWeek);
	ui.comboBox_5->setCurrentIndex(start.month);
	
	FLJob::DayLight end = _job->end_daylight();
	ui.timeEdit_2->time().setHMS(end.hour, end.min, 0);
	ui.comboBox_2->setCurrentIndex(end.week);
	ui.comboBox_4->setCurrentIndex(end.dayOfWeek);
	ui.comboBox_6->setCurrentIndex(end.month);
}
