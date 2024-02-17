#include "flpropertytable.h"
#include "flobject.h"

#include <QHeaderView>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDebug>
#include <QCheckBox>
#include <QVBoxLayout>
#include "flwio.h"

FLPropertyTable::FLPropertyTable(QWidget *parent)
	: QTableWidget(parent)
{
	setColumnCount(2);
	setHorizontalHeaderLabels(QStringList() << "Property" << "Value");		
	horizontalHeader()->setStretchLastSection(true);
	verticalHeader()->setHidden(true);
	horizontalHeader()->setVisible(true);
	setColumnWidth(0,100);
	flobject_node = nullptr;

}

FLPropertyTable::~FLPropertyTable()
{

}

void FLPropertyTable::setNode(FLObject *node)
{	
	clear();
	setRowCount(0);
	setHorizontalHeaderLabels(QStringList() << "Property" << "Value");

	if(node==nullptr)
		return;
	flobject_node = node;
	

	int i=0;
	QMap<QString, FLObjectPropertys> pro = node->Properties();
	QMapIterator<QString,FLObjectPropertys> it(pro);
	while(it.hasNext())
	{
		it.next();
		insertRow(i);
		QTableWidgetItem* titleItem = new QTableWidgetItem(it.key());
		titleItem->setFlags( titleItem->flags()^Qt::ItemIsEditable);
		setItem(i,0,titleItem);
		FLObjectPropertys item_property = it.value();
		//qDebug() << "property name" << it.key();
		//qDebug() <<"property type" << item_property.value.type(); 
		
		if(item_property.items.size() !=0 )
		{
			//qDebug() << "items " << item_property.items;
			QStringList vList = item_property.items;
			if (it.key() == "SCU_FEATURE_FLAGS") {

				QVBoxLayout *l = new QVBoxLayout();
				

				QWidget *w = new QWidget();

				

				for (int n = 0; n < vList.size()-1; n++)
				{
					QCheckBox *pCheckBox = new QCheckBox(this);
					pCheckBox->setText(vList[n]);
					l->addWidget(pCheckBox);

					pCheckBox->setProperty("RowIndex", i);
					if (item_property.value.toString().contains(vList[n]))
					{
						pCheckBox->setChecked(true);
					}

					connect(pCheckBox, SIGNAL(clicked()), this, SLOT(on_setff()));
				}
				
				w->setLayout(l);

				setCellWidget(i, 1, w);
				setRowHeight(i, vList.size() * 20);
				
			}
			else if(item_property.value.toString() == "MAC")
			{
				QLineEdit *pEdit = new QLineEdit(this);
				pEdit->setInputMask("NN:NN:NN:NN:NN:NN:NN:NN");
				
				if(vList.size()==8)
				{
					QString  str_mac;
					foreach(auto s,vList)
					{
						str_mac += s;
						if(s!=vList[7])
							str_mac+=":";
					}
					pEdit->setText(str_mac);
				}
				else
					pEdit->setText("FF:FF:FF:FF:FF:FF:FF:FF");
				setCellWidget(i,1,pEdit);
				//connect(pEdit,SIGNAL(	textChanged(const QString & )), this,SLOT(editChanged(const QString &)));
				connect(pEdit, SIGNAL(editingFinished()), this, SLOT(on_edit_finished()));
				pEdit->setEnabled(item_property.bEditable);
				pEdit->setProperty("RowIndex",i);
			}
			else
			{
				auto wio = qobject_cast<FLWio*>(flobject_node);

				QComboBox *pCom = new QComboBox(this);
				for(int n=0; n< vList.size(); n++)
					pCom->addItem(vList[n]);
				if(item_property.value.type()== QVariant::Int)
					pCom->setCurrentIndex(item_property.value.toInt());
				else
					pCom->setCurrentText(item_property.value.toString());
				setCellWidget(i,1,pCom);
				pCom->setEnabled(item_property.bEditable);
				pCom->setProperty("RowIndex",i);
				if(wio && it.key() == "Signaling")
				{
					QStringList mode_list;
					mode_list  << "Mircom" <<	"Cooper" <<"System Sensor" << "Gentex" << "Potter";
					if( mode_list.contains( wio->Model()) )
						pCom->setEnabled(true);
					else
						pCom->setEnabled(false);
				}
				connect(pCom,SIGNAL(	currentTextChanged(const QString & )),this ,SLOT( comboChanged(const QString &)));
			}
		}
		else
		{

			if (it.key() == "Corrs") {

				QPushButton *pButton = new QPushButton(this);
				pButton->setText("Set");
				setCellWidget(i, 1, pButton);

				connect(pButton, SIGNAL(clicked()), this, SLOT(on_setcorrs()));
			}
			
			else
			{
				QLineEdit *pEdit = new QLineEdit(this);
				//pEdit->setInputMask("FF:FF:FF:FF");
				if (item_property.value.type() == QVariant::PointF)
				{
					pEdit->setText(QString("%1,%2").arg(item_property.value.toPointF().x()).arg(item_property.value.toPointF().y()));
				}
				else
					pEdit->setText(item_property.value.toString());
				pEdit->setEnabled(item_property.bEditable);
				setCellWidget(i, 1, pEdit);
				//connect(pEdit,SIGNAL(	textChanged(const QString & )), this,SLOT(editChanged(const QString &)));
				connect(pEdit, SIGNAL(editingFinished()), this, SLOT(on_edit_finished()));
				pEdit->setProperty("RowIndex", i);
			}
		}		
		i++;		
	}
	return;
}

void FLPropertyTable::on_setcorrs()
{

}

void FLPropertyTable::on_setff()
{
	auto l = (QVBoxLayout*) (sender()->parent());
	int n_index = sender()->property("RowIndex").toInt();
	auto table_item = item(n_index, 0);
	QString str = ((QCheckBox*)sender())->text();
	
	QStringList clist;

	QWidget *pw = ((QCheckBox*)sender())->parentWidget();
	if (pw == NULL)
		return;

	foreach(QWidget *w, pw->findChildren<QWidget*>())
	{
		clist << ((QCheckBox*)w)->text();
	}

	clist << clist.join('|');

	flobject_node->addProperty(table_item->text(), str, clist, true);
	
}
void FLPropertyTable::SetJobNode(FLObject* node)
{
}

void FLPropertyTable::SetFloorNode(FLObject *node)
{
}

void FLPropertyTable::SetZCNode(FLObject* node)
{
}

void FLPropertyTable::editChanged(const QString &str )
{
	if (str.size() <= 0)
		return;
	qDebug() << "Edit changed to " << str ;
	auto edit = (QLineEdit*) sender();
	int n_index = edit->property("RowIndex").toInt();
	auto table_item = item(n_index,0);
	if (edit->inputMask() == "NN:NN:NN:NN:NN:NN:NN:NN")
	{
		//str = str.toUpper();
		QRegExp mac("[A-F]");
		QStringList mac_addr = str.split(":");
		for (size_t i = 0; i < mac_addr.length(); i++)
		{
			auto s = mac_addr[i].toUpper();
			if (s.size() == 0)
				s = "FF";
			else if (s.size() == 1)
				s = "0" + s;

			if (s.at(0) < '0')
				s[0] = '0';
			if (s.at(0) > 'F')
				s[0] = 'F';

			if (s.at(1) < '0')
				s[1] = '0';
			if (s.at(1) > 'F')
				s[1] = 'F';

			mac_addr[i] = s;

		}
		QString new_add;
		size_t i;
		for ( i = 0; i < mac_addr.length()-1; i++)
		{
			new_add += mac_addr[i] + ":";
		}
		new_add += mac_addr[i] ;
		
		flobject_node->addProperty( table_item->text(),"MAC",mac_addr);
		edit->setText(new_add);
	}
	else
		flobject_node->addProperty( table_item->text(),str,true);
}


void FLPropertyTable::comboChanged(const QString &str)
{
	qDebug() << "combo changed to " << str ;
	auto edit = (QComboBox*) sender();
	int n_index = edit->property("RowIndex").toInt();
	auto table_item = item(n_index,0);
	auto item_text = table_item->text();	

	auto wio = qobject_cast<FLWio*>(flobject_node);
	if(wio && table_item->text() == "Model")
	{
		QStringList mode_list;
		mode_list  << "Mircom" <<	"Cooper" <<"System Sensor" << "Gentex" << "Potter";
		QComboBox* pCom = nullptr;
		for(int n=0; n< rowCount(); n++)
		{
			auto signal_item = item(n,0);
			if(signal_item->text()=="Signaling")
			{
				pCom = qobject_cast<QComboBox*>( cellWidget(n,1) );
				break;
			}
		}
		if(pCom)
		{
			if( mode_list.contains( str ) )
				pCom->setEnabled(true);
			else
				pCom->setEnabled(false);
		}
		
	}
	
	QStringList list_combo;

	for(int n=0; n< edit->count(); n++)
	{
		list_combo << edit->itemText(n);
	}
	flobject_node->addProperty(table_item->text(),str,list_combo,true);	
}

void FLPropertyTable::on_edit_finished()
{
	auto edit = (QLineEdit*)sender();
	auto str = edit->text();
	if (str.size() <= 0)
		return;
	qDebug() << "Edit changed to " << str;
	
	int n_index = edit->property("RowIndex").toInt();
	auto table_item = item(n_index, 0);
	if (edit->inputMask() == "NN:NN:NN:NN:NN:NN:NN:NN")
	{
		//str = str.toUpper();
		QRegExp mac("[A-F]");
		QStringList mac_addr = str.split(":");
		for (size_t i = 0; i < mac_addr.length(); i++)
		{
			auto s = mac_addr[i].toUpper();
			if (s.size() == 0)
				s = "FF";
			else if (s.size() == 1)
				s = "0" + s;

			if (s.at(0) < '0')
				s[0] = '0';
			if (s.at(0) > 'F')
				s[0] = 'F';

			if (s.at(1) < '0')
				s[1] = '0';
			if (s.at(1) > 'F')
				s[1] = 'F';

			mac_addr[i] = s;

		}
		QString new_add;
		size_t i;
		for (i = 0; i < mac_addr.length() - 1; i++)
		{
			new_add += mac_addr[i] + ":";
		}
		new_add += mac_addr[i];

		flobject_node->addProperty(table_item->text(), "MAC", mac_addr);
		edit->setText(new_add);
	}
	else
		flobject_node->addProperty(table_item->text(), str, true);
}
