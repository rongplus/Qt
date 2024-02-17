#include "flcreatefloordlg.h"
#include "flcreatezcdlg.h"
#include <QFileDialog>
#include <QSvgRenderer> 

FLCreateFloorDlg::FLCreateFloorDlg(FLJob* pJob,bool show_add_group,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);	
	ui.groupBox_2->setVisible(show_add_group);
	if(show_add_group == false)
		resize(width(),height()-160);
	m_pFloor = new FLFloor(pJob);
	int num_floor = pJob->countFloor()+1;
	
	m_pFloor->Init();
	m_pFloor->addProperty("Name","New_Floor_" + QString::number(num_floor));
	
	Init(m_pFloor);
}


FLCreateFloorDlg::FLCreateFloorDlg(FLFloor* pFloor,bool show_add_group,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.groupBox_2->setVisible(show_add_group);
	if(show_add_group == false)
		resize(width(),height()-160);
	m_pFloor = pFloor;
	m_bEdit = true;
	setWindowTitle("Edit Floor");	
	
	Init(m_pFloor);
}

FLCreateFloorDlg::~FLCreateFloorDlg()
{

}

void FLCreateFloorDlg::Init(FLFloor* pFloor)
{
	FLObjectPropertys item_property = pFloor->getProperty("Name");
	ui.leName->setText(item_property.value.toString());
	item_property = pFloor->getProperty("FloorGraph");
	
	QPixmap pix(item_property.value.toString());
	ui.lbl_floor_width->setText( "Width =" + QString::number(pix.width()));
	ui.lbl_floor_height->setText( "Height = " + QString::number(pix.height()));

	ui.leGraph->setText(item_property.value.toString());
	item_property = pFloor->getProperty("Comments");
	ui.textEdit->setText( item_property.value.toString());
	item_property = pFloor->getProperty("Plotting_Scale");
	ui.cb_scale->setCurrentText(item_property.value.toString());
	
	int nZC = pFloor->countZC();
	for(int n=0; n< nZC; n++)
	{
		AddZC(pFloor->getZC(n));
	}

	connect(ui.pbAdd,SIGNAL(clicked()),this , SLOT(onAdd()));
	connect(ui.pbDelete,SIGNAL(clicked()),this , SLOT(onDelete()));
	connect(ui.pbEdit,SIGNAL(clicked()),this , SLOT(OnEdit()));
	connect(ui.pushButton,SIGNAL(clicked()),this , SLOT(onSelectGraph()));
	//connect(ui.pbJobGraph,SIGNAL(clicked()),this , SLOT(onSelectGraph()));
	connect(ui.okButton,SIGNAL(clicked()),this , SLOT(onOk()));
	connect(ui.cancelButton,SIGNAL(clicked()),this , SLOT(onCancle()));
}

void FLCreateFloorDlg::AddZC(FLZoneControllor* pZC)
{
	QTreeWidgetItem *zcItem = new QTreeWidgetItem();

	zcItem->setIcon(0,QIcon(":/firelink/Resources/icons/zc-icon-48.png"));
	zcItem->setData(0,ItemTypeData,QString("ZONECONTROL"));
	zcItem->setData(0,ItemPointerData,(quintptr(pZC)));
	zcItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	FLObjectPropertys property_zc = pZC->getProperty("Name");
	zcItem->setText(0,property_zc.value.toString());
	ui.treeWidget->addTopLevelItem(zcItem);
	

	int nIO = pZC->countIO();

	QTreeWidgetItem *inputItem = new QTreeWidgetItem();
	inputItem->setIcon(0,QIcon(":/firelink/Resources/icons/input-icon-48.png"));
	zcItem->setData(0,ItemTypeData,QString("NULL"));
	inputItem->setData(0,ItemPointerData,(quintptr(pZC)));
	inputItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	inputItem->setText(0,"Input");
	zcItem->addChild(inputItem);

	QTreeWidgetItem *outputItem = new QTreeWidgetItem();
	outputItem->setIcon(0,QIcon(":/firelink/Resources/icons/relay-icon-48.png"));
	outputItem->setData(0,ItemTypeData,QString("NULL"));
	outputItem->setData(0,ItemPointerData,(quintptr(pZC)));
	outputItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	outputItem->setText(0,"RELAYS");
	zcItem->addChild(outputItem);

	QTreeWidgetItem *localTrItem = new QTreeWidgetItem();
	localTrItem->setIcon(0,QIcon(":/firelink/Resources/icons/trouble-icon-48.png"));
	localTrItem->setData(0,ItemTypeData,QString("NULL"));
	localTrItem->setData(0,ItemPointerData,(quintptr(pZC)));
	localTrItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	localTrItem->setText(0,"LOCAL TROUBLES");
	zcItem->addChild(localTrItem);


	QTreeWidgetItem *remoteTrItem = new QTreeWidgetItem();

	remoteTrItem->setIcon(0,QIcon(":/firelink/Resourcs/icons/trouble-icon-48.png"));
	remoteTrItem->setData(0,ItemPointerData,(quintptr(pZC)));
	remoteTrItem->setData(0,ItemTypeData,QString("NULL"));
	remoteTrItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	remoteTrItem->setText(0,"Roemote TROUBLES");
	zcItem->addChild(remoteTrItem);

	QTreeWidgetItem *sysStateItem = new QTreeWidgetItem();

	sysStateItem->setIcon(0,QIcon(":/firelink/Resources/icons/sys-stat-icon-48.png"));
	sysStateItem->setData(0,ItemPointerData,(quintptr(pZC)));
	sysStateItem->setData(0,ItemTypeData,QString("NULL"));
	sysStateItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	sysStateItem->setText(0,"System Status");
	zcItem->addChild(sysStateItem);

	for(int n=0; n< nIO; n++)
	{
		FLObjectPropertys item_property = pZC->getIO(n)->getProperty("Type");
		QString type_name = item_property.value.toString();
		if(type_name == "ZC_STATS")
			ui.treeWidget->addNode(sysStateItem,pZC->getIO(n));
		else if(type_name == "ZC_REMOTE_TROUBLE")
			ui.treeWidget->addNode(remoteTrItem,pZC->getIO(n));
		else if(type_name == "ZC_LOCAL_TROUBLES")
			ui.treeWidget->addNode(localTrItem,pZC->getIO(n));
		else if(type_name == "ZC_RELAY")
			ui.treeWidget->addNode(outputItem,pZC->getIO(n));
		else if(type_name == "ZC_INPUTS")
			ui.treeWidget->addNode(inputItem,pZC->getIO(n));	
		else 
			ui.treeWidget->addNode(zcItem,pZC->getIO(n));
	}


	QTreeWidgetItem *wioItem = new QTreeWidgetItem();

	wioItem->setIcon(0,QIcon(":/firelink/Resources/icons/wio-icon-48.png"));
	wioItem->setData(0,ItemPointerData,(quintptr(pZC)));
	wioItem->setData(0,ItemTypeData,QString("NULL"));
	wioItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
	wioItem->setText(0,"WIO");
	zcItem->addChild(wioItem);

	int nWio = pZC->countWio();
	for(int n=0; n< nWio; n++)
	{		
		ui.treeWidget->addNode(wioItem,pZC->getWio(n));
	}
}

void FLCreateFloorDlg::onSelectGraph()
{
	auto file = QFileDialog::getOpenFileName();
	if (file == "")
		return;
	QSvgRenderer gg(file);
	ui.lbl_floor_width->setText("Width = " + QString::number(gg.viewBox().width()));
	ui.lbl_floor_height->setText("Width = " + QString::number(gg.viewBox().height()));
	ui.leGraph->setText(file);
	
}

void FLCreateFloorDlg::onAdd()
{
	FLCreateZCDlg dlg(false,m_pFloor,this);
	if(dlg.exec() ==QDialog::Accepted)
	{
		m_pFloor->addZC(dlg.ZC());
		AddZC(dlg.ZC());
	}
}

void FLCreateFloorDlg::onDelete()
{

}

void FLCreateFloorDlg::OnEdit()
{
	QTreeWidgetItem* pItem = ui.treeWidget->currentItem();

	if(pItem == nullptr)
		return;

	FLObject *node = qobject_cast<FLObject*>((QObject*)pItem->data(0,ItemPointerData).toULongLong());
	QString type = node->getProperty("Type").value.toString();
	if ( type == "ZC") 
	{
		FLCreateZCDlg dlg(false,(FLZoneControllor*)node,this);
		dlg.exec();
	}

	
}

void FLCreateFloorDlg::onOk()
{
	m_pFloor->addProperty("Name",ui.leName->text());
	m_pFloor->addProperty("FloorGraph",ui.leGraph->text());
	m_pFloor->addProperty("Comments",ui.textEdit->toPlainText());
	m_pFloor->addProperty("unit-of-measure",ui.comboBox->currentText(),QStringList() <<"Meters" << "Yards"<<"Feet"<<"Inches"<<"Centimeters");
	QStringList str_plotting_scale;
	str_plotting_scale<<"1:1"<<"1:2"<<"1:5"<<"1:10" << "1:20" << "1:50" << "1:100" << "1:200" << "1:500" << "1:1000" << "1:2000" << "1:5000";
	m_pFloor->addProperty("Plotting_Scale",ui.cb_scale->currentText(),str_plotting_scale, true);

	QDialog::accept();
}

void FLCreateFloorDlg::onCancle()
{
	if(m_bEdit==false)
		delete m_pFloor;
	QDialog::reject();
}
