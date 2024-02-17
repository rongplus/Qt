#include "flchooseoutputdialog.h"
#include "flwio.h"
FLChooseOutputDialog::FLChooseOutputDialog(FLZoneControllor* zc, FLObject* port, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	_zc = zc;
	_port = port;
	init();

	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(on_ok()));
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(on_cancel()));
	connect(ui.checkBox, SIGNAL(stateChanged(int)), this, SLOT(on_stateChanged(int)));
	connect(ui.checkBox_2, SIGNAL(stateChanged(int)), this, SLOT(on_stateChanged(int)));
	connect(ui.checkBox_3, SIGNAL(stateChanged(int)), this, SLOT(on_stateChanged(int)));
	connect(ui.checkBox_4, SIGNAL(stateChanged(int)), this, SLOT(on_stateChanged(int)));
	connect(ui.checkBox_5, SIGNAL(stateChanged(int)), this, SLOT(on_stateChanged(int)));
	connect(ui.checkBox_6, SIGNAL(stateChanged(int)), this, SLOT(on_stateChanged(int)));
}

FLChooseOutputDialog::~FLChooseOutputDialog()
{

}

void FLChooseOutputDialog::init()
{
	Q_ASSERT(_zc);
	Q_ASSERT(_port);
	QString type = _port->getProperty("Type").value.toString();
	auto wio = qobject_cast<FLWio*>(_port);
	if (wio)
	{
		//return;
		//type = wio->getProperty("WIOType").value.toString();
	}
	QList<FLObject*> list;
	_bOutput = true;
	_bWioOutput = false;

	if (type == "WIO")
	{
		auto wio = qobject_cast<FLWio*>(_port);
		Q_ASSERT(wio);
		auto wio_type = wio->getProperty("WIOType").value.toString();
		if (wio_type.compare("Output") == 0)
		{
			_bWioOutput = true;
			list = _zc->inputs();
			//todo: remove trouble
			for each (auto obj in list)
			{
				QString sub_type = obj->getProperty("Type").value.toString();
				if (sub_type == "ZC_LOCAL_TROUBLES" || sub_type == "ZC_REMOTE_TROUBLE")
					list.removeOne(obj);
			}
			ui.checkBox_2->setVisible(false);
			ui.checkBox_3->setVisible(false);
			
		}
		else
		{
			list = _zc->outputs();
			_bOutput = false;
			_bWioOutput = false;
		}
	}
	else	if (type.compare("ZC_RELAY") == 0)
	{
		list = _zc->inputs();		
	}	
	else//input
	{
		list = _zc->outputs();
		_bOutput = false;
	}
	auto corrs = _port->getProperty("Corrs").value.toString();
	auto corrs_list = corrs.split(" ", QString::SkipEmptyParts);
	int corrs_index = 0;
	int corrs_count = corrs_list.count();

	for each (auto obj in list)
	{
		QListWidgetItem* it = new QListWidgetItem(obj->getProperty("Name").value.toString());		
		ui.listWidget->addItem(it);
		it->setCheckState(Qt::Unchecked);
		if( corrs_index < corrs_count && corrs_list[corrs_index] != "0x00")
			it->setCheckState(Qt::Checked);
		corrs_index++;
	}

	ui.comboBox->addItem(_port->getProperty("Name").value.toString());
	if (_bOutput)
	{
		setWindowTitle("Choose Input Dialog");
		ui.label->setText("Input");
		ui.label_2->setText("Output");
		ui.checkBox_6->setVisible(false);
	}
	else
	{
		ui.checkBox->setVisible(false);
		ui.checkBox_2->setVisible(false);
		ui.checkBox_3->setVisible(false);
		ui.checkBox_4->setVisible(false);
	}
}

void FLChooseOutputDialog::on_cancel()
{
	QDialog::reject();
}

void FLChooseOutputDialog::on_stateChanged(int state)
{
	QCheckBox * chb = (QCheckBox*)sender();
	QString type = _port->getProperty("Type").value.toString();
	bool bOut = true;
	if (type == "WIO")
	{
		auto wio_type = _port->getProperty("WIOType").value.toString();
		if (wio_type == "Input")
			bOut = false;
	}
	else if(type != "ZC_RELAY")
		bOut = false;

	//bool bCheck = state == Qt::Checked;
	int nItem = ui.listWidget->count();
	int stat = 0;
	int end = 0;


	if (chb == ui.checkBox)
	{
		end = 4;
	}
	else if (chb == ui.checkBox_2)
	{
		stat = 4;
		end = 22;
		
	}
	else if (chb == ui.checkBox_3)
	{
		stat = 22;
		end = 34;
	}
	else if (chb == ui.checkBox_4)
	{
		stat = 34;
		end = 48;
		if (_bWioOutput)
		{
			stat = 4;
			end = 18;
		}
	}
	
	else if (chb == ui.checkBox_6)
	{
		stat = 0;
		end = 3;
		
	}
	else if (chb == ui.checkBox_5)
	{
		stat = 48;
		end = nItem;
		if (_bWioOutput)
		{
			stat = 18;
			end = nItem;
		}
		if (bOut == false)
			stat = 3;
	}



	for (size_t i = stat; i < end && i<nItem; i++)
	{
		auto item = ui.listWidget->item(i);
		item->setCheckState( (Qt::CheckState) state);
	}

}

void FLChooseOutputDialog::on_ok()
{
	QString type = _port->getProperty("Type").value.toString();
	auto value = _port->getProperty("Value").value.toString();
	QList<FLObject*> list;
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
	
	QDialog::accept();
}

