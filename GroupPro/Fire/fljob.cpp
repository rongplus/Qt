#include "fljob.h"

FLJob::FLJob(QObject *parent)
	: FLObject(parent)
{
	
}

FLJob::~FLJob()
{

}

void FLJob::addFloor(QPointer<FLFloor> pFloor)
{
	m_ltFloor.append(pFloor);
}

void FLJob::removeFloor(QPointer<FLFloor> pFloor)
{
	m_ltFloor.removeOne(pFloor);
}

QPointer<FLFloor> FLJob::getFloor(int nFloor)
{
	return m_ltFloor[nFloor];
}

int FLJob::countFloor()
{
	return m_ltFloor.count();
}

void FLJob::Init()
{
	addProperty("Standard","ULC (Canada)");
	//addProperty("Name","Job");
	InitGuid();
	addProperty("Type","JOB",false);
}

void FLJob::set_daylighttime(DayLight start, DayLight end)
{
	startDayLight = start;
	endDayLight = end;
}
void FLJob::set_daily_adjust(int sec)
{
	_DailyAdjust = sec;
}