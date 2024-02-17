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
#ifndef FLGLOBAL_H
#define FLGLOBAL_H

#include <QLineEdit>

#include <QObject>
#include "fl_job.pb.h" 
using namespace std;

#define TIMER_SIG_SIL            0  // FCU AUTO SIGNAL SILENCE TIMER
#define TIMER_HEARTBEAT          1  // SVU HEARTBEAT TIMER
#define TIMER_CFG_ACTIVITY       2  // CFU CONFIGURATION ACTIVITY
#define TIMER_ACFAIL_XMIT_DELAY  3  // FCU DELAY FOR TRB TRANSMIT FOR AC FAIL
#define TIMER_DSU_MAN_DISP       4  // DSU MANUAL QUEUE DISPLAY
#define TIMER_CMD_ABORT          5  // CMD KEY PRESS TIMER
#define TIMER_CMD_MSGTIMER       6  // KEY PRESS ABORT TIMER
#define TIMER_BACKLIGHT_ON       7  // 30 SECONDS LCD BACKLIGHT
#define TIMER_SINCE_CKT_RESET    8  // 1 MINUTE TIMER
#define TIMER_INFO               9  // INFO KEY TIMER
#define TMU_UNSIL_BUZZER         10 // BUZZER UNSILENCE TIMER
#define TIMER_CMD_KEY_TIMEOUT    11
#define TIMER_PING_CYCLE         12 // WCU PING CYCLE
#define TIMER_INDIV_STAT_REP     13 // WCU INDIVIDUAL STATUS REPORT
#define TIMER_PING_STARTUP       14 // WCU START PING CYCLE // TEMP SOLUTION
#define TIMER_CMD_INFO           15 // UTLITY TIMER FOR COMMAND TASK
#define TIMER_LAMPTEST           16
#define TIMER_WIO_REFRESH        17 

#define CFG_MAN_SIGSIL  0x00000001ul // ENABLE MANUAL SIGNAL SILENCE
#define CFG_AGENCY_ULC  0x00000002ul // UL/ULC AGENCY



// OTA Cluster Command Frames
#define COMMAND_IMAGE_NOTIFY                          0x00
#define COMMAND_QUERY_NEXT_IMAGE_REQ                  0x01
#define COMMAND_QUERY_NEXT_IMAGE_RSP                  0x02
#define COMMAND_IMAGE_BLOCK_REQ                       0x03
#define COMMAND_IMAGE_PAGE_REQ                        0x04
#define COMMAND_IMAGE_BLOCK_RSP                       0x05
#define COMMAND_UPGRADE_END_REQ                       0x06
#define COMMAND_UPGRADE_END_RSP                       0x07
#define COMMAND_QUERY_SPECIFIC_FILE_REQ               0x08
#define COMMAND_QUERY_SPECIFIC_FILE_RSP               0x09

enum OTA_MSG_TYPES {
	OTA_IMAGE_NOTIFY,
	OTA_QUERY_RESPONSE,
	OTA_IMAGE_RESPONSE,
	OTA_UPGRADE_END,
};
enum IMAGE_TYPES {
	OTA_IT_MANSPC = 0x0000,
	OTA_IT_CONFIG = 0xffc1,
};

#pragma pack(push, 1)

typedef struct APSHeader_t {
	uint8_t frameControl;
	uint8_t dstEndPoint;
	uint16_t clusterID;
	uint16_t smartEP;
	uint8_t srcEndPoint;
	uint8_t apsCounter;
}APSHeader;


typedef struct ZCLHeader_t {
	uint8_t frameControl;
	uint8_t transSN;
	uint8_t cmdID;
}ZCLHeader;


typedef struct imgData_t {
	uint8_t Length;
	uint8_t Data[48];
}imgData;

typedef struct ZCLPayload_t {
	uint8_t status;
	uint16_t manfCode;
	uint16_t imgType;
	uint32_t fileVer;
	uint32_t fileOffset;
	imgData imd;
}ZCLPayload;



typedef struct ZCLPayload_UER_t {
	uint16_t manfCode;
	uint16_t imgType;
	uint32_t fileVer;
	uint32_t curTime;
	uint32_t ugTime;
}ZCLPayload_UER;


typedef struct ZCLPayload_IN_t {
	uint8_t PayloadType;
	uint8_t QueryJitter;
	uint16_t ManfCode;
	uint16_t ImageType;
	uint32_t FileVersion;
}ZCLPayload_IN;


typedef struct ZCLPayload_Query_Response_t {
	uint8_t status;
	uint16_t ManfCode;
	uint16_t ImageType;
	uint32_t FileVersion;
	uint32_t imageSize;
}ZCLPayload_Query_Response;

typedef struct ZCLPayload_Query_Request_t {
	uint8_t FC;
	uint16_t manfCode;
	uint16_t imgType;
	uint32_t fileVer;
	uint8_t Hardware_Ver;
	uint8_t Hardware_Rev;
}ZCLPayload_Query_Request;

typedef struct OTA_QueryRequest_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload_Query_Request zclPayload;
}OTA_QueryRequest;

typedef struct OTA_QueryResponse_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload_Query_Response zclPayload;
}OTA_QueryResponse;

typedef struct OTA_ImageNotify_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload_IN zclPayload;
}OTA_ImageNotify;

typedef struct OTA_ImgBlockResponse_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload zclPayload;
}OTA_ImgBlockResponse;


typedef struct OTA_UpgradeEndResponse_t {
	APSHeader apsHeader;
	ZCLHeader zclHeader;
	ZCLPayload_UER zclPayload;
}OTA_UpgradeEndResponse;

// DEFINE DAYS OF THE WEEK
#define SUNDAY    (0)
#define MONDAY    (1)
#define TUESDAY   (2)
#define WEDNESDAY (3)
#define THURSDAY  (4)
#define FRIDAY    (5)
#define SATURDAY  (6)

// week of the month
#define WEEK_1ST     0
#define WEEK_2ND     1
#define WEEK_3RD     2
#define WEEK_4TH     3
#define WEEK_5TH     4

// month of the year
#define MONTH_JAN    1
#define MONTH_FEB    2
#define MONTH_MAR    3
#define MONTH_APR    4
#define MONTH_MAY    5
#define MONTH_JUN    6
#define MONTH_JLY    7
#define MONTH_AUG    8
#define MONTH_SEP    9
#define MONTH_OCT    10
#define MONTH_NOV    11
#define MONTH_DEC    12


class FLGlobal : public QObject
{
	Q_OBJECT

public:
	
	~FLGlobal();
	static DeviceModels *getDeviceModels()
	{
		if(gDevModels == nullptr)
		{
			gDevModels = new DeviceModels();
			FLGlobal::populateDevices();
		}
		return gDevModels;
	}
	static bool VerifyInputContent(QLineEdit* edit,QWidget*parent);
private:
	FLGlobal(QObject *parent);
	static void populateDevices();
	static DeviceModels* gDevModels;
};

inline QString _q_string(string str)
{
	return QString::fromStdString(str);
}
#endif // FLGLOBAL_H
