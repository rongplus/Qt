#include "flcreatewiodlg.h"
#include "flglobal.h"

#include <QMessageBox>

FLCreateWioDlg::FLCreateWioDlg(FLWio *pWio, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	m_bEdit = true;
	m_pWio = pWio;


	Init(m_pWio);
}

FLCreateWioDlg::FLCreateWioDlg(FLZoneControllor *zc, QWidget *parent /*= 0*/)
{
	ui.setupUi(this);
	m_pWio = new FLWio(zc);
	m_pWio->addProperty("Channel", "Channel " + QString::number(zc->getProperty("Channel").value.toInt()), false);
	int num_wio = zc->countWio() + 1;
	m_pWio->Init();
	m_pWio->addProperty("Name", "New_WIO_" + QString::number(num_wio));
	m_pWio->addProperty("WIOType", "Input", QStringList() << "Input" << "Output");
	Init(m_pWio);

}

FLCreateWioDlg::~FLCreateWioDlg()
{

}

void FLCreateWioDlg::Init(FLWio *pWio)
{
	ui.leName->setText(pWio->getProperty("Name").value.toString());
	QStringList mac_addr = pWio->getProperty("MAC Address").items;
	ui.lineEdit->setText(mac_addr.join(":"));
	ui.lineEdit_2->setText(pWio->getProperty("Location").value.toString());

	ui.comboBox->setCurrentText(pWio->getProperty("Signaling").value.toString());
	QStringList slt = pWio->getProperty("Model").items;
	ui.comboBox_2->setCurrentText(pWio->getProperty("Model").value.toString());


	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(onOK()));
	connect(ui.cancelButton, SIGNAL(clicked()), this, SLOT(OnCancel()));

	DeviceModels *g = FLGlobal::getDeviceModels();
	int nSize = g->dev_size();

	for (int n = 0; n < nSize; n++)
	{
		DeviceModel m = g->dev(n);
		ui.comboBox_2->addItem(QString::fromStdString(m.name()));
	}
}

void FLCreateWioDlg::onOK()
{
	auto number = ui.lineEdit_3->text().toInt();
	FLZoneControllor* zc = (FLZoneControllor*)m_pWio->parent();
	if (zc->countWio() + number > 49)
	{
		QMessageBox::warning(this, "Number limited", "The maximum number of WIO is 49");
		return;
	}

	wio_list.append(m_pWio);
	auto name = ui.leName->text();
	QByteArray name_arr;

	for (size_t i = name.size()-1; i >0; i--)
	{
		auto a = name.at(i).toLatin1();
		if (a > '9' || a < '0')
			break;
		name_arr.prepend( a );
	}
	int start_index = 1;

	/*
	auto name_end = name.at(name.length()-1);
	
	if (name_end >= '0' && name_end <= '9')
	{
		m_pWio->addProperty("Name", name);
		start_index = name.right(1).toInt();
		name = name.left(name.length() - 1);
		
	}
	else
	{
		m_pWio->addProperty("Name", name + QString::number(1));
	}*/
	if (name_arr.size() == 0)
	{
		if(number>1)
			m_pWio->addProperty("Name", name + QString::number(1));
		else
			m_pWio->addProperty("Name", name );
	}
	else
	{
		m_pWio->addProperty("Name", name);
		start_index = QString(name_arr).toInt();
		name = name.left(name.length() - name_arr.size());
	}

	bool bOK;
	for (int n = 1; n < number; n++)
	{
		FLWio* wio = new FLWio(m_pWio->parent());
		wio_list.append(wio);
		FLZoneControllor *zc =(FLZoneControllor*) m_pWio->parent();
		wio->addProperty("Channel", "Channel " + QString::number(zc->getProperty("Channel").value.toInt()), false);
		
		wio->Init();	
		
		wio->addProperty("Name", name + QString::number(n + start_index));
		QString strmac = ui.lineEdit->text();
		QStringList slt = strmac.split(":");
		slt[slt.count() - 1] =QString("%1").arg(slt[slt.count() - 1].toInt(&bOK) + n,2,16, QLatin1Char('0')).toUpper();

		wio->addProperty("MAC Address", "MAC", slt);
		wio->addProperty("Location", ui.lineEdit_2->text());
		wio->setSignal(ui.comboBox->currentIndex());
		wio->setModel(ui.comboBox_2->currentIndex());
		if (ui.comboBox_3->currentIndex() == 0)
			wio->addProperty("WIOType", "Input", QStringList() << "Input" << "Output", false);
		else
			wio->addProperty("WIOType", "Output", QStringList() << "Input" << "Output", false);
		wio->addProperty("Corrs", "0x00",false);
		wio->addProperty("Value", "0x00");

	}


	QString strmac = ui.lineEdit->text();
	QStringList slt = strmac.split(":");
	m_pWio->addProperty("MAC Address","MAC",slt);
	m_pWio->addProperty("Location",ui.lineEdit_2->text());
	m_pWio->setSignal(ui.comboBox->currentIndex());
	m_pWio->setModel(ui.comboBox_2->currentIndex());
	if(ui.comboBox_3->currentIndex() == 0)
		m_pWio->addProperty("WIOType", "Input",QStringList() <<"Input"<<"Output",false);
	else
		m_pWio->addProperty("WIOType", "Output", QStringList() << "Input" << "Output", false);
	m_pWio->addProperty("Corrs", "0x00");
	m_pWio->addProperty("Value", "0x00");
	QDialog::accept();
}

void FLCreateWioDlg::OnCancel()
{
	if(m_bEdit==false)
		delete m_pWio;
	QDialog::reject();
}

void FLCreateWioDlg::on_model_changed(int index)
{
	if(index>0 && index <=5)	//mode =//	Mircom	//	Cooper	//	System Sensor	//	Gentex	//	Potter
	{
		ui.comboBox_2->setEnabled(true);
	}
	else
	{
		ui.comboBox_2->setEnabled(false);
	}
}
