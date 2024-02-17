#include "fljobprotol.h"

#include <QSettings>
#include <QMessageBox>
#include <iostream>
#include <fstream>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include "flglobal.h"
#include "zc.pb.h"
using namespace std;

FLJobProtol::FLJobProtol(QObject *parent)
	: QObject(parent)
{

}

FLJobProtol::~FLJobProtol()
{

}

void FLJobProtol::Object2Proto(Pro_Job* pro_job, FLJob* job)
{
	pro_job->set_comment( job->getProperty("Comments").value.toString().toStdString() );
	pro_job->set_guid( job->getProperty("GUID").value.toString().toStdString() );
	pro_job->set_jobpath( job->getProperty("JobPath").value.toString().toStdString() );
	pro_job->set_name( job->getProperty("Name").value.toString().toStdString() );
	pro_job->set_standard( job->getProperty("Standard").value.toString().toStdString() );
	pro_job->set_type("JOB");

	auto dl = pro_job->add_daylight();
	FLJob::DayLight start = job->start_daylight();
	dl->set_min(start.min);
	dl->set_hour(start.hour);
	dl->set_week(start.week);
	dl->set_month(start.month);
	dl->set_dayofweek(start.dayOfWeek);

	dl = pro_job->add_daylight();
	start = job->end_daylight();
	dl->set_min(start.min);
	dl->set_hour(start.hour);
	dl->set_week(start.week);
	dl->set_month(start.month);
	dl->set_dayofweek(start.dayOfWeek);

	pro_job->set_dailyadjust(job->DailyAdjust());

	int num_floor = job->countFloor();
	for(int n=0; n< num_floor; n++)
	{
		Pro_Floor* pro_floor = pro_job->add_floor();
		FLFloor* floor = job->getFloor(n);
		Object2Proto(pro_floor,floor);
	}
}

void FLJobProtol::Object2Proto(Pro_Floor* pro_floor, FLFloor* floor)
{
	pro_floor->set_comment( floor->getProperty("Comments").value.toString().toStdString() );
	pro_floor->set_guid( floor->getProperty("GUID").value.toString().toStdString() );
	pro_floor->set_floorgraph( floor->getProperty("FloorGraph").value.toString().toStdString() );
	pro_floor->set_name( floor->getProperty("Name").value.toString().toStdString() );
	pro_floor->set_unit_of_measure( floor->getProperty("unit_of_measure").value.toString().toStdString());
	pro_floor->set_type("FLOOR");
	pro_floor->set_plotting_scale(floor->getProperty("Plotting_Scale").value.toString().toStdString());

	QPointF zc_pos = floor->getProperty("Position").value.toPointF();
	Pro_Point *pro_floor_pt = new Pro_Point();
	pro_floor_pt->set_x(zc_pos.x());
	pro_floor_pt->set_y(zc_pos.y());


	pro_floor->set_allocated_position(pro_floor_pt);

	QPointF floor_scale = floor->getProperty("Scale").value.toPointF();
	if(floor_scale.x() <=0.01 || floor_scale.y()<=0.01)
	{
		floor_scale = QPointF(1.0,1.0);
	}
	Pro_Point* pro_floor_scale = new Pro_Point();
	pro_floor_scale->set_x(floor_scale.x());
	pro_floor_scale->set_y(floor_scale.y());

	pro_floor->set_allocated_scale(pro_floor_scale);

	int num_zc = floor->countZC();
	for(int n=0; n< num_zc; n++)
	{
		FLZoneControllor* zc = floor->getZC(n);
		Pro_ZC* pro_zc = pro_floor->add_zc();
		Object2Proto(pro_zc,zc);
	}
}

void FLJobProtol::Object2Proto(Pro_ZC* pro_zc, FLZoneControllor* zc)
{
	qDebug() << "Channel" << zc->getProperty("Channel").value.toString();
	pro_zc->set_channel( zc->getProperty("Channel").value.toString().toStdString() );
	pro_zc->set_guid( zc->getProperty("GUID").value.toString().toStdString() );
	pro_zc->set_location( zc->getProperty("Location").value.toString().toStdString() );
	pro_zc->set_name( zc->getProperty("Name").value.toString().toStdString() );
	pro_zc->set_range(zc->getProperty("Range").value.toString().toInt());
	//passcode 
	pro_zc->add_passcode(zc->getProperty("PassCode_Low").value.toString().toStdString() );
	pro_zc->add_passcode(zc->getProperty("PassCode_Mid").value.toString().toStdString());
	pro_zc->add_passcode(zc->getProperty("PassCode_Hei").value.toString().toStdString());

	//timer 1
	auto t1 = pro_zc->add_timer();
	t1->set_id(TIMER_SIG_SIL);
	t1->set_duration(zc->get_interval(TIMER_SIG_SIL));

	t1 = pro_zc->add_timer();
	t1->set_id(TIMER_ACFAIL_XMIT_DELAY);
	t1->set_duration(zc->get_interval(TIMER_ACFAIL_XMIT_DELAY));

	//LWORD SCU_FEATURE_FLAGS
	auto scu_flag = zc->getProperty("SCU_FEATURE_FLAGS").value.toString();
	if(scu_flag == "CFG_MAN_SIGSIL")
		pro_zc->set_scu_feature_flags(CFG_MAN_SIGSIL);
	else if(scu_flag == "CFG_AGENCY_ULC")
		pro_zc->set_scu_feature_flags(CFG_AGENCY_ULC);
	else
		pro_zc->set_scu_feature_flags(CFG_MAN_SIGSIL| CFG_AGENCY_ULC);

	QStringList vList = zc->getProperty("MAC Address").items;
	QString mac ;
	if(vList.size()==8)
	{
		mac = QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(vList[0]).arg(vList[1]).arg(vList[2]).arg(vList[3]).arg(vList[4]).arg(vList[5]).arg(vList[6]).arg(vList[7]);
	}
	else
		mac = "FF:FF:FF:FF:FF:FF:FF:FF";

	pro_zc->set_macaddr( mac.toStdString()  );
	pro_zc->set_type("ZC");
	pro_zc->set_panid( zc->getProperty("PAN ID").value.toString().toStdString() );

	QPointF zc_pos = zc->getProperty("Position").value.toPointF();
	Pro_Point* pro_zc_pt = new Pro_Point;


	pro_zc_pt->set_x(zc_pos.x());
	pro_zc_pt->set_y(zc_pos.y());
	pro_zc->set_allocated_position(pro_zc_pt);

	QPointF zc_scale = zc->getProperty("Scale").value.toPointF();
	if(zc_scale.x() <=0.01 || zc_scale.y()<=0.01)
		zc_scale = QPointF(1.0,1.0);
	Pro_Point *pro_zc_scale =new Pro_Point();
	pro_zc_scale->set_x(zc_scale.x());
	pro_zc_scale->set_y(zc_scale.y());

	pro_zc->set_allocated_scale(pro_zc_scale);

	int num_wio = zc->countWio();
	for(int n=0; n< num_wio; n++)
	{
		FLWio* wio = zc->getWio(n);
		Pro_WIO* pro_wio = pro_zc->add_wio();
		Object2Proto(pro_wio,wio);
	}

	int num_io = zc->countIO();
	for(int n=0; n< num_io; n++)
	{
		FLObject* io = zc->getIO(n);
		QString type = io->getProperty("Type").value.toString();		
		Pro_ZCIO* pro_zcio = nullptr;
		if (type == "ZC_INPUTS")
		{
			pro_zcio = pro_zc->add_input();
			pro_zcio->set_tag( io->getProperty("Tag").value.toString().toStdString() );
		}
		else if(type == "ZC_RELAY")
			pro_zcio = pro_zc->add_relay();
		else if(type == "ZC_LOCAL_TROUBLES")
			pro_zcio = pro_zc->add_local_troubles();
		else if(type == "ZC_REMOTE_TROUBLE")
			pro_zcio = pro_zc->add_remote_trouble();
		else if(type == "ZC_STATS")
			pro_zcio = pro_zc->add_stats();
		if(pro_zcio!=nullptr)
		{
			pro_zcio->set_name(io->getProperty("Name").value.toString().toStdString());
			pro_zcio->set_type( type.toStdString());
			//if(io->hasKey("Value"))
			pro_zcio->set_value(io->getProperty("Value").value.toString().toStdString());
			//if(io->hasKey("Output"))
			pro_zcio->set_corrs(io->getProperty("Corrs").value.toString().toStdString());
			if (io->hasKey("CktProcessType"))
			{
				pro_zcio->set_cktprocesstype(io->getProperty("CktProcessType").value.toString().toStdString());
			}
		}
	}

	pro_zc->set_comment(zc->getProperty("Comments").value.toString().toStdString());
}

void FLJobProtol::Object2Proto(Pro_WIO* pro_wio, FLWio* wio)
{
	pro_wio->set_channel( wio->getProperty("Channel").value.toString().toStdString() );
	pro_wio->set_guid( wio->getProperty("GUID").value.toString().toStdString() );
	pro_wio->set_location( wio->getProperty("Location").value.toString().toStdString() );
	pro_wio->set_name( wio->getProperty("Name").value.toString().toStdString() );
	QStringList vList = wio->getProperty("MAC Address").items;
	QString mac ;
	if(vList.size()==8)
	{
		mac = QString("%1:%2:%3:%4:%5:%6:%7:%8").arg(vList[0]).arg(vList[1]).arg(vList[2]).arg(vList[3]).arg(vList[4]).arg(vList[5]).arg(vList[6]).arg(vList[7]);
	}
	else
		mac = "FF:FF:FF:FF:FF:FF:FF:FF";
	pro_wio->set_macaddr( mac.toStdString() );
	pro_wio->set_type("WIO");
	pro_wio->set_model( wio->getProperty("Model").value.toString().toStdString());
	pro_wio->set_signaling( wio->getProperty("Signaling").value.toString().toStdString());
	QPointF wio_pos = wio->getProperty("Position").value.toPointF();
	Pro_Point* pro_wio_pt = new Pro_Point();
	pro_wio_pt->set_x(wio_pos.x());
	pro_wio_pt->set_y(wio_pos.y());
	pro_wio->set_allocated_position(pro_wio_pt);
	
	auto wio_type = wio->getProperty("WIOType").value.toString();
	pro_wio->set_wiotype(wio_type.toStdString());

	QPointF wio_scale = wio->getProperty("Scale").value.toPointF();
	if(wio_scale.x() <=0.01 || wio_scale.y()<=0.01)
		wio_scale = QPointF(1.0,1.0);

	Pro_Point *pro_wio_scale = new Pro_Point();
	pro_wio_scale->set_x(wio_scale.x());
	pro_wio_scale->set_y(wio_scale.y());
	pro_wio->set_allocated_scale(pro_wio_scale);

	pro_wio->set_value(wio->getProperty("Value").value.toString().toStdString());
	//if(io->hasKey("Output"))
	pro_wio->set_corrs(wio->getProperty("Corrs").value.toString().toStdString());

	pro_wio->set_comment(wio->getProperty("Comments").value.toString().toStdString());
}

void FLJobProtol::Proto2Object( const  Pro_Job* pro_job, FLJob* job)
{
	// 	pro_job->set_comment( job->getProperty("Comments").value.toString().toStdString() );
	job->addProperty("Comments",QString::fromStdString( pro_job->comment()));
	// 	pro_job->set_guid( job->getProperty("GUID").value.toString().toStdString() );
	job->addProperty("GUID",QString::fromStdString( pro_job->guid()),false);
	// 	pro_job->set_jobpath( job->getProperty("JobPath").value.toString().toStdString() );
	job->addProperty("JobPath",QString::fromStdString( pro_job->jobpath()),false);
	// 	pro_job->set_name( job->getProperty("Name").value.toString().toStdString() );
	job->addProperty("Name",QString::fromStdString( pro_job->name()));
	// 	pro_job->set_standard( job->getProperty("Standard").value.toString().toStdString() );
	job->addProperty("Standard",QString::fromStdString( pro_job->standard()),QStringList() << "UL(United States)" <<"ULC(Canada)");
	// 	pro_job->set_type("JOB");
	job->addProperty("Type",QString::fromStdString( pro_job->type()),false);

	int nDayLight = pro_job->daylight_size();
	//for (int n = 0; n < nDayLight; n++)
	if(nDayLight==2){
		auto ds = pro_job->daylight(0);
		FLJob::DayLight d1;
		d1.min = ds.min();
		d1.dayOfWeek = ds.dayofweek();
		d1.hour = ds.hour();
		d1.month = ds.month();
		d1.week = ds.week();

		auto de = pro_job->daylight(1);
		FLJob::DayLight d2;
		d2.min = de.min();
		d2.dayOfWeek = de.dayofweek();
		d2.hour = de.hour();
		d2.month = de.month();
		d2.week = de.week();

		job->set_daylighttime(d1, d2);

	}

	job->set_daily_adjust( pro_job->dailyadjust());
	int num_floor = pro_job->floor_size();
	for(int n=0; n< num_floor; n++)
	{
		FLFloor*  floor= new FLFloor(job);
		const Pro_Floor* pro_floor = &pro_job->floor(n);
		Proto2Object(pro_floor,floor);
		job->addFloor(floor);
	}
}

void FLJobProtol::Proto2Object(const  Pro_Floor* pro_floor, FLFloor* floor)
{
	// 	pro_floor->set_comment( floor->getProperty("Comments").value.toString().toStdString() );
	floor->addProperty("Comments",QString::fromStdString( pro_floor->comment()));
	// 	pro_floor->set_guid( floor->getProperty("GUID").value.toString().toStdString() );
	floor->addProperty("GUID",QString::fromStdString( pro_floor->guid()),false);
	// 	pro_floor->set_floorgraph( floor->getProperty("FllorGraph").value.toString().toStdString() );
	floor->addProperty("FloorGraph",QString::fromStdString( pro_floor->floorgraph()),false);
	// 	pro_floor->set_name( floor->getProperty("Name").value.toString().toStdString() );
	floor->addProperty("Name",QString::fromStdString( pro_floor->name()));
	// 	pro_floor->set_unit_of_measure( floor->getProperty("unit_of_measure").value.toInt() );
	floor->addProperty("unit-of-measure", QString::fromStdString(pro_floor->unit_of_measure()) ,QStringList() <<"Meters" << "Yards"<<"Feet"<<"Inches"<<"Centimeters");
	// 	pro_floor->set_type("FLOOR");
	floor->addProperty("Type",QString::fromStdString( pro_floor->type()),false);
	QStringList str_plotting_scale;
	str_plotting_scale<<"1:1"<<"1:2"<<"1:5"<<"1:10" << "1:20" << "1:50" << "1:100" << "1:200" << "1:500" << "1:1000" << "1:2000" << "1:5000";
	floor->addProperty("Plotting_Scale",QString::fromStdString(pro_floor->plotting_scale()),str_plotting_scale, true);

	Pro_Point pro_pos = pro_floor->position();
	QPointF pos;
	floor->addProperty("Position",QPointF(pro_pos.x(),pro_pos.y()),false);

	Pro_Point pro_scale = pro_floor->scale();
	floor->addProperty("Scale",QPointF(pro_scale.x(),pro_scale.y()),false);

	int num_zc = pro_floor->zc_size();
	for(int n=0; n< num_zc; n++)
	{
		FLZoneControllor* zc = new FLZoneControllor(floor);
		const Pro_ZC* pro_zc = &pro_floor->zc(n);
		Proto2Object(pro_zc,zc);
		floor->addZC(zc);
	}
}

void FLJobProtol::Proto2Object(const  Pro_ZC* pro_zc, FLZoneControllor* zc)
{
	// 	pro_zc->set_channel( zc->getProperty("Channel").value.toInt() );
	QStringList ch_list;
	for (int i = 11; i<27; i++)
		ch_list.append("Channel " + QString::number(i));

	zc->addProperty("Channel", QString::fromStdString(pro_zc->channel()) , ch_list);
	// 	pro_zc->set_guid( zc->getProperty("GUID").value.toString().toStdString() );
	zc->addProperty("GUID",QString::fromStdString( pro_zc->guid()), false);
	// 	pro_zc->set_location( zc->getProperty("Location").value.toString().toStdString() );
	zc->addProperty("Location",QString::fromStdString( pro_zc->location()));
	// 	pro_zc->set_name( zc->getProperty("Name").value.toString().toStdString() );
	zc->addProperty("Name",QString::fromStdString( pro_zc->name()));
	// 	pro_zc->set_macaddr( zc->getProperty("MAC Address").value.toString().toStdString() );
	zc->addProperty("MAC Address","MAC",QString::fromStdString( pro_zc->macaddr()).split(":"));
	// 	pro_zc->set_type("ZC");
	zc->addProperty("Type",QString::fromStdString( pro_zc->type()), false);
	// 	pro_zc->set_panid( zc->getProperty("PAN ID").value.toString().toStdString() );
	QStringList ll = PanID_List;
	zc->addProperty("PAN ID",QString::fromStdString( pro_zc->panid()), ll);
	zc->addProperty("Range",QString::number( pro_zc->range()));

	zc->addProperty("Comments", QString::fromStdString(pro_zc->comment()));
	//passcode 	
	zc->addProperty("PassCode_Low", QString::fromStdString(pro_zc->passcode(0)));
	zc->addProperty("PassCode_Mid", QString::fromStdString(pro_zc->passcode(1)));
	zc->addProperty("PassCode_Hei", QString::fromStdString(pro_zc->passcode(2)));


	//timer 1
	auto tm_count = pro_zc->timer_size();
	for (size_t i = 0; i < tm_count; i++)
	{
		PRO_TIMER tm = pro_zc->timer(i);
		auto id = tm.id();
		auto interval = tm.duration();	
		zc->add_timer(id, interval);
	}
	//LWORD SCU_FEATURE_FLAGS
	auto scu_flag = pro_zc->scu_feature_flags();
	if (scu_flag == CFG_MAN_SIGSIL)
		zc->addProperty("SCU_FEATURE_FLAGS", "CFG_MAN_SIGSIL", QStringList() << "CFG_MAN_SIGSIL" << "CFG_AGENCY_ULC" << "CFG_MAN_SIGSIL/CFG_AGENCY_ULC  ", true);
	else if (scu_flag == CFG_AGENCY_ULC)
		zc->addProperty("SCU_FEATURE_FLAGS", "CFG_AGENCY_ULC", QStringList() << "CFG_MAN_SIGSIL" << "CFG_AGENCY_ULC" << "CFG_MAN_SIGSIL/CFG_AGENCY_ULC  ", true);
	else
		zc->addProperty("SCU_FEATURE_FLAGS", "CFG_MAN_SIGSIL/CFG_AGENCY_ULC", QStringList() << "CFG_MAN_SIGSIL" << "CFG_AGENCY_ULC" << "CFG_MAN_SIGSIL/CFG_AGENCY_ULC  ", true);


	Pro_Point pro_pos = pro_zc->position();
	QPointF pos;
	zc->addProperty("Position",QPointF(pro_pos.x(),pro_pos.y()),false);

	Pro_Point pro_scale = pro_zc->scale();
	zc->addProperty("Scale",QPointF(pro_scale.x(),pro_scale.y()),false);

	int num_wio = pro_zc->wio_size();
	for(int n=0; n< num_wio; n++)
	{
		FLWio* wio = new FLWio(zc);
		const Pro_WIO* pro_wio = &pro_zc->wio(n);
		Proto2Object(pro_wio,wio);
		zc->addWio(wio);
	}

	int num_io = pro_zc->input_size();
	for(int n=0; n< num_io; n++)
	{
		FLObject* io = new FLObject(zc);
		const Pro_ZCIO* pro_zcio = &pro_zc->input(n);		

		io->addProperty("Name",QString::fromStdString(pro_zcio->name()),false);
		io->addProperty("Type",QString::fromStdString(pro_zcio->type()),false);	
		io->addProperty("Corrs", QString::fromStdString(pro_zcio->corrs()),false);
		io->addProperty("Value", QString::fromStdString(pro_zcio->value()), false);
		if (io->getProperty("Type").value.toString() == "ZC_INPUTS")
		{
			io->addProperty("Tag", QString::fromStdString(pro_zcio->tag()), true);
		}
		if (pro_zcio->cktprocesstype().size() >0 )
		{
			io->addProperty("CktProcessType", QString::fromStdString(pro_zcio->cktprocesstype()), QStringList() << "ALARM" << "SUPVLATCH" << "SUPVNONLATCH" << "TROUBLE");
		}

		zc->addIO(io);
	}

	num_io = pro_zc->relay_size ();
	for(int n=0; n< num_io; n++)
	{
		FLObject* io = new FLObject(zc);
		const Pro_ZCIO* pro_zcio = &pro_zc->relay(n);
		io->addProperty("Name",QString::fromStdString(pro_zcio->name()),false);
		io->addProperty("Type",QString::fromStdString(pro_zcio->type()), false);
		
		io->addProperty("Corrs", QString::fromStdString(pro_zcio->corrs()), false);
		io->addProperty("Value", QString::fromStdString(pro_zcio->value()),false);
		zc->addIO(io);
	}

	num_io = pro_zc->local_troubles_size ();
	for(int n=0; n< num_io; n++)
	{
		FLObject* io = new FLObject(zc);
		const Pro_ZCIO* pro_zcio = &pro_zc->local_troubles(n);
		io->addProperty("Name",QString::fromStdString(pro_zcio->name()),false);
		io->addProperty("Type",QString::fromStdString(pro_zcio->type()), false);
		io->addProperty("Corrs", QString::fromStdString(pro_zcio->corrs()), false);
		io->addProperty("Value", QString::fromStdString(pro_zcio->value()), false);
		zc->addIO(io);
	}

	num_io = pro_zc->remote_trouble_size ();
	for(int n=0; n< num_io; n++)
	{
		FLObject* io = new FLObject(zc);
		const Pro_ZCIO* pro_zcio = &pro_zc->remote_trouble(n);
		io->addProperty("Name",QString::fromStdString(pro_zcio->name()),false);
		io->addProperty("Type",QString::fromStdString(pro_zcio->type()), false);
		io->addProperty("Corrs", QString::fromStdString(pro_zcio->corrs()), false);
		io->addProperty("Value", QString::fromStdString(pro_zcio->value()), false);
		zc->addIO(io);
	}

	num_io = pro_zc->stats_size ();
	for(int n=0; n< num_io; n++)
	{
		FLObject* io = new FLObject(zc);
		const Pro_ZCIO* pro_zcio = &pro_zc->stats(n);	
		io->addProperty("Name",QString::fromStdString(pro_zcio->name()),false);
		io->addProperty("Type",QString::fromStdString(pro_zcio->type()),false);	
		io->addProperty("Corrs", QString::fromStdString(pro_zcio->corrs()), false);
		io->addProperty("Value", QString::fromStdString(pro_zcio->value()), false);
		zc->addIO(io);
	}
	zc->reset_input();
	zc->reset_output();
}

void FLJobProtol::Proto2Object(const Pro_WIO* pro_wio, FLWio* wio)
{
	// 	pro_wio->set_channel( wio->getProperty("Channel").value.toString().toStdString() );
	QStringList ch_list;
	for (int i = 11; i<27; i++)
		ch_list.append("Channel " + QString::number(i));
	wio->addProperty("Channel", QString::fromStdString( pro_wio->channel()), ch_list);
	// 	pro_wio->set_guid( wio->getProperty("GUID").value.toString().toStdString() );
	wio->addProperty("GUID", QString::fromStdString( pro_wio->guid()),false);
	// 	pro_wio->set_location( wio->getProperty("Location").value.toString().toStdString() );
	wio->addProperty("Location", QString::fromStdString( pro_wio->location()));
	// 	pro_wio->set_name( wio->getProperty("Name").value.toString().toStdString() );
	wio->addProperty("Name", QString::fromStdString( pro_wio->name()));
	// 	pro_wio->set_macaddr( wio->getProperty("MAC Address").value.toString().toStdString() );
	wio->addProperty("MAC Address","MAC", QString::fromStdString( pro_wio->macaddr()).split(":"));
	// 	pro_wio->set_type("WIO");
	wio->addProperty("Type", QString::fromStdString( pro_wio->type()),false);
	// 	pro_wio->set_model( wio->getProperty("Model").value.toString().toStdString());
	wio->setModel( QString::fromStdString( pro_wio->model()));
	// 	pro_wio->set_signaling( wio->getProperty("Signaling").value.toString().toStdString());
	wio->setSignal(QString::fromStdString( pro_wio->signaling()));
	wio->addProperty("Comments", QString::fromStdString(pro_wio->comment()));
	Pro_Point pro_pos = pro_wio->position();
	QPointF pos;
	wio->addProperty("Position",QPointF(pro_pos.x(),pro_pos.y()),false);

	Pro_Point pro_scale = pro_wio->scale();
	wio->addProperty("Scale",QPointF(pro_scale.x(),pro_scale.y()),false);
	wio->addProperty("WIOType", QString::fromStdString(pro_wio->wiotype()),QStringList()<<"Input" << "Output", false);
	wio->addProperty("Corrs", QString::fromStdString(pro_wio->corrs()), false);
	wio->addProperty("Value", QString::fromStdString(pro_wio->value()),false);

}

FLJob * FLJobProtol::OpenJob(QString filename)
{
	if (filename == "")
		return nullptr;
	QSettings st("Mircom", "FireLink");
	auto job_list = st.value("Recently_Job_Path").toStringList();
	QFile fi(filename);
	if (fi.exists() == false)
	{
		QMessageBox::warning(nullptr, "Error Job file", "Please choose right job file.");
		job_list.removeOne(filename);
		st.setValue("Recently_Job_Path", job_list);
		return nullptr;
	}

	Pro_Job *pro_job = new Pro_Job();
	fstream input(filename.toStdString(), ios::in | ios::binary);
	bool bOk = pro_job->ParseFromIstream(&input);
	if (bOk == false)
	{
		QMessageBox::warning(nullptr, "Error Job file", "Please choose right job file.");
		job_list.removeOne(filename);
		st.setValue("Recently_Job_Path", job_list);
		return nullptr;
	}
	FLJob* job = new FLJob(nullptr);
	Proto2Object(pro_job, job);
	job->addProperty("JobPath", filename);
	
	input.close();

	job_list.removeOne(filename);
	job_list.insert(0, filename);
	if (job_list.size() > 8)
		job_list.removeAt(8);
	st.setValue("Recently_Job_Path", job_list);
	return job;
}

void FLJobProtol::SaveJob(FLJob * job)
{
	Pro_Job *pro_job = new Pro_Job();

	FLJobProtol::Object2Proto(pro_job, job);

	QString str_file_name = job->getProperty("JobPath").value.toString();

	fstream output(str_file_name.toStdString(), ios::out | ios::trunc | ios::binary);
	pro_job->SerializeToOstream(&output);

	//google::protobuf::ShutdownProtobufLibrary();
	output.close();
	QMessageBox::information(0, "Save Job Successful", job->getProperty("Name").value.toString() + " Saved to " + str_file_name);

	QSettings st("Mircom", "FireLink");
	auto job_list = st.value("Recently_Job_Path").toStringList();
	job_list.removeOne(str_file_name);
	job_list.insert(0, str_file_name);
	if (job_list.size() > 8)
		job_list.removeAt(8);
	//job_list.reserve(8);
	st.setValue("Recently_Job_Path", job_list);
	delete pro_job;
}

FLZoneControllor* FLJobProtol::ZCFromFile(FLZoneControllor*pzc,QString filename)
{
	QFile fi(filename);
	if (fi.open(QIODevice::ReadWrite) == false)
		return nullptr;
	QDataStream in_s(&fi);

	int nSize = fi.size();
	char *buff = new char[nSize];

	in_s.readRawData(buff, nSize);
	QByteArray arr;
	nSize--;
	while (buff[nSize] == 0x1a)
		nSize--;
	for (size_t i = 0; i < nSize; i++)
	{
		arr.append(buff[i]);
	}

	//bool bOK = rtc.ParseFromArray(buff, rtc_len + 1);
	ZC *zc = new ZC();
	bool bOK = zc->ParseFromArray(buff, nSize + 1);
	if (bOK == false)
		return nullptr;
	//FLZoneControllor* pro_zc =  new FLZoneControllor(0);
	//pro_zc->InitDefaultIO();
	pzc->readFirmware(zc);
	delete zc;
	return pzc;
}
bool FLJobProtol::ZC2File(FLZoneControllor* zc, QString filename)
{
	return false;
}

FLWio* FLJobProtol::WIOFromFile(QString filename)
{
	return nullptr;
}
bool FLJobProtol::WIO2File(FLWio* wio, QString filename)
{
	return false;
}

CFG_RTC FLJobProtol::RTCFromFile(QString filename)
{
	//	
	CFG_RTC rtc;

	int n = sizeof(CFG_RTC);
	QFile fi(filename);
	if (fi.open(QIODevice::ReadWrite) == false)
		return rtc;
	QDataStream in_s(&fi);

	char *buff = new char[100];
	int rtc_len = 100;
	in_s.readRawData(buff, rtc_len);
	QByteArray arr;
	rtc_len--;
	while (buff[rtc_len] == 0x1a)
		rtc_len--;
	for (size_t i = 0; i < rtc_len; i++)
	{
		arr.append(buff[i]);
	}
	bool bOK = rtc.ParseFromArray(buff, rtc_len + 1);	
#if 0
	fstream input("rtc_save.bin", ios::in | ios::binary);
	bool bOk = rtc.ParseFromIstream(&input);
	if (bOk == false)
	{
		QMessageBox::warning(this, "Error rtc file", "Please choose right rtc file.");
		return;
	}

	input.close();
#endif
	return rtc;
}