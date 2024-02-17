#include "flzonecontrollor.h"
#include "fl_defs.pb.h"
#include <google/protobuf/descriptor.h>
#include <QPointF>
#include "flglobal.h"

#include "fljob.h"

#include <iostream>
#include <fstream>
#include <string.h>
#include<math.h>
#include<cmath>
#include<stdlib.h>
#include <bitset>
#include <iomanip>

#include <QFile>
using namespace std;
FLZoneControllor::FLZoneControllor(QObject *parent)
	: FLObject(parent)
{
	
}

FLZoneControllor::~FLZoneControllor()
{

}

void FLZoneControllor::addWio(QPointer<FLWio> pWio)
{
	
	auto wio_type = pWio->getProperty("WIOType").value.toString();
	QList<FLObject*> list;

	foreach (auto obj , m_ltWio)
	{
		auto type = obj->getProperty("WIOType").value.toString();
		if (type.compare(wio_type) == 0)
			list.append(obj);
	}
		
	//QString str = QString("0x%1").arg( (int)(list.count() + 1),2,10, QLatin1Char('0'));
	int value = list.count() % 8+1;
	
	pWio->addProperty("Value", format_value(value), false);
	m_ltWio.append(pWio);
	emit childrenChanged(pWio,1);
}

void FLZoneControllor::removeWio(QPointer<FLWio> pWio)
{	
	auto value = pWio->getProperty("Value").value.toString();
	m_ltWio.removeOne(pWio);	
	int input = 1;
	int output = 1;

	foreach (auto obj , m_ltWio)
	{
		auto corrs = obj->getProperty("Corrs").value.toString();
		//corrs.remove(value);
		obj->addProperty("Corrs", "0x00", false);
		auto wio_type = obj->getProperty("WIOType").value.toString();
		if (wio_type == "Input")
		{
			input++;
			obj->addProperty("Value", format_value(input), false);
		}	
		else
		{
			output++;
			obj->addProperty("Value", format_value(output), false);
		}	
	}

	return;


	//reset corrs
	auto wio_type = pWio->getProperty("WIOType").value.toString();
	QList<FLObject*> list;
	QList<FLObject*> reset_list;
	if (wio_type == "Input")
	{
		//reset output list
		list = outputs();
		reset_list = inputs();
	}
	else
	{
		//reset input list
		list = inputs();
		reset_list = outputs();
	}

	foreach (auto obj , list)
	{
		auto corrs = obj->getProperty("Corrs").value.toString();
		//corrs.remove(value);
		obj->addProperty("Corrs", "0x00", false);
	}

	
	//reset value;
	foreach (auto obj , m_ltWio)
	{
		auto wio_type = obj->getProperty("WIOType").value.toString();
		if(wio_type == "Input")
			obj->addProperty("Value", "0x" + QString::number(++input ), false);
		else
			obj->addProperty("Value", "0x" + QString::number(++output), false);
	}
	
	//m_pWio->addProperty("Value", "0x00");
	emit childrenChanged(pWio,0);
}

QPointer<FLWio> FLZoneControllor::getWio(int nWio)
{
	return m_ltWio[nWio];
}

int FLZoneControllor::countWio()
{
	return m_ltWio.count();
}

void FLZoneControllor::InitDefaultIO()
{	
	int input = 1;
	int output = 1;
	for (int i=0;i<4;++i)
	{
		FLObject* pIO = new FLObject(this);
		pIO->addProperty("Type","ZC_INPUTS",false);
		pIO->addProperty("Name","Local Input "+QString::number(i+1),false);
		pIO->addProperty("Value", format_value(input++), false);	
		pIO->addProperty("Corrs", "0x00", false);
		pIO->addProperty("CktProcessType", "ALARM", QStringList() << "ALARM" << "SUPVLATCH"<<"SUPVNONLATCH"<<"TROUBLE");
		pIO->addProperty("Tag", "", true);
		addIO(pIO);
	}

	for (int i=0;i<4;++i)
	{		
		FLObject* pIO = new FLObject(this);
		pIO->addProperty("Type","ZC_RELAY",false);
		pIO->addProperty("Name","Output Relay "+QString::number(i+1),false);
		pIO->addProperty("Value", format_value(output++), false);
		pIO->addProperty("Corrs", "0x00", false);
		addIO(pIO);		
	}
	input = 1;
	output = 1;
	for (int i=1;i<LocalTroubleType_descriptor()->value_count();++i)
	{
		FLObject* pIO = new FLObject(this);
		pIO->addProperty("Type","ZC_LOCAL_TROUBLES",false);
		pIO->addProperty("Name",QString::fromStdString(LocalTroubleType_descriptor()->value(i)->name()),false);
		pIO->addProperty("Value", format_value(input++), false);
		pIO->addProperty("Corrs", "0x00", false);
		addIO(pIO);		
		
	}

	input = 1;
	output = 1;
	for (int i=1;i<RemoteTroubleType_descriptor()->value_count();++i)
	{	
		FLObject* pIO = new FLObject(this);
		pIO->addProperty("Type","ZC_REMOTE_TROUBLE",false);
		pIO->addProperty("Name",QString::fromStdString(RemoteTroubleType_descriptor()->value(i)->name()));
		pIO->addProperty("Value", format_value(input++), false);
		pIO->addProperty("Corrs", "0x00", false);
		addIO(pIO);	
	}
	input = 1;
	output = 1;
	for (int i=0;i<SystemStatusType_descriptor()->value_count();++i)
	{
		FLObject* pIO = new FLObject(this);
		pIO->addProperty("Type","ZC_STATS",false);
		pIO->addProperty("Name",QString::fromStdString(SystemStatusType_descriptor()->value(i)->name()));
		pIO->addProperty("Value", format_value(input++), false);
		pIO->addProperty("Corrs", "0x00", false);
		addIO(pIO);	
	}	
}

void FLZoneControllor::addIO(FLObject* pIO)
{
	m_ltNode.append(pIO);
}

void FLZoneControllor::deleteIO(FLObject* pIO)
{

}

FLObject* FLZoneControllor::getIO(int nIndex)
{
	return m_ltNode[nIndex];
}

int FLZoneControllor::countIO()
{
	return m_ltNode.count();
}

void FLZoneControllor::Init()
{	
	addProperty("Location","Location");
	addProperty("MAC Address","MAC",QStringList() << "00"<< "00"<< "00"<< "00"<<"00"<<"00"<<"00"<<"00");
	addProperty("PAN ID","FFFF");
	QStringList ch_list;
	for (int n = 11; n < 27; n++)
		ch_list.append("Channel" + QString::number(n));
	addProperty("Channel","Channel 11", ch_list);
	//addProperty("Name","ZoneControllor");
	InitGuid();
	addProperty("Type","ZC",false);
	addProperty("Scale",QPointF(1.0,1.0),false);
	addProperty("Range",100,true);

	addProperty("PassCode_Low", "1111", true);
	addProperty("PassCode_Mid", "2222", true);
	addProperty("PassCode_Hei", "3333", true);

	auto li = [] (int stat, int end, int step,QString unit){ 
		QStringList lt; 
		for (int n = stat; n < end; n += step) 
			lt.append(QString::number(n) + unit);
		return lt; 
	};	
	QStringList tt = QStringList() << "-1";
	tt.append(li(1, 19, 1," Hours"));
	addProperty("TIMER_SIG_SIL", "-1",  tt);

	QStringList h_li = QStringList() << "30 Sec" << "1 Minutes" << "3 Minutes" << "5 Minutes";

	h_li.append(li(5, 31, 5," Minutes"));

	addProperty("TIMER_ACFAIL_XMIT_DELAY", "30 SEC", h_li);

	addProperty("SCU_FEATURE_FLAGS", "CFG_MAN_SIGSIL", QStringList() << "CFG_MAN_SIGSIL" << "CFG_AGENCY_ULC"<< "CFG_MAN_SIGSIL/CFG_AGENCY_ULC",true);

}

QList<FLObject*> FLZoneControllor::outputs()
{
	QList<FLObject*> out_list;
	foreach (auto obj , m_ltNode)
	{
		auto type = obj->getProperty("Type").value.toString();
		//out_value
		if (type.compare("ZC_RELAY") == 0)
		{
			out_list.append(obj);
		}
		else
		{

		}
	}
	foreach (auto obj , m_ltWio)
	{
		auto type = obj->getProperty("WIOType").value.toString();
		if (type.compare("Output") == 0)
		{
			out_list.append(obj);
		}
		else
		{
		}

	}
	return out_list;
}

QList<FLObject*> FLZoneControllor::inputs()
{
	QList<FLObject*> out_list;
	foreach (auto obj , m_ltNode)
	{
		auto type = obj->getProperty("Type").value.toString();
		//out_value
		if (type.compare("ZC_RELAY") != 0)
		{
			out_list.append(obj);
		}
		else
		{

		}
	}
	foreach (auto obj , m_ltWio)
	{
		auto type = obj->getProperty("WIOType").value.toString();
		if (type.compare("Output") != 0)
		{
			out_list.append(obj);
		}
		else
		{
		}

	}
	return out_list;
}

int FLZoneControllor::output_number(bool bDefault)
{
	//default = 3;
	if (bDefault)
		return 3;
	return outputs().count() ;
}

int FLZoneControllor::input_number(bool bDefault)
{
	//default = 48
	if (bDefault)
		return 48;
	return inputs().count() ;
}

void FLZoneControllor::reset_input()
{
	auto out_list = outputs();
	auto in_list = inputs();
	int nCount = in_list.count();
	for (int n = 0; n < nCount; n++)
	{
		QString corrs;
		foreach (auto obj , out_list)
		{
			auto value = obj->getProperty("Corrs").value.toString();
			auto list = value.split(" ", QString::SkipEmptyParts);
			if (n < list.count() && list[n] == "0x01")
				corrs += "0x01 ";
			else
				corrs += "0x00 ";
		}
		in_list[n]->addProperty ("Corrs", corrs, false);
	}
}

void FLZoneControllor::reset_output()
{
	auto out_list = outputs();
	auto in_list = inputs();
	int nCount = out_list.count();
	for (int n = 0; n < nCount; n++)
	{
		QString corrs;
		foreach (auto obj , in_list)
		{
			auto value = obj->getProperty("Corrs").value.toString();
			auto list = value.split(" ", QString::SkipEmptyParts);
			if (n < list.count() && list[n] == "0x01")
				corrs += "0x01 ";
			else
				corrs += "0x00 ";
		}
		out_list[n]->addProperty("Corrs", corrs, false);
	}
}

QString FLZoneControllor::get_default_corrs(QString corrstype, QList<QPointer<FLObject>> lt)
{
	QString corrs;
	foreach (auto obj , lt)
	{
		auto type = obj->getProperty("Type").value.toString();
		if (type != corrstype)
			continue;
		int enum_index = 0;
		auto name = obj->getProperty("Name").value.toString();
		if (corrstype == "ZC_LOCAL_TROUBLES")
		{
			auto vv = LocalTroubleType_descriptor()->FindValueByName(name.toStdString());
			enum_index = vv->number();
		}
		else if (corrstype == "ZC_REMOTE_TROUBLE")
		{
			auto vv = RemoteTroubleType_descriptor()->FindValueByName(name.toStdString());
			enum_index = vv->number();
		}
		


		auto value = obj->getProperty("Corrs").value.toString();//0x01 0x02 0x03
																//value = value.replace("0x", "");
		auto out_list = value.split(" ", QString::SkipEmptyParts);
		char out_value = 0;
		for (int n = 0; n< 4 && n <out_list.count(); n++)
		{
			auto var = out_list[n];
			if (var == "0x01")
				format_value(out_value, n);
		}
		//
		while (corrs.size() < enum_index )
		{
			corrs += char(0x00);//"0"
		}
		corrs += out_value;		
	}
	return corrs;
}

QString FLZoneControllor::get_default_corrs(QString corrstype, QList<FLObject*> lt)
{
	QString corrs;
	foreach (auto obj , lt)
	{
		auto type = obj->getProperty("Type").value.toString();
		if (type != corrstype)
			continue;
		//LocalTroubleType_descriptor()->value(i)		
		
		auto value = obj->getProperty("Corrs").value.toString();//0x01 0x02 0x03
																//value = value.replace("0x", "");
		auto out_list = value.split(" ", QString::SkipEmptyParts);
		char out_value = 0;
		for (int n = 0; n< 4 && n <out_list.count(); n++)
		{
			auto var = out_list[n];
			if (var == "0x01")
				format_value(out_value, n);
		}
		
		corrs += out_value;
	}
	return corrs;
}

QString FLZoneControllor::get_wio_corrs(QString corrstype, QList<QPointer<FLObject>> lt)
{
	QString corrs;
	foreach (auto obj , lt)
	{
		auto type = obj->getProperty("Type").value.toString();
		if (type != corrstype)
			continue;	
		auto value = obj->getProperty("Corrs").value.toString();//0x01 0x02 0x03
																//value = value.replace("0x", "");
		auto out_list = value.split(" ", QString::SkipEmptyParts);
		char out_value = 0;
		int start_index =0;
		for (int n = start_index; n< out_list.count(); n++)
		{
			auto var = out_list[n];
			format_value(out_value, var.toInt());
		}
		//out_value		
		corrs += out_value;		
	}
	return corrs;
}
QString FLZoneControllor::get_wio_corrs(QString type, QList<FLObject*> lt)
{
	return "";
}

void FLZoneControllor::writeFirmware(ZC *pZC)
{
	if (pZC == nullptr)
		return;
	QList<FLObject*> out_list;
	QList<FLObject*> in_list;
	foreach (auto obj , m_ltWio)
	{
		auto type = obj->getProperty("WIOType").value.toString();
		if (type == "Input")
			in_list.append(obj);
		else
			out_list.append(obj);
	}

	int out_wio_count = out_list.count();

	//format firmware data	

	//pZC->set_dataintcheck(0);	
}

void FLZoneControllor::readFirmware(ZC * pZC)
{


#pragma region Passcoed
	/*auto passcoe = pZC->add_passcodes();
	passcoe->set_data("1111");
	passcoe->set_len(4);*/
#pragma endregion
	

}

int FLZoneControllor::get_interval(int type)
{	

	QString str;
	if (type == 0)
	{
		str = getProperty("TIMER_SIG_SIL").value.toString();		
	}
	else
	{
		str = getProperty("TIMER_ACFAIL_XMIT_DELAY").value.toString();
	}
	auto v_lt = str.split(" ", QString::SkipEmptyParts);
	auto v = v_lt[0].toInt();
	if (v == -1)
		return v;
	if (v_lt[1] == "Sec")
		return v;
	else if (v_lt[1] == "Minutes")
		return v * 60;
	else if (v_lt[1] == "Hours")
		return v * 60 * 60;


	return v;
}

void FLZoneControllor::add_timer(int id, int duration)
{
	auto li = [](int stat, int end, int step, QString unit) {
		QStringList lt;
		for (int n = stat; n < end; n += step)
			lt.append(QString::number(n) + unit);
		return lt;
	};
	QString unit = " Sec";
	auto interval = duration ;
	if (duration / 3600 >0)
	{
		interval = duration / 3600;
		unit = " Hours";
	}
	else if( duration/60>0)
	{ 
		interval = duration / 3600;
		unit = " Minutes";	
	}

	if (id == TIMER_SIG_SIL)
	{
		QStringList tt = QStringList() << "-1";
		tt.append(li(1, 19, 1, " Hours"));
		addProperty("TIMER_SIG_SIL", QString::number(interval)+ unit, tt);
	}
	else
	{
		QStringList h_li = QStringList() << "30 Sec" << "1 Minutes" << "3 Minutes" << "5 Minutes";
		h_li.append(li(5, 31, 5, " Minutes"));
		addProperty("TIMER_ACFAIL_XMIT_DELAY", QString::number(interval) + unit, h_li);
	}
}

void FLZoneControllor::format_value(char & out_value, int in_value)
{
	int value = pow(2, (in_value )%8 );	
	out_value |= value;
	
}

QString FLZoneControllor::format_value(int value)
{
	return QString("0x%1").arg(value, 2, 10, QLatin1Char('0'));
}



void FLZoneControllor::setMacAddress(QString strMAc)
{
	QStringList slt = strMAc.split(":", QString::SkipEmptyParts);
	if (slt.count() != 8)
	{
		for (int n = 0; n < 8 - slt.count(); n++)
			slt << "FF";
	}
	addProperty("MAC Address", "MAC", slt);

}
QString FLZoneControllor::MacAddress()
{
	QStringList vList = getProperty("MAC Address").items;
	if (vList.size() < 8)
		return "FF:FF:FF:FF:FF:FF:FF:FF";
	auto mac = QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(vList[0]).arg(vList[1]).arg(vList[2]).arg(vList[3]).arg(vList[4]).arg(vList[5]).arg(vList[6]).arg(vList[7]);
	return mac;
}
