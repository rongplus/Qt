#ifndef MYMODEL_H
#define MYMODEL_H

#include <QObject>
#include <QStandardItemModel>


class MyDB : public QObject
{
	Q_OBJECT
public:
	MyDB( QObject* p = nullptr);
	~MyDB();

	QStringList _head_data;
	QList< QList<QVariant>> _data;
};

class MyModel : public QStandardItemModel
{
	Q_OBJECT
public:
	MyModel(QObject *parent = Q_NULLPTR);

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const ;
	virtual QVariant data(int row, int col, int role = Qt::DisplayRole) const ;

	void setDB(MyDB* db);
	//check all datas
	bool isEdited();
	//check row datas
	bool isEdited(int row);
	//check cell data
	bool isEdited(int row,int col);
	int a = 0;

private:
	MyDB* _db;
};

#endif // MYMODEL_H
