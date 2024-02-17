#include "flfloor.h"
#include "fljob.h"
#include <QPointF>
FLFloor::FLFloor(QObject *parent)
	: FLObject(parent)
{
	addProperty("Name", "Floor");
	addProperty("Type", "FLOOR");
	addProperty("FloorGraph", "graphics/sample_floor_plan.svg");
	addProperty("Comments", "none");
	addProperty("unit-of-measure", "Meters", QStringList() << "Meters" << "Yards" << "Feet" << "Inches" << "Centimeters");
	QStringList str_plotting_scale;
	str_plotting_scale << "1:1" << "1:2" << "1:5" << "1:10" << "1:20" << "1:50" << "1:100" << "1:200" << "1:500" << "1:1000" << "1:2000" << "1:5000";
	addProperty("Plotting_Scale","1:1", str_plotting_scale, true);
}

FLFloor::~FLFloor()
{

}

void FLFloor::addZC(QPointer<FLZoneControllor> pZC)
{
	m_ltZC.append(pZC);
	emit childrenChanged(pZC,1);
}

QPointer<FLZoneControllor> FLFloor::addZC(QString str)
{
	QPointer<FLZoneControllor> pZC = new FLZoneControllor(this);
	pZC->addProperty("Name",str);
	pZC->InitDefaultIO();
	addZC(pZC);
	return pZC;
}

void FLFloor::removeZC(QPointer<FLZoneControllor> pZC)
{
	
	m_ltZC.removeOne(pZC);
	emit childrenChanged(pZC,0);
	
}

QPointer<FLZoneControllor> FLFloor::getZC(int nZC)
{
	return m_ltZC[nZC];
}

int FLFloor::countZC()
{
	return m_ltZC.count();
}

void FLFloor::Init()
{
	addProperty("unit-of-measure","Meters");
	addProperty("Name","Floor");
	
	addProperty("Type","FLOOR",false);
	addProperty("FloorGraph","graphics/sample_floor_plan.svg");
	addProperty("Comments","New floor");
	addProperty("Scale",QPointF(1.0,1.0),false);
	QStringList str_plotting_scale;
	str_plotting_scale<<"1:1"<<"1:2"<<"1:5"<<"1:10" << "1:20" << "1:50" << "1:100" << "1:200" << "1:500" << "1:1000" << "1:2000" << "1:5000";
	addProperty("Plotting_Scale","1:100",str_plotting_scale, true);
	InitGuid();
}
