#include "flheatmap.h"
#include "fljob.h"
#include <QPointF>
FLHeatMap::FLHeatMap(QObject *parent)
	: FLObject(parent)
{
	
}

FLHeatMap::~FLHeatMap()
{

}

void FLHeatMap::addTx(QPoint pZC)
{
	m_ltTx.append(pZC);
	//emit childrenChanged(pZC,1);
}

QPoint FLHeatMap::addTx(QString str)
{
	QPoint pZC ;
	/*pZC->addProperty("Name",str);
	pZC->InitDefaultIO();
	addTx(pZC);*/
	return pZC;
}

void FLHeatMap::removeTx(QPoint pZC)
{
	
	m_ltTx.removeOne(pZC);
	//emit childrenChanged(pZC,0);
	
}

QPoint FLHeatMap::getTx(int nZC)
{
	return m_ltTx[nZC];
}

int FLHeatMap::countTx()
{
	return m_ltTx.count();
}

void FLHeatMap::Init()
{
	addProperty("unit-of-measure","Meters");
	addProperty("Name","HeatMap");
	
	addProperty("Type","HEATMAP",false);
	//addProperty("FloorGraph","graphics/sample_floor_plan.svg");
	addProperty("Comments","Heat Map");
	addProperty("Scale",QPointF(1.0,1.0),false);
	InitGuid();
}
