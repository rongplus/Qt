#include "flzcpropertydlg.h"

FLZCPropertyDlg::FLZCPropertyDlg(FLZoneControllor* zc, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	_zc = zc;

	ui.lineEdit->setText(_zc->getProperty("Name").value.toString());
	ui.lineEdit_2->setText(_zc->getProperty("Comments").value.toString());
	ui.lineEdit_3->setText(_zc->getProperty("Location").value.toString());

	QStringList vList = _zc->getProperty("MAC Address").items;
	QString mac;
	if (vList.size() == 8)
	{
		mac = QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(vList[0]).arg(vList[1]).arg(vList[2]).arg(vList[3]).arg(vList[4]).arg(vList[5]).arg(vList[6]).arg(vList[7]);
	}
	else
		mac = "FF:FF:FF:FF:FF:FF:FF:FF";
	ui.leMAC->setText(mac);

	ui.lineEdit_7->setText(_zc->getProperty("PassCode_Low").value.toString());
	ui.lineEdit_8->setText(_zc->getProperty("PassCode_Mid").value.toString());
	ui.lineEdit_9->setText(_zc->getProperty("PassCode_Hei").value.toString());

	auto scu_flag = zc->getProperty("SCU_FEATURE_FLAGS").value.toString();

	ui.comboBox_6->setCurrentText(_zc->getProperty("PanID").value.toString());

	ui.comboBox->addItems(_zc->getProperty("Channel").items);
	ui.comboBox->setCurrentText(_zc->getProperty("Channel").value.toString());

	ui.comboBox_3->addItems(QStringList() << "CFG_MAN_SIGSIL" << "CFG_AGENCY_ULC" << "CFG_MAN_SIGSIL/CFG_AGENCY_ULC");
	ui.comboBox_3->setCurrentText(_zc->getProperty("SCU_FEATURE_FLAGS").value.toString());

	ui.comboBox_4->addItems( _zc->getProperty("TIMER_SIG_SIL").items);
	ui.comboBox_4->setCurrentText(_zc->getProperty("TIMER_SIG_SIL").value.toString());

	ui.comboBox_5->addItems(_zc->getProperty("TIMER_ACFAIL_XMIT_DELAY").items);
	ui.comboBox_5->setCurrentText(_zc->getProperty("TIMER_ACFAIL_XMIT_DELAY").value.toString());


	connect(ui.pb_ok, SIGNAL(clicked()), this, SLOT(on_ok()));
	connect(ui.pb_cancel, SIGNAL(clicked()), this, SLOT(on_cancel()));
}

FLZCPropertyDlg::~FLZCPropertyDlg()
{

}

void FLZCPropertyDlg::on_cancel()
{
	QDialog::reject();
}
void FLZCPropertyDlg::on_ok()
{
	QDialog::accept();
}
