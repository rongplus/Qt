#include "flcreatejobdlg.h"

#include <QSettings>
#include <QMessageBox>
#include <QFileDialog>
#include "flcreatefloordlg.h"
#include "flcreatezcdlg.h"
#include "flcreatewiodlg.h"
#include "flglobal.h"

FLCreateJobDlg::FLCreateJobDlg(FLJob* pJob,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QSettings st("Mircom","FireLink");
	QString str = st.value("Default_Job_Path").toString();
	if(str.isNull() == false)
		ui.leJobPath->setText(str);
	m_bEditJob = false;

	if(pJob)
	{
		m_pJob = pJob;
		m_bEditJob = true;
		setWindowTitle("Edit Job");
	}
	else
	{
		m_pJob = new FLJob(nullptr);	
		//m_pJob->addProperty("JobPath",str+"/" + ui.leJobName-> text()+".bin");
		m_pJob->addProperty("Name","New_Job");
		m_pJob->Init();
		
	}
	Init(m_pJob);
	

	connect(ui.pbAddFloor,SIGNAL(clicked()),this , SLOT(onAddFloor()));
	connect(ui.pbDeleteFloor,SIGNAL(clicked()),this , SLOT(onDeleteFloor()));
	connect(ui.pbEditFloor,SIGNAL(clicked()),this , SLOT(OnEditFllor()));
	connect(ui.pbJobPath,SIGNAL(clicked()),this , SLOT(onSelectPath()));
	//connect(ui.pbJobGraph,SIGNAL(clicked()),this , SLOT(onSelectGraph()));
	connect(ui.okButton,SIGNAL(clicked()),this , SLOT(onOk()));
	connect(ui.cancelButton,SIGNAL(clicked()),this , SLOT(onCancle()));

}

FLCreateJobDlg::~FLCreateJobDlg()
{

}

void FLCreateJobDlg::Init(FLJob* pJob)
{
	ui.leJobName->setText( pJob->getProperty("Name").value.toString());
	ui.leJobPath->setText(pJob->getProperty("JobPath").value.toString());
	if(ui.leJobPath->text() == "")
	{
		QSettings setting("Mircom","FireLink");
		ui.leJobPath->setText(setting.value("Default_Job_Path").toString() );
	}
	else
	{
		ui.leJobPath->setText(pJob->getProperty("JobPath").value.toString());
	}
	ui.cbJobStandard->setCurrentText(pJob->getProperty("Standard").value.toString());
	ui.leJobComments->setText(pJob->getProperty("Comments").value.toString());
	ui.treeWidget->addNode(pJob);
}

void FLCreateJobDlg::onSelectPath()
{
	QString str_file_name = QFileDialog::getExistingDirectory(this,"Save to",ui.leJobPath->text());
	if(str_file_name!="")
		ui.leJobPath->setText(str_file_name);
}

void FLCreateJobDlg::onSelectGraph()
{

}

void FLCreateJobDlg::onAddFloor()
{
	auto item = ui.treeWidget->currentItem();
	if( item == nullptr || (QObject*)item->data(0,ItemPointerData).toULongLong() == m_pJob)
	{
		FLCreateFloorDlg dlg(m_pJob,false);
		if(dlg.exec() == QDialog::Accepted)
		{
			if(m_bEditJob)
			{
				list_floor_added.append(dlg.Floor());			
			}
			else
			{
				m_pJob->addFloor(dlg.Floor());			
			}
			auto item = ui.treeWidget->topLevelItem(0);
			ui.treeWidget->addNode(item,dlg.Floor());

			ui.treeWidget->expandItem(item);

		}
		return;
	}
	auto node = qobject_cast<FLObject* > ((QObject*)item->data(0,ItemPointerData).toULongLong() );
	if(node == nullptr)
		return;

	QString type = node->getProperty("Type").value.toString();

	if(type == "FLOOR")
	{
		auto floor = (FLFloor*) node;
		FLCreateZCDlg dlg(false,floor,this);
		if(dlg.exec() == QDialog::Accepted)
		{
			
			floor->addZC(dlg.ZC());
			ui.treeWidget->addNode(item,dlg.ZC());
			ui.treeWidget->expandItem(item);
		}
	}
	else if(type == "ZC")
	{
		auto zc = (FLZoneControllor*) node;
		if (zc->countWio() == 49)
		{
			QMessageBox::warning(this, "Number limited", "The maximum number of WIO is 49");
			return;
		}
		FLCreateWioDlg dlg(zc,this);
		if(dlg.exec() == QDialog::Accepted)
		{
			auto lt = dlg.WIO_list();
			foreach (auto wio , lt )
			{
			zc->addWio(wio);
			ui.treeWidget->addNode(item->child(5),wio);
			ui.treeWidget->expandItem(item);
			}
			
		}
	}
}

void FLCreateJobDlg::onDeleteFloor()
{

}

void FLCreateJobDlg::OnEditFllor()
{
	QTreeWidgetItem* pItem = ui.treeWidget->currentItem();
	if(pItem==nullptr)
		return;
	FLObject *node = qobject_cast<FLObject*>((QObject*)pItem->data(0,ItemPointerData).toULongLong());
	QString type = node->getProperty("Type").value.toString();
	if ( type == "FLOOR") 
	{
		FLCreateFloorDlg dlg((FLFloor*)node,false,this);
		if(dlg.exec() == QDialog::Accepted)
		{
			//m_pJob->addFloor(dlg.Floor());
			pItem->setText(0,dlg.Floor()->getProperty("Name").value.toString());
		}
	}
	else if(type == "ZC")
	{
		FLCreateZCDlg dlg(false,(FLZoneControllor*)node,this);
		if(dlg.exec() == QDialog::Accepted)
		{
			//m_pJob->addFloor(dlg.Floor());
			pItem->setText(0,dlg.ZC()->getProperty("Name").value.toString());
		}
	}
	else if(type == "WIO")
	{
		FLCreateWioDlg dlg((FLWio*)node,this);
		if(dlg.exec() == QDialog::Accepted)
		{
			//m_pJob->addFloor(dlg.Floor());
			pItem->setText(0,dlg.WIO()->getProperty("Name").value.toString());
		}
	}
}

void FLCreateJobDlg::onOk()
{
	if( FLGlobal::VerifyInputContent(ui.leJobName,this) == false || FLGlobal::VerifyInputContent(ui.leJobPath,this) == false)
		return;
	QString file_path = ui.leJobPath->text() + "/" + ui.leJobName->text() + ".bin";
	if( QFile::exists(file_path) == true)
	{
		QMessageBox::warning(this,"File existed","Please choose a new directory or job name");
		return;
	}
	m_pJob->addProperty("Name",ui.leJobName->text());
	m_pJob->addProperty("Standard",ui.cbJobStandard->currentText(),QStringList() << "ULC(Canada)" << "UL(United States)");
	m_pJob->addProperty("Comments",ui.leJobComments->toPlainText());
	m_pJob->addProperty("JobPath",file_path,false);

	if(m_bEditJob)
	{
		foreach(auto floor, list_floor_added)
			m_pJob->addFloor(floor);
	}

	QSettings st("Mircom","FireLink");
	if(ui.checkBox->isChecked())
		st.setValue("Default_Job_Path",ui.leJobPath->text());		

	QDialog::accept();
}

void FLCreateJobDlg::onCancle()
{
	if(m_bEditJob== false)
		delete m_pJob;
	QDialog::reject();
}

void FLCreateJobDlg::on_item_clicked(QTreeWidgetItem* item,int col)
{
	auto node = qobject_cast<FLObject* > ((QObject*)item->data(0,ItemPointerData).toULongLong() );
	if(node==nullptr)
	{
		ui.pbAddFloor->setEnabled(false);		
		ui.pbDeleteFloor->setEnabled(false);		
		ui.pbEditFloor->setEnabled(false);
		return;
	}
	QString type = node->getProperty("Type").value.toString();
	ui.pbAddFloor->setEnabled(true);
	if(type == "JOB")
	{
		ui.pbAddFloor->setText("Add Floor");
		ui.pbEditFloor->setEnabled(false);
		ui.pbDeleteFloor->setEnabled(false);
	}
	else if(type == "FLOOR")
	{
		ui.pbAddFloor->setText("Add ZC");
		ui.pbDeleteFloor->setEnabled(true);
		ui.pbDeleteFloor->setText("Delete Floor");

		ui.pbEditFloor->setEnabled(true);
		ui.pbEditFloor->setText("Edit Floor");
	}
	else if(type == "ZC")
	{
		ui.pbAddFloor->setText("Add WIO");
		ui.pbDeleteFloor->setEnabled(true);
		ui.pbDeleteFloor->setText("Delete ZC");

		ui.pbEditFloor->setEnabled(true);
		ui.pbEditFloor->setText("Edit ZC");
	}
	else if(type == "WIO")
	{
		ui.pbAddFloor->setEnabled(false);
		ui.pbAddFloor->setText("Add ZC");
		ui.pbDeleteFloor->setEnabled(true);
		ui.pbDeleteFloor->setText("Delete WIO");

		ui.pbEditFloor->setEnabled(true);
		ui.pbEditFloor->setText("Edit WIO");
	}
	else
	{
		ui.pbAddFloor->setEnabled(false);		
		ui.pbDeleteFloor->setEnabled(false);		
		ui.pbEditFloor->setEnabled(false);
	}
}

void FLCreateJobDlg::on_jobname_changed(QString str)
{
	auto item = ui.treeWidget->topLevelItem(0);
	//ui.leJobPath->setText(pJob->getProperty("JobPath").value.toString());

	if(item )item->setText(0,str);
}
