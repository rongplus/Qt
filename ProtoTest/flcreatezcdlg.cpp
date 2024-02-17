#include "flcreatezcdlg.h"
#include "flcreatewiodlg.h"
FLCreateZCDlg::FLCreateZCDlg(bool show_add_group,FLZoneControllor* pZC,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.groupBox_2->setVisible(show_add_group);
	if(show_add_group == false)
		resize(width(),height()-200);
	m_bEdit = false;

	m_pZC = pZC;
	m_bEdit = true;	

	Init(m_pZC);


}

FLCreateZCDlg::FLCreateZCDlg(bool show_add_group,FLFloor* floor, QWidget *parent /*= 0*/)
{	
	ui.setupUi(this);
	ui.groupBox_2->setVisible(show_add_group);
	if(show_add_group == false)
		resize(width(),height()-200);
	m_pZC = new FLZoneControllor(floor);
	int num_zc = floor->countZC()+1;


	m_pZC->Init();
	m_pZC->InitDefaultIO();
	m_pZC->addProperty("Name","New_ZC_" + QString::number(num_zc));
	
	Init(m_pZC);	
}

FLCreateZCDlg::~FLCreateZCDlg()
{

}

void FLCreateZCDlg::Init(FLZoneControllor* pZC)
{
	ui.leName->setText(pZC->getProperty("Name").value.toString());
	ui.leLocation->setText(pZC->getProperty("Location").value.toString());
	//QStringList strMac = pZC->getProperty("MAC").toStringList();
	QStringList mac_addr = pZC->getProperty("MAC Address").items;

	QString mac;
	foreach (auto s , mac_addr)
	{
		mac += s + ":";
	}
	ui.leMAC->setText(mac);
	ui.comboBox_2->setCurrentText(pZC->getProperty("PAN ID").value.toString());
	ui.comboBox->setCurrentIndex(pZC->getProperty("Channel").value.toInt());

	ui.leRange->setText( QString::number( pZC->getProperty("Range").value.toInt()));

	int nIO = pZC->countIO();

	QTreeWidgetItem *inputItem = new QTreeWidgetItem();
	inputItem->setIcon(0,QIcon(":/pro/Resources/icons/input-icon-48.png"));

	inputItem->setData(0,ItemPointerData,quintptr(pZC));
	inputItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	inputItem->setText(0,"Input");
	ui.treeWidget->addTopLevelItem (inputItem);

	QTreeWidgetItem *outputItem = new QTreeWidgetItem();
	outputItem->setIcon(0,QIcon(":/pro/Resources/icons/relay-icon-48.png"));
	outputItem->setData(0,ItemTypeData,QString("NULL"));
	outputItem->setData(0,ItemPointerData,quintptr(pZC));
	outputItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	outputItem->setText(0,"RELAYS");
	ui.treeWidget->addTopLevelItem(outputItem);

	QTreeWidgetItem *localTrItem = new QTreeWidgetItem();
	localTrItem->setIcon(0,QIcon(":/pro/Resources/icons/trouble-icon-48.png"));
	localTrItem->setData(0,ItemTypeData,QString("NULL"));
	localTrItem->setData(0,ItemPointerData,quintptr(pZC));
	localTrItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	localTrItem->setText(0,"LOCAL TROUBLES");
	ui.treeWidget->addTopLevelItem(localTrItem);


	QTreeWidgetItem *remoteTrItem = new QTreeWidgetItem();

	remoteTrItem->setIcon(0,QIcon(":/pro/Resourcs/icons/trouble-icon-48.png"));
	remoteTrItem->setData(0,ItemPointerData,quintptr(pZC));
	remoteTrItem->setData(0,ItemTypeData,QString("NULL"));
	remoteTrItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	remoteTrItem->setText(0,"Roemote TROUBLES");
	ui.treeWidget->addTopLevelItem(remoteTrItem);

	QTreeWidgetItem *sysStateItem = new QTreeWidgetItem();

	sysStateItem->setIcon(0,QIcon(":/pro/Resources/icons/sys-stat-icon-48.png"));
	sysStateItem->setData(0,ItemPointerData,quintptr(pZC));
	sysStateItem->setData(0,ItemTypeData,QString("NULL"));
	sysStateItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	sysStateItem->setText(0,"System Status");
	ui.treeWidget->addTopLevelItem(sysStateItem);

	for(int n=0; n< nIO; n++)
	{
		QString type =pZC->getIO(n)->getProperty("Type").value.toString();
		if(type == "ZC_STATS")
			ui.treeWidget->addNode(sysStateItem,pZC->getIO(n));
		else if(type == "ZC_REMOTE_TROUBLE")
			ui.treeWidget->addNode(remoteTrItem,pZC->getIO(n));
		else if(type == "ZC_LOCAL_TROUBLES")
			ui.treeWidget->addNode(localTrItem,pZC->getIO(n));
		else if(type == "ZC_RELAY")
			ui.treeWidget->addNode(outputItem,pZC->getIO(n));
		else if(type == "ZC_INPUTS")
			ui.treeWidget->addNode(inputItem,pZC->getIO(n));

	}


	m_pwioItem = new QTreeWidgetItem();

	m_pwioItem->setIcon(0,QIcon(":/pro/Resources/icons/wio-icon-48.png"));
	m_pwioItem->setData(0,ItemPointerData,(quintptr(pZC)));
	m_pwioItem->setData(0,ItemTypeData,QString("NULL"));
	m_pwioItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	m_pwioItem->setText(0,"WIO");
	ui.treeWidget->addTopLevelItem(m_pwioItem);

	int nWio = pZC->countWio();
	for(int n=0; n< nWio; n++)
	{		
		ui.treeWidget->addNode(m_pwioItem,pZC->getWio(n));
	}
	
	for (int i=11;i<27;i++)
		ui.comboBox->addItem("Channel "+QString::number(i),i);

	connect(ui.pbAdd,SIGNAL(clicked()),this , SLOT(onAdd()));
	connect(ui.pbDelete,SIGNAL(clicked()),this , SLOT(onDelete()));
	connect(ui.pbEdit,SIGNAL(clicked()),this , SLOT(OnEdit()));
	connect(ui.okButton,SIGNAL(clicked()),this , SLOT(onOk()));
	connect(ui.cancelButton,SIGNAL(clicked()),this , SLOT(onCancle()));

}

void FLCreateZCDlg::onAdd()
{
	FLCreateWioDlg dlg(m_pZC,this);
	if(dlg.exec() == QDialog::Accepted)
	{
		auto wio = dlg.WIO();
		if(m_bEdit)
		{
			list_wio_added.append(wio);			
		}
		else
		{
			m_pZC->addWio(wio);
		}
		
		wio->addProperty("Channel","Channel "+ QString::number(m_pZC->getProperty("Channel").value.toInt()),false);
		
		QTreeWidgetItem*pItem = new QTreeWidgetItem();

		pItem->setIcon(0,QIcon(":/pro/Resources/icons/wio-icon-48.png"));
		pItem->setData(0,ItemPointerData,(quintptr(wio)));
		pItem->setData(0,ItemTypeData,QString("NULL"));
		pItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
		pItem->setText(0,wio->getProperty("Name").value.toString());

		m_pwioItem->addChild(pItem);
	}
}

void FLCreateZCDlg::onDelete()
{

}

void FLCreateZCDlg::OnEdit()
{

}

void FLCreateZCDlg::onOk()
{
	QStringList lt;
	for (int n = 0; n < ui.comboBox_2->count(); n++)
	{
		lt << ui.comboBox_2->itemText(n);
	}
	m_pZC->addProperty("Name",ui.leName->text());
	m_pZC->addProperty("Location",ui.leLocation->text());
	QString strmac = ui.leMAC->text();
	QStringList slt = strmac.split(":");
	m_pZC->addProperty("MAC Address","MAC",slt);
	m_pZC->addProperty("PAN ID",ui.comboBox_2->currentText(), lt);
	m_pZC->addProperty("Range",ui.leRange->text().toInt(),true);
	QStringList channellist ;
	for (int i=11;i<27;i++)
		channellist << "Channel "+QString::number(i) ;
	m_pZC->addProperty("Channel","Channel "+ QString::number(ui.comboBox->currentIndex()+11),channellist);
	if(m_bEdit)
	{
		foreach(auto wio, list_wio_added)
			m_pZC->addWio(wio);
	}
	QDialog::accept();
}

void FLCreateZCDlg::onCancle()
{
	if(m_bEdit==false)
		delete m_pZC;
	QDialog::reject();
}
