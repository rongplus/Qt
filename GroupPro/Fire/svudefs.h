//  svudefs.h - definitions for the Supervisor Unit
//

//   ************************************************************************
//   **            Company Confidential - For Internal Use Only            **
//   **          Mircom Technologies Ltd. & Affiliates ("Mircom")          **
//   **                                                                    **
//   **   This information is confidential and the exclusive property of   **
//   ** Mircom.  It is intended for internal use and only for the purposes **
//   **   provided,  and may not be disclosed to any third party without   **
//   **                prior written permission from Mircom.               **
//   **                                                                    **
//   **                        Copyright 1998-2017                         **
//   ************************************************************************
//
//  Revision History
//  ----------------
//  M. Nadeem 2007-06-06 - Original
//  M. Nadeem 2011-07-25 - Added more error code pertaining to firmware update process
//  C. Calin  2016-06-23 - Add EEPROM-related declarations. Removed un-used
//                         Flash-related declarations.
//  C. Calin  2016-07-15 - Add macros for watchdog maintaining process.
//  S. Zimmo  2016-07-19 - Fixed macro TRB_CFG_INTERRUPTED; was 0xc8, now 0xc6.
//  C. Calin  2016-10-03 - bug#5404; Add TRB_LAMPTEST declaration.
//                            SVUCalc16BitCheckSum()
//                            SVUCalc8BitCheckSum()
//                            SVUCalc32BitCRC()
//                            SVUGetConfiguration()
//                            SVUSaveConfiguration()
// C. Calin 2016-11-30 - Add SVUClearTroubles() declaration.
// C. Calin 2017-01-11 - Add trouble for 'default configuration' switch off
//                       normal.
// C. Calin 2017-01-20 - Add bSVUFlashType3125 flash identification flag.
// D. Zhang 2017-02-03 - Add SVUSaveRTC() 

#ifndef SVUDEFS_H
#define SVUDEFS_H

#include "typedefs.h"
#include "mcostype.h"
#include "scudefs.h"
#include <string.h>

typedef BYTE TRB_CODE;

// Local CPU trouble codes
#define TRB_NO_TROUBLE     0x00   // INVALID TROUBLE CODE
//-----------------------------------------------------------------
#define TRB_POWERUP        0x01   // SYSTEM INITIALIZATION IN PROCESS
#define TRB_SYSUPDATE      0x02   // SYSTEM UPDATE IN PROCESS
#define TRB_SYSRESET       0x03   // SYSTEM RESET IN PROCESS
#define TRB_EEPROM         0x04   // EEPROM READ/WRITE FAILED
#define TRB_UNIT_TRB       0x05   // UNIT TROUBLE IN CKT_UNIT_STAT
#define TRB_BAD_LOGENTRY   0x06   // EVENT LOG CHKSUM ERR    LOG #, ENTRY (OLDEST 0)
#define TRB_EVLOG_INIT     0x07   // EV LOG INIT ERROR (WIPED)  LOG, PART
#define TRB_BATTERY_NOT_PRESENT  0x08// Troubles for Battery circuit
#define TRB_BATTERY_FAULT  0x09
#define TRB_BATTERY_LOW    0x0a

#define TRB_CHRGR_FAULT    0x0b
#define TRB_BOOSTER_FAULT  0x0c // NAC power sopply trouble
// spare                   0x0d
#define TRB_AC_SUPPLY      0x0e
#define TRB_GND_FLT        0x0f

#define TRB_SIG_SIL        0x10
#define TRB_CFG_NOTPROG    0x11    // USER MUST CHECK PROGRAMMING
#define TRB_TRBCKT_ALARM   0x12    // TROUBLE ONLY INPUT CIRCUIT ACTIVE
#define TRB_LAMPTEST       0x13    // LAMPTEST SUPERVISION, DSU NO
#define TRB_SERVICE_TOOL   0x14    // SERVICE TOOL CONNECTED
#define TRB_IPT_CKT        0x15    // TROUBLES ON IPT CKTs
#define TRB_OPT_CKT        0x16    // TROUBLES ON OPT CKTs
#define TRB_CFG_SW         0x17    // CONFIGURATION SWITCH OFF NORMAL

                        // TROUBLES LATCHED TO CPU RESET
#define TRB_RAM_DATACHK   0x40    // RAM READ/WRITE ERROR
#define TRB_RAM_ADDRCHK   0x41    // RAM ADDRESS ERROR
#define      TRB_ROMCHK   0x42    // ROM CHECKSUM ERROR
#define    TRB_STACKCHK   0x43    // STACK CHECK ERROR
#define    TRB_FLASHCHK   0x44    // FLASH CHECK ERROR
#define  TRB_SYSMSGPOOL   0x45    // SYSTEM MSG POOL NOT EMPTY
#define  TRB_MCXMSGPOOL   0x46    // MCX MSG POOL NOT EMPTY

                            // CODES FOR INFORMATIONAL MESSAGES
#define      TRB_INF_COMMAND 0xb0  // COMMAND EXECUTED        DSU #, CMD_ID
#define TRB_INF_ACCESS_LEVEL 0xb1  // ACCESS LEVEL CHANGED    DSU #, ACCESS LVL
#define    TRB_INF_PSWD_FAIL 0xb2  // PASSWORD ENTRY ERROR    DSU #, ACCESS LVL
#define    TRB_INF_CLEAR_LOG 0xb3  // CLEAR EVENT LOG         DSU #, LOG #

                           // VERY SERIOUS TROUBLES, CPU REBOOT USUALLY REQUIRED
#define   TRB_HEARTBEAT_TMR 0xc0    // HEARTBEAT TIMER IS NOT RUNNING
#define   TRB_TASKHEARTBEAT 0xc1    // NO RESPONSE FROM A TASK FOR HEARTBEAT
#define          TRB_ASSERT 0xc2    // ASSERTION FAILED
#define    TRB_ILLEG_OPCODE 0xc3    // ILLEGAL OP CODE
#define    TRB_BAD_INTERUPT 0xc4    // INTERRUPT VECTOR
#define     TRB_CFG_VERSION 0xc5    // CONFIG VERSION # ERROR
#define TRB_CFG_INTERRUPTED 0xc6    // SYSTEM WAS POWERED DOWN IN CONFIG MODE
#define TRB_FLASH_RW_FAIL   0xc7    // READ/WRITE FLASH FAILED
#define     TRB_HW_VERSION  0xc8    // WRONG PANEL VERSION


#define   MAX_NONLATCH_TRB TRB_RAM_DATACHK   // LAST NON-LATCHING TROUBLE
#define MAX_RESETLATCH_TRB TRB_INF_COMMAND   // LAST TROUBLE CLEARED AT RESET
#define   MAX_NONFATAL_TRB TRB_HEARTBEAT_TMR   // LAST NON-FATAL TROUBLE
#define            MAX_TRB 0xee   // HIGHEST TRB CODE


#define MAX_TRB_MAP  (((MAX_TRB) / (sizeof(STATUS) * 8 ) \
                      + (((MAX_TRB) % (sizeof(STATUS) * 8)) ? 1 : 0 )))

extern STATUS gSVUActiveTrb[MAX_TRB_MAP];


// Here are troubles from remote units
#define REM_TRB_NO_TROUBLE            0x00     // INVALID TROUBLE CODE
//-----------------------------------------------------------------
#define  REM_TRB_POWERUP              0x01    // SYSTEM INITIALIZATION IN PROCESS
#define  REM_TRB_SYSUPDATE            0x02    // SYSTEM UPDATE IN PROCESS
// spare                              0x03
// spare                              0x04
#define  REM_TRB_UNIT_TRB             0x05    // UNIT TROUBLE IN CKT_UNIT_STAT
#define  REM_TRB_EVLOG_INIT           0x06    // EV LOG INIT ERROR (WIPED)  LOG, PART
#define  REM_TRB_BAD_LOGENTRY         0x07    // EVENT LOG CHKSUM ERR    LOG #, ENTRY (OLDEST 0)
#define  REM_TRB_STUCK_SWI            0x08    // STUCK SWITCH
// spare                              0x09
// spare                              0x0a
#define  REM_TRB_BATTERY_NOT_PRESENT  0x0b    // Troubles for Battery circuit
#define  REM_TRB_BATTERY_FAULT        0x0c
// Spare                              0x0d
#define  REM_TRB_CHRGR_FAULT          0x0e
#define  REM_TRB_BOOSTER_FAULT        0x0f    // NAC power supply trouble
// spare                              0x10
#define  REM_TRB_AC_SUPPLY            0x11
// Spare                              0x12
// Spare                              0x13
// Spare                              0x14
// Spare                              0x15
// Spare                              0x16
#define  REM_TRB_IPT_CKT_OPEN         0x17    // INPUT CKT TROUBLE
// SPARE                              0x18    // LEAVE ENOUGH FOR MAX IPTS
#define REM_TRB_IPT_CKTS               2 // CORRELATE WITH TRB_OPT_CKT_OPEN

//#if MAX_CKT_INPUTS != REM_TRB_IPT_CKTS
//# error "MAX_CKT_INPUTS != TRB_IPT_CKTS"
//#endif

#define  REM_TRB_OPT_CKT_OPEN         0x19    // OPT CKT TROUBLE
// SPARE                              0x1a
#define  REM_TRB_OPT_CKT_SHORT        0x1b    // OPT CKT TROUBLE
// SPARE                              0x1c    // LEAVE ENOUGH FOR MAX IPTS
#define REM_TRB_OPT_CKTS               2 // CORRELATE WITH TRB_OPT_CKT_OPEN

//#if MAX_CKT_OUTPUTS != TRB_OPT_CKTS
//# error "MAX_CKT_OUTPUTS != TRB_OPT_CKTS"
//#endif

#define REM_TRB_SYSRESET              0x1e  // SYSTEM RESET IN PROCESS
#define REM_CPU_OFFLINE               0x1f  // CPU OFFLINE

// ....
// Spare                              0x20 - 0x3F
//
                            // TROUBLES LATCHED TO CPU RESET
#define REM_TRB_RAM_DATACHK   0x40    // RAM READ/WRITE ERROR
#define REM_TRB_RAM_ADDRCHK   0x41    // RAM ADDRESS ERROR
#define      REM_TRB_ROMCHK   0x42    // ROM CHECKSUM ERROR
#define    REM_TRB_STACKCHK   0x43    // STACK CHECK ERROR
#define    REM_TRB_FLASHCHK   0x44    // FLASH CHECK ERROR
#define  REM_TRB_SYSMSGPOOL   0x45    // SYSTEM MSG POOL NOT EMPTY
#define  REM_TRB_MCXMSGPOOL   0x46    // MCX MSG POOL NOT EMPTY


// Up to this trouble will be sent out to master; make sure is multiple of 8
#define  REM_MAX_PUBLIC_TRB  (REM_TRB_MCXMSGPOOL)

// CODES FOR INFORMATIONAL MESSAGES
#define REM_TRB_INF_COMMAND      0xb0  // COMMAND EXECUTED        DSU #, CMD_ID
#define REM_TRB_INF_ACCESS_LEVEL 0xb1  // ACCESS LEVEL CHANGED    DSU #, ACCESS LVL
#define REM_TRB_INF_PSWD_FAIL    0xb2  // PASSWORD ENTRY ERROR    DSU #, ACCESS LVL
#define REM_TRB_INF_CLEAR_LOG    0xb3  // CLEAR EVENT LOG         DSU #, LOG #

                               // FATAL TROUBLES:
#define REM_TRB_HEARTBEAT_TMR    0xc0    // HEARTBEAT TIMER IS NOT RUNNING
#define REM_TRB_TASKHEARTBEAT    0xc1    // NO RESPONSE FROM A TASK FOR HEARTBEAT
#define REM_TRB_ASSERT           0xc2    // ASSERTION FAILED
#define REM_TRB_ILLEG_OPCODE     0xc3    // ILLEGAL OP CODE
#define REM_TRB_BAD_INTERUPT     0xc4    // INTERRUPT VECTOR
#define REM_TRB_CFG_VERSION      0xc5    // CONFIG VERSION # ERROR
#define REM_TRB_CFG_INTERRUPTED  0xc8    // SYSTEM WAS POWERED DOWN IN CONFIG MODE
#define REM_TRB_FLASH_RW_FAIL    0xc9    // READ/WRITE FLASH FAILED

#define REM_MAX_NONLATCH_TRB     REM_TRB_RAM_DATACHK    // LAST NON-LATCHING TROUBLE
#define REM_MAX_RESETLATCH_TRB   REM_MAX_PUBLIC_TRB     // LAST TROUBLE CLEARED AT RESET
#define REM_MAX_NONFATAL_TRB     REM_TRB_HEARTBEAT_TMR  // LAST NON-FATAL TROUBLE

#define REM_MAX_TRB 0xfe   // HIGHEST TRB CODE

#define REM_MAX_TRB_MAP  (((REM_MAX_TRB) / (sizeof(STATUS) * 8 ) \
                      + (((REM_MAX_TRB) % (sizeof(STATUS) * 8)) ? 1 : 0 )))

typedef struct SVU_CPU_TRB_MAP_t
  {
  STATUS Trb[ REM_MAX_TRB_MAP ];
  } SVU_CPU_TRB_MAP;

// Two-dimension array to hold all WIO troubles
extern SVU_CPU_TRB_MAP gSVURemActiveTrb[MAX_WIO];

//
//  SVU Basic Types
//  ---------------
//

typedef BYTE	TRB_SEVERITY;

#define TRBSEV_INFORMATION 0    // TROUBLE SEVERITY LEVELS
#define TRBSEV_WARNING     1
#define TRBSEV_PROCEDURAL  2
#define TRBSEV_SERIOUS     3
#define TRBSEV_FATAL       4

#define TRBSEV_UNKNOWN  ((TRB_SEVERITY)-1)

typedef LWORD	TRB_INFO;
#define TRBINFO_UNKNOWN ((TRB_INFO)-1)

//
//  SVUMakeTrbInfo is a utility macro to pack to words of data into the
//  TRB_INFO (32bit) value
#define SVUMakeTrbInfo( word1, word2 ) \
           ( (TRB_INFO)( ( (LWORD)( (word1) & 0xffff ) << 16 ) \
                       | ( (LWORD)( (word2) & 0xffff ) ) ) )

extern void SVULogTroubleOnce( CPU_NO CPUNo, TRB_CODE TrbCode, BOOL bStatus,
                        TRB_SEVERITY Severity, TRB_INFO TrbInfo );
extern void SVULogTrouble( CPU_NO CPUNo, TRB_CODE TrbCode, BOOL bStatus,
                           TRB_SEVERITY Severity, TRB_INFO TrbInfo );
extern void SVUFatalTrouble( TRB_CODE TrbCode, TRB_INFO TrbInfo );

// Define data structure to save fatal trouble information
typedef struct SVU_SAFERAM_ENTRY_t 
  {
  struct
    {
    TRB_CODE Code;
    TRB_INFO Info;
    } Trb;

  WORD CheckSum;
  } SVU_SAFERAM_ENTRY;
extern SVU_SAFERAM_ENTRY * pSVUSafeRAM;

// Address spared from '0' initialization
// at start-up
#define SCU_SAFE_RAM_ADDR (0x2002ffff - sizeof(SVU_SAFERAM_ENTRY)) 

extern void SVUCheckOfflineTrb( void );

#define ASSERT  // enable the assert trouble
extern void SVUAssert( BOOL bFatal );

extern void SVUInit( void );
extern BOOL SVUSetActiveTrb( CPU_NO CPUNo, TRB_CODE TrbCode, BOOL bSet );
extern BOOL SVUAnyActiveTrb( void );
extern BOOL SVUChkActiveTrb( CPU_NO CPUNo, TRB_CODE TrbCode );
extern void SVUProcessHeartbeat( void );
extern BOOL SVUChkTrbStat( CPU_NO CPUNo, TRB_CODE TrbCode );
extern void SVUClearTroubles( void );

extern void SVUEEPromTableInit( void );

extern WORD SVUCalc16BitCheckSum( const void *pData, WORD Len );
extern BYTE SVUCalc8BitCheckSum( const void *pData, WORD Len );
extern INT32U SVUCalc32BitCRC( BYTE *pSrc, WORD nBytes );

extern void SVUGetConfiguration(BYTE *pJobSuperStruct, WORD SzMax);
extern BOOL SVUSaveConfiguration(BYTE *pJobSuperStruct, WORD Size);

enum SVUFlashType_t {
  TYPE_3125,
  TYPE_5123,
  TYPE_INVALID};

extern enum SVUFlashType_t SVUFlashType; // Indicates if flash chip is old type or new type.
extern BOOL SVUDetectFlashType(void);

// Added by D. Zhang
extern void SVUSaveRTC( void );



/////////////////////////////////////////////////////
//
//  FLASH Memory access type definitions and routines
//  -------------------------------------------------
//
//

#define ERASED_WORD        (0xFFFF)     // ERASED PATTERN IN FLASH
#define ZEROED_WORD        (0x0000)

typedef BYTE FLASH_TYPE;

#define FLASHTYPE_CFGDATA 0
#define FLASHTYPE_VERSION 1
#define   FLASHTYPE_EVLOG 2
#define   FLASHTYPE_CLOCK 3

#define FLASHTYPE_INVALID ((FLASH_TYPE)-1)

extern EEPROM_ADDRESS clockstart;
extern EEPROM_ADDRESS loginfostart;

// Watchdog macros
// ---------------
#define SVU_TASK_RUN_INTERVAL 500 //SVUMain run intervals

#define SVU_WD_TIMEOUT 200 // WD will reset CPU in 200ms

#define SVU_WD_GRACE_TIME 2000 // make sure for 2 seconds from start-up WD is maintained

#define SVU_WD_START_UP ((SVU_WD_GRACE_TIME)/(SVU_WD_TIMEOUT))

#define SVU_WD_DLY_FACTOR 2 // the bigger the more delays from SVUMain are allowed

#define SVU_WD_REFILL (((SVU_TASK_RUN_INTERVAL) / (SVU_WD_TIMEOUT)) *  SVU_WD_DLY_FACTOR)

extern int SVUWDCounts;

// OBS. Check with MX_TIM2_Init() when changing these macros

#endif


