#ifndef FLJOBPROTOL_H
#define FLJOBPROTOL_H

#include <QObject>

#include "flfloorview.h"
#include "fljob.h"
#include "flfloor.h"
#include "flzonecontrollor.h"
#include "flpropertytable.h"
#include "FLData.pb.h"
#include "zc.pb.h"

class FLJobProtol : public QObject
{
	Q_OBJECT

public:
	FLJobProtol(QObject *parent);
	~FLJobProtol();

	static void Object2Proto(Pro_Job* pro_job, FLJob* job);
	static void Object2Proto(Pro_Floor* pro_floor, FLFloor* floor);
	static void Object2Proto(Pro_ZC* pro_zc, FLZoneControllor* zc);
	static void Object2Proto(Pro_WIO* pro_wio, FLWio* wio);

	static void Proto2Object(const Pro_Job* pro_job, FLJob* job);
	static void Proto2Object(const Pro_Floor* pro_floor, FLFloor* floor);
	static void Proto2Object(const Pro_ZC* pro_zc, FLZoneControllor* zc);
	static void Proto2Object(const Pro_WIO* pro_wio, FLWio* wio);

	static FLJob* OpenJob(QString filename);
	static void SaveJob(FLJob* job);

	static FLZoneControllor* ZCFromFile(FLZoneControllor*pzc, QString filename);
	static bool ZC2File(FLZoneControllor* zc,QString filename);

	static FLWio* WIOFromFile(QString filename);
	static bool WIO2File(FLWio* wio, QString filename);
	static CFG_RTC RTCFromFile(QString filename);

private:
	
};

#endif // FLJOBPROTOL_H
