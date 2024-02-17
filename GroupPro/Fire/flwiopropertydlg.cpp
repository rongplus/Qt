#include "flwiopropertydlg.h"
#include "flzonecontrollor.h"

FLWioPropertyDlg::FLWioPropertyDlg(FLWio* wio,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	_wio = wio;
	Q_ASSERT(_wio);

	ui.lineEdit->setText(wio->getProperty("Name").value.toString());
	ui.lineEdit_2->setText(wio->getProperty("Comments").value.toString());
	ui.lineEdit_3->setText(wio->getProperty("Location").value.toString());

	//ui.comboBox_2->addItems(wio->getProperty("Channel").items);
	for (int n = 11; n < 27; n++)
		ui.comboBox_2->addItem("Channel " + QString::number(n));
	ui.comboBox_2->setCurrentText(wio->getProperty("Channel").value.toString());

	ui.comboBox_4->addItems(wio->getProperty("Signaling").items);
	ui.comboBox_4->setCurrentText(wio->getProperty("Signaling").value.toString());

	ui.comboBox_3->addItems(wio->getProperty("Model").items);
	ui.comboBox_3->setCurrentText(wio->getProperty("Model").value.toString());	

	ui.comboBox_5->addItems(wio->getProperty("WIOType").items);
	ui.comboBox_5->setCurrentText(wio->getProperty("WIOType").value.toString());
	

	QStringList vList = wio->getProperty("MAC Address").items;
	QString mac;
	if (vList.size() == 8)
	{
		mac = QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(vList[0]).arg(vList[1]).arg(vList[2]).arg(vList[3]).arg(vList[4]).arg(vList[5]).arg(vList[6]).arg(vList[7]);
	}
	else
		mac = "FF:FF:FF:FF:FF:FF:FF:FF";
	ui.leMAC->setText(mac);
	
	FLZoneControllor* _zc = (FLZoneControllor*)_wio->parent();
	auto corrs = wio->getProperty("Corrs").value.toString();
	QList<FLObject*> list;
	auto wio_type = wio->getProperty("WIOType").value.toString();
	if (wio_type.compare("Output") == 0)
	{
		list = _zc->inputs();
		for each (auto obj in list)
		{
			QString sub_type = obj->getProperty("Type").value.toString();
			if (sub_type == "ZC_LOCAL_TROUBLES" || sub_type == "ZC_REMOTE_TROUBLE")
				list.removeOne(obj);
		}
	
	}
	else
	{
		list = _zc->outputs();	
		
	}

	auto corrs_list = corrs.split(" ", QString::SkipEmptyParts);
	int corrs_index = 0;
	int corrs_count = corrs_list.count();

	for each (auto obj in list)
	{
		QListWidgetItem* it = new QListWidgetItem(obj->getProperty("Name").value.toString());
		ui.listWidget->addItem(it);
		it->setCheckState(Qt::Unchecked);
		if (corrs_index < corrs_count && corrs_list[corrs_index] != "0x00")
			it->setCheckState(Qt::Checked);
		corrs_index++;
	}

	connect(ui.pb_ok, SIGNAL(clicked()), this, SLOT(on_ok()));
	connect(ui.pb_cancel, SIGNAL(clicked()), this, SLOT(on_cancel()));

	
	connect(ui.checkBox_4, SIGNAL(stateChanged(int)), this, SLOT(on_stateChanged(int)));

}

FLWioPropertyDlg::~FLWioPropertyDlg()
{
}

void FLWioPropertyDlg::on_cancel()
{
	QDialog::reject();
}

void FLWioPropertyDlg::on_stateChanged(int state)
{
	for (size_t i = 0; i < ui.listWidget->count() ; i++)
	{
		auto item = ui.listWidget->item(i);
		item->setCheckState((Qt::CheckState) state);
	}
}

void FLWioPropertyDlg::on_ok()
{
	auto _port = _wio;
	FLZoneControllor* _zc = (FLZoneControllor*)_wio->parent();
	auto wio_type = _wio->getProperty("WIOType").value.toString();

	QString type = _port->getProperty("Type").value.toString();
	auto value = _port->getProperty("Value").value.toString();
	QList<FLObject*> list;
	bool _bOutput = false;
	if (wio_type.compare("Output") == 0)
		_bOutput = true;

	if (_bOutput)//output
	{
		list = _zc->inputs();
	}
	else
	{
		list = _zc->outputs();
	}

	int nItem = ui.listWidget->count();
	QString corrs;
	for (size_t i = 0; i < nItem; i++)
	{
		auto item = ui.listWidget->item(i);
		if (item->checkState() == Qt::Checked)
		{
			corrs += "0x01 ";//list[i]->getProperty("Value").value.toString() + " ";			
		}
		//reset
		else
		{
			corrs += "0x00 ";
		}
	}
	_port->addProperty("Corrs", corrs, false);
	if (_bOutput)//output
	{
		_zc->reset_input();
	}
	else
	{
		_zc->reset_output();
	}

	_wio->addProperty("Name", ui.lineEdit->text());
	_wio->addProperty("Comments", ui.lineEdit_2->text());
	_wio->addProperty("Location", ui.lineEdit_3->text());
	QString strmac = ui.leMAC->text();
	QStringList slt = strmac.split(":");
	_wio->addProperty("MAC Address", "MAC", slt);

	_wio->addProperty("Channel", ui.comboBox_2->currentText(), false);

	_wio->setSignal(ui.comboBox_4->currentIndex());
	_wio->setModel(ui.comboBox_3->currentIndex());
	if (ui.comboBox_3->currentIndex() == 0)
		_wio->addProperty("WIOType", "Input", QStringList() << "Input" << "Output", false);
	else
		_wio->addProperty("WIOType", "Output", QStringList() << "Input" << "Output", false);

	QDialog::accept();
}