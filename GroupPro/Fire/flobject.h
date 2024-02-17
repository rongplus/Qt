/***************************************************************************************
*                                                                                      *
*	   **************************************************************************      *
*	   **            Company Confidential - For Internal Use Only              **      *
*	   **                   Mircom Group of Companies("MGC")     	           **      *
*	   **                                                                      **      *
*	   **   This information is confidential  and the exclusive property of    **      *
*	   ** Mircom.  It is intended for internal use and only for the purposes   **      *
*	   **   provided,  and may not be disclosed to any third party without     **      *
*	   **                prior written permission from Mircom.                 **      *
*	   **                                                                      **      *
*	   **                          Copyright 2016                              **      *
*	   **                            Rong Huang(ron)                           **      *
*	   **************************************************************************      *
*																					   *
***************************************************************************************/
#ifndef FLOBJECT_H
#define FLOBJECT_H

#include <QObject>
#include <QVariant>

#define ItemTypeData Qt::UserRole
#define ItemPointerData Qt::UserRole+1

const QString NAME = "Name";
const QStringList PanID_List = {"1126","1225","1324","1423","1522","1621","1720","1819","1918","2017","2116","2215","2314","2413","2512","2611"};

struct FLObjectPropertys
{
	bool bEditable;
	QVariant value;
	QStringList items;
};
/*
enum EProperty_key
{
	ENAME = 0,
	EGUID,
	ECOMMENTS,
	EJOBPATH,
	ETYPE,
	ESTANDARD
};*/

class FLObject : public QObject
{
	Q_OBJECT

public:
	FLObject(QObject *parent);
	~FLObject();

	FLObjectPropertys getProperty(QString strKey);

	bool hasKey(QString strKey);

	virtual void Init();
	QMap<QString,FLObjectPropertys >  Properties();

	void addProperty(QString strKey,const QVariant & value, bool bEditAble = true);	
	void addProperty(QString strKey,const QVariant & value, QStringList &slt, bool bEditAble = true);

	void InitGuid();
protected:
	
	//QList<QVariant>, 1, Editable(true/false), 2, value(items size !=0, string for content), 3, items
	QMap<QString,FLObjectPropertys > m_property;

signals:
	void propertyChanged(QString);
	void childrenChanged(FLObject *,int);
private:
	
};

#endif // FLOBJECT_H
