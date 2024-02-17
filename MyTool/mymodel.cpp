#include "mymodel.h"
#include <QDate>
#include <QDebug>

MyModel::MyModel(QObject *parent)
	:QStandardItemModel(parent )
{

}



QVariant MyModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
	//NOTE: please fix at 9
	/*if(role == Qt::DisplayRole && orientation == Qt::Horizontal)
		return "Col" + QString::number(section);
	if(role == Qt::DisplayRole && orientation == Qt::Vertical)
		return "Col" + QString::number(section);*/
	return QStandardItemModel::headerData(section,orientation,role);
}

void MyModel::setDB( MyDB* db)
{
	_db = db;
	int col = 0;
	setColumnCount( db->_head_data.size());
	//insertRows(0,1);

	for(auto title : db->_head_data)
	{
		setHeaderData(col,Qt::Horizontal,title);
		col++;
	}
	int row = 0;
	col = 0;
	for(auto list : db->_data)
	{
		row = 0;
		for( auto value: list)
		{
			int n = rowCount();
			if(rowCount() <= row )
			{
				qDebug() << "row" << row;
				insertRows(row,1);
			}
			if(value.type() == QVariant::Bool)
				setData(index(row,col),value.toBool()?"Yes":"No");
			else
				setData(index(row,col),value);
			row ++;
		}
		col ++;
	}
}

QVariant MyModel::data(const QModelIndex &index, int role ) const
{
	/*index.row();
	index.column();
	if(role == Qt::DisplayRole)
		return QString::number(index.row()) + " - " + QString::number(index.column());*/
	return QStandardItemModel::data(index,  role );
}

bool MyModel::isEdited()
{
	int row;
	for (row=0; row<rowCount(); row++)
	{
		if( isEdited(row))
			return true;
	}
	return false;
}

bool MyModel::isEdited(int row)
{
	int  col;
	for (col=0; col< columnCount() ; col++)
	{
		if (isEdited(row,col) )
			return true;
	}
	return false;
}

bool MyModel::isEdited(int row,int col)
{
	return data(row,col,Qt::UserRole).toString().trimmed() != data(row,col,Qt::DisplayRole).toString().trimmed() ;
}

QVariant MyModel::data(int row, int col, int role ) const
{
	return QStandardItemModel::data(index(row,col),  role );
}

MyDB::MyDB(QObject *p)
	:QObject(p)
{

	QList<QVariant >  l1 = QList<QVariant >() <<	"const" << "GLenum ZERO" << "= 0" <<false;
	_data.append(l1);
	l1 = QList<QVariant >() << 	"const" << "GLenum ONE" << "= 1" << true;
	_data.append(l1);
	l1 = QList<QVariant >()<< 	"const" << "GLenum ONE" << "= 1" << true;
	_data.append(l1);
	l1 = QList<QVariant >() << 	"const" << "GLenum ONE" << "= 1" << false;
	_data.append(l1);

	_head_data << "d1" << "d2" << "d3" << "d4" << "d5";

}

MyDB::~MyDB(){
	//BUG
	//TODO
	//FIXME
}
