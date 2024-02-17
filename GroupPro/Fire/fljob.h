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


#ifndef FLJOB_H
#define FLJOB_H

#include <QObject>
#include "flfloor.h"
#include "flobject.h"



class FLJob : public FLObject
{
	Q_OBJECT

public:
	FLJob(QObject *parent);
	~FLJob();

	void addFloor(QPointer<FLFloor> pFloor);
	void removeFloor(QPointer<FLFloor> pFloor);
	QPointer<FLFloor> getFloor(int nFloor);
	int countFloor();
	virtual void Init();

	struct DayLight
	{
		int month;
		int week; // week of the month
		int dayOfWeek; // what day of the week
		int hour;
		int min;
		bool operator ==(DayLight& other) { 
			if( month != other.month || week != other.week || dayOfWeek != other.dayOfWeek || hour != other.hour || min != other.min)
				return false; 
			return true;
		}
	};

	void set_daylighttime(DayLight start, DayLight end);
	void set_daily_adjust(int sec);

	DayLight start_daylight() { return startDayLight; }
	DayLight end_daylight() { return endDayLight; }
	int DailyAdjust() { return _DailyAdjust; }
	

private:
	QList< QPointer<FLFloor> > m_ltFloor;
	
	DayLight startDayLight;
	DayLight endDayLight;

	int _DailyAdjust; // Indicates how many seconds we compensate for clock 		
};

#endif // FLJOB_H
