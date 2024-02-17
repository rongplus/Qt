#include "flobject.h"
#include <QUuid>
FLObject::FLObject(QObject *parent)
	: QObject(parent)
{
	
}

FLObject::~FLObject()
{

}

void FLObject::addProperty(QString strKey,  const QVariant & value,bool bEditAble)
{
	bool hasKey = m_property.contains(strKey);
	FLObjectPropertys p;
	p.bEditable = bEditAble;
	p.value = value;
	m_property[strKey] =  p;
	if(hasKey)
		emit propertyChanged(strKey);
}

void FLObject::addProperty(QString strKey,const QVariant & value, QStringList &slt, bool bEditAble /*= true*/)
{
	bool hasKey = m_property.contains(strKey);
	if (hasKey)
	{
		Q_ASSERT(value.type() == m_property[strKey].value.type());
	}
	FLObjectPropertys p;
	p.bEditable = bEditAble;
	p.value = value;
	p.items = slt;
	m_property[strKey] =  p;
	if(hasKey)
		emit propertyChanged(strKey);
}

void FLObject::InitGuid()
{
	addProperty("GUID",QUuid::createUuid().toString().replace("{","").replace("}",""),false);
}

FLObjectPropertys FLObject::getProperty(QString strKey)
{
	return m_property[strKey];
}

bool FLObject::hasKey(QString strKey)
{
	return m_property.contains(strKey);
}


void FLObject::Init()
{
	addProperty("Name","FLObject");
	addProperty("GUID",QUuid::createUuid().toString().replace("{","").replace("}",""),false);
	addProperty("Type","FLObject",false);
}

QMap< QString , FLObjectPropertys> FLObject::Properties()
{
	return m_property;
}
