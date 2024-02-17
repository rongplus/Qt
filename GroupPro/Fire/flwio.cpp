#include "flwio.h"

#include <QPointF>
#include "flglobal.h"

FLWio::FLWio(QObject *parent)
	: FLObject(parent)
{

}

FLWio::~FLWio()
{

}

void FLWio::setLocation(QString strLocation)
{
	addProperty("Location", strLocation);

}

void FLWio::setMacAddress(QString strMAc)
{
	QStringList slt = strMAc.split(":",QString::SkipEmptyParts);
	if (slt.count() != 8)
	{
		for (int n = 0; n < 8 - slt.count(); n++)
			slt << "FF";
	}
	addProperty("MAC Address", "MAC", slt);
	
}
QString FLWio::MacAddress()
{
	QStringList vList = getProperty("MAC Address").items;
	if (vList.size() < 8)
		return "FF:FF:FF:FF:FF:FF:FF:FF";
	auto mac = QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(vList[0]).arg(vList[1]).arg(vList[2]).arg(vList[3]).arg(vList[4]).arg(vList[5]).arg(vList[6]).arg(vList[7]);
	return mac;
}
void FLWio::setShrtAddress(QString strShrt)
{
	addProperty("Short Address", strShrt);
}

void FLWio::setModel(int nModel)
{
	DeviceModels *g = FLGlobal::getDeviceModels();
	int nSize = g->dev_size();
	QStringList slt;
	for(int n=0; n< nSize; n++)
	{
		slt << Model(n);
	}
	addProperty("Model",Model(nModel),slt);
}

void FLWio::setModel(QString model)
{
	DeviceModels *g = FLGlobal::getDeviceModels();
	int nSize = g->dev_size();
	QStringList slt;
	for(int n=0; n< nSize; n++)
	{
		slt << Model(n);
	}
	addProperty("Model",model,slt);
}

void FLWio::setSignal(int nSignal)
{
	QStringList slt;
	slt << "Continues" << "Temporal"<<  "March Time"<< "California" ;
	addProperty("Signaling", Signal(nSignal),slt);
}

void FLWio::setSignal(QString signal)
{
	QStringList slt;
	slt << "Continues" << "Temporal"<<  "March Time"<< "California" ;
	addProperty("Signaling", signal,slt);
}

QString FLWio::Model()
{
	return getProperty("Model").value.toString();
}

QString FLWio::Model(int nModel)
{
	DeviceModels *g = FLGlobal::getDeviceModels();
	g->dev_size();
	QString str ;
	DeviceModel m = g->dev(nModel);
	str = QString::fromStdString( m.name());
	return str;
}

QString FLWio::Signal()
{	
	return getProperty("Signaling").value.toString();
}

QString FLWio::Signal(int nSignal)
{
	QString strSignal = "Undefined";
	switch(nSignal)
	{
	case 0:
		break;
	case 1:
		strSignal = "Continues";
		break;
	case 2:
		strSignal = "Temporal";
		break;
	case 3:
		strSignal = "March Time";
		break;
	case 4:
		strSignal = "California";
		break;
	default:
		break;
	}
	return strSignal;
}

void FLWio::Init()
{
	addProperty("Location","Location");
	addProperty("MAC Address","MAC",QStringList() << "FF"<< "FF"<< "FF" << "FF" << "FF" << "FF" << "FF" << "FF");
	setModel(0);
	setSignal(0);
	//addProperty("Name","WIO");	
	addProperty("Type","WIO",false);
	InitGuid();
	addProperty("Scale",QPointF(1.0,1.0),false);
	addProperty("WIOType", "Input", QStringList()<<"Input" <<"Output",true);
}
