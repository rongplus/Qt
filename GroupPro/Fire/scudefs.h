//   ************************************************************************
//   **            Company Confidential - For Internal Use Only            **
//   **          Mircom Technologies Ltd. & Affiliates ("Mircom")          **
//   **                                                                    **
//   **   This information is confidential and the exclusive property of   **
//   ** Mircom.  It is intended for internal use and only for the purposes **
//   **   provided,  and may not be disclosed to any third party without   **
//   **                prior written permission from Mircom.               **
//   **                                                                    **
//   **                        Copyright 1997-2017                         **
//   ************************************************************************

// Revision history
// ----------------
// B. Oliver 97-10-16 - Original; FA-1000
// C. Calin 2016-04-19 - Adjusted to fit FireLink project.
// C. Calin 2016-10-03 - Add 'lamp test' and 'info' software switches.
//                     - Add CFG_AGENCY_ULC feature flag.
// C. Calin 2016-11-16 - Removed non-applicable process type: waterflow
//                     - Add CFG_AGENCY_ULC feature flag.
//                     - Add method external declarations:
//                         SCULoadConfigFromFlash()
//                     - Declare job file structure SCU_JOB_FILE. 
//                       This will be used to temporary store protobuffer data 
//                       from PC before conversion to actual data format.
// C. Calin 2016-11-24 - Reduced number of WIOs from 50 to 49.
//                     - Removed macros refering to organizing circuits
//                       as configurable/fixed.
//                     - Add method external declarations:
//                         SCULoadConfigFromFlash()
//                     - Declare job file structure SCU_JOB_FILE. 
//                       This will be used to temporary store protobuffer data 
//                       from PC before conversion to actual data format.
// C. Calin 2017-01-10 - Add 'auto-daylight saving' option flag.
// C. Calin 2017091011 - Support for 'default configuration' request through
//                       main board DIP switch.
// D. Zhang 2017-01-26 - Merged Protocol Buffer message decoding into 
//                          DecodeProtoBuffer()
// D. Zhang 2017-02-02 - Merged Protocol Buffer message encoding into
//                          SCUEncode()
// 

//  Description
//  -----------
//
//  Defines the following:
//    - circuit types
//    - constants for the state of various circuits types
//    - functions processing the state of ciruits in the map
//
#ifndef SCUDEFS_H
#define SCUDEFS_H

#include "typedefs.h"
#include "mcostype.h"
#include "mcosdefs.h"

//////////////////////////////////
//
//  MAX I/O SYSTEM CONSTANTS
//  ------------------------
//
//  These constants define the maximum number of i/o circuits
//  for defining the size of various data structures etc.
//
//  If these maximums are changed, ?????
//
#define  MAX_CKT_INPUTS   4  // TOTAL OF ALM, SUPV, TRB, MON CKTS
#define  MAX_CKT_OUTPUTS  4  // TOTAL OF SIL & NON-SIL SIGNALS AND STROBES

#define  MAX_WIO    49  // MAX WIOs
#define  MAX_CPU    ((MAX_WIO)+ 2)  // ZC + MAX WIOs + PC-TOOL

#define MAX_IO_CKTS  (MAX_CKT_INPUTS \
                      + MAX_CKT_OUTPUTS )
//  SCU Basic Types
typedef BYTE	 CKT_TYPE;
typedef BYTE	 CKT_NO;
typedef BYTE	 STATUS;
typedef STATUS   MASK;

/**********************************************************************
*  Circuit Types
*********************************************************************/
//  The following cicuit types are for input messages and system status
//
//  The numbers must be contiguous as they are used as indexes in the
//  partition table (gSCUPartitionMap).

#define CKT_INPUTS          0  // INPUT CIRCUITS
#define CKT_OUTPUTS         1  // OUTPUT CIRCUITS
#define CKT_SYS_STAT        2  // SYSTEM STATUS FLAGS
#define CKT_SIG_RATE        3  // SIGNAL RATE CONTROL
#define CKT_TIMERS          4  // SYSTEM TIMERS
#define CKT_CFGTST_SW       5  // CONFIGURATION/TEST DIP SW
#define CKT_CPU             6  // LOCAL CPU STATUS
#define CKT_UNIT_STAT       7  // GENERAL UNIT STATUS
#define CKT_MISC_IPT        8  // BITS FROM IOU
#define CKT_MISC_OPT        9  // BITS TO IOU
#define CKT_BUZZER          10 // ALERT BUZZER
#define CKT_DSU_STAT        11 // SHARED DISPLAY STATUS
#define CKT_DSU_QSTAT       12 // SHARED DISPLAY QUEUE STATUS
#define CKT_SERVICE_TOOL    13 // SERVICE TOOL ACTIVITY
#define CKT_WIO_INPUTS      14 // REMOTE WIO INPUTS
#define CKT_WIO_OUTPUT      15 // GLOBAL REMOTE WIO OUTPUT

#define MAX_ZC_CKT_TYPES    16 // NUMBER OF CIRCUIT TYPES IN STATUS MAP

#define CKT_INVALID_TYPE   ((CKT_TYPE)-1)  //USED IN SOME TABLES AS A DELIMITER

#define CKT_INVALID_NO     ((CKT_NO)-1)

#define SCUIsCfgCktType(Type) ((Type)==CKT_INPUTS \
                               || (Type)== CKT_OUTPUTS \
                               || (Type)== CKT_WIO_INPUTS \
                               ||  (Type)== CKT_WIO_OUTPUT )

#define MAX_ZC_CFG_TYPES    2

#define SCUIsCktType(CktType) (CKT_INPUTS == (CktType) \
                              || CKT_OUTPUTS == (CktType))

//  Input Only Ckts
//  ---------------
//  These circuit types don't appear in status map (input messages only)
//
//  They don't have to be contiguous but can't overlap the status map types

#define CKT_SWITCH       (MAX_ZC_CKT_TYPES + 1)  // CONTROL SWITCHES
#define CKT_SW_DSPL      (MAX_ZC_CKT_TYPES + 2)  // DISPLAY CONTROL SWITCHES (FOR DSU)
#define CKT_CC_LED_CTL   (MAX_ZC_CKT_TYPES + 3)  // FPU COMMON CONTROL LEDS
#define CKT_LED_RATE_CTL (MAX_ZC_CKT_TYPES + 4)  // FPU LED RATE GENERATOR
#define CKT_BUZZER_CTL   (MAX_ZC_CKT_TYPES + 5)  // FPU BUZZER CONTROL

//  System Status
//  -------------
//
//  These defines are designed to be used in conjunction with
//  the flag functions SCUSetFlag, SCUClearFlag, SCUChkFlag.
//
//  Each macro replaces all three parameters in the functions.
//
//#define STAT_GRNDFLT  CKT_MISC_IPT, 0, MISCIPT_GRNDFLT  // CORRESPOND TO INPUT MESSAGE
//#define STAT_REMTRB   CKT_MISC_IPT, 0, MISCIPT_REMTRB
//#define STAT_PWRFAIL  CKT_MISC_IPT, 0, MISCIPT_PWRFAIL
//#define STAT_BATTRB   CKT_MISC_IPT, 0, MISCIPT_BATTRB
//#define STAT_AUXTRB   CKT_MISC_IPT, 0, MISCIPT_AUXTRB
//#define STAT_4WIRETRB CKT_MISC_IPT, 0, MISCIPT_4WIRETRB
//#define STAT_CONTTRB  CKT_MISC_IPT, 0, MISCIPT_CONTTRB
//#define STAT_CTYTRB   CKT_MISC_IPT, 0, MISCIPT_CTYTRB

//#define STAT_ALM_RELAY_ACTIVE   CKT_MISC_OPT, 0, MISCOPT_ALMRELAY
//#define STAT_SUPV_RELAY_ACTIVE  CKT_MISC_OPT, 0, MISCOPT_SUPVRELAY
//#define STAT_TRB_RELAY_ACTIVE   CKT_MISC_OPT, 0, MISCOPT_TRBRELAY

//#define STAT_AUX_ALM_RELAY_ACTIVE    CKT_MISC_OPT, 0, MISCOPT_AUX_ALM_RELAY
//#define STAT_ALM_XMIT_ACTIVE    CKT_MISC_OPT, 0, MISCOPT_ALM_XMIT


#define SYS_STAT_INITDONE    0 // ALL UNITS HAVE REPORTED INIT DONE
#define SYS_STAT_COMMON_ALM  1 // ANY FIRE ALARM ACTIVE
#define SYS_STAT_COMMON_SUPV 2 // ANY SUPERVISORY ALARM ACTIVE
#define SYS_STAT_COMMON_TRB  3 // ANY TROUBLE ACTIVE
#define SYS_STAT_AUTO_SS_TIMING  4 // SIGNAL SILENCE TIMING ACTIVE
#define SYS_STAT_EVAC_ACTIVE 5 // N/A
#define SYS_STAT_ALARM_ACK   6 // N/A
#define SYS_STAT_FIRE_DRILL  7 // FIRE DRILL ACTIVE
#define SYS_STAT_SIG_SIL     8 // SIGNALS HAVE BEEN SILENCED
#define SYS_STAT_BUZZER_SIL  9 // BUZZER SILENCED
#define SYS_STAT_CONFIG_MODE 10 // PANEL IN CONFIGURATION MODE
#define SYS_STAT_SYSRESET    11 // SYTEM RESET ACTIVE
#define SYS_STAT_WALKTEST    12 // TBD
#define SYS_AC_ON            13 // AC is present
#define SYS_STAT_SIL_INHIB   14 // SIGNAL SILENCE INHIBIT 

#define MAX_STAT         15 // NUMBER OF FLAGS IN CKT_SYS_STAT

#define STAT_INITDONE          CKT_SYS_STAT, SYS_STAT_INITDONE
#define STAT_COMMON_ALM        CKT_SYS_STAT, SYS_STAT_COMMON_ALM
#define STAT_COMMON_SUPV       CKT_SYS_STAT, SYS_STAT_COMMON_SUPV
#define STAT_COMMON_TRB        CKT_SYS_STAT, SYS_STAT_COMMON_TRB
#define STAT_AUTO_SS_TIMING    CKT_SYS_STAT, SYS_STAT_AUTO_SS_TIMING
#define STAT_EVAC_ACTIVE       CKT_SYS_STAT, SYS_STAT_EVAC_ACTIVE
#define STAT_ALARM_ACK         CKT_SYS_STAT, SYS_STAT_ALARM_ACK
#define STAT_FIRE_DRILL        CKT_SYS_STAT, SYS_STAT_FIRE_DRILL
#define STAT_SIG_SIL           CKT_SYS_STAT, SYS_STAT_SIG_SIL
#define STAT_BUZZER_SIL        CKT_SYS_STAT, SYS_STAT_BUZZER_SIL
#define STAT_SYSRESET_ACTIVE   CKT_SYS_STAT, SYS_STAT_SYSRESET
#define STAT_WALKTEST_ACTIVE   CKT_SYS_STAT, SYS_STAT_WALKTEST
#define STAT_CONFIG_MODE       CKT_SYS_STAT, SYS_STAT_CONFIG_MODE
#define STAT_AC_ON             CKT_SYS_STAT, SYS_AC_ON
#define STAT_SIG_SIL_INHIB     CKT_SYS_STAT, SYS_STAT_SIL_INHIB

/******************************************************************************
//  STATUS DEFINITIONS
*******************************************************************************/
//  The following constants are used to define valid states for
//  the various ciruit types


//  I/O CIRCUITS
//  ------------
//  Input and output ciruit status contains three pieces of
//  information:
//     - alarm state (3 bits)
//     - trouble state (3 bits)
//     - bypass state (1 bit flag)
//
//  Note there are macros to assist the various fields from the byte.
//  (see SCUSetAlarmStatus etc)
//

//
//  Generic Status (specific status should be used where possible)
//
//
#define ALARM_MASK 0x07

#define ALARM0   0x00	 // NORMAL CONDITION, NO ALARM IS ACTIVE
#define ALARM1   0x01
#define ALARM2   0x02
#define ALARM3   0x03
#define ALARM4   0x04
#define ALARM5   0x05
#define ALARM6   0x06
#define ALARM7   0x07

#define TROUBLE_MASK 0x38

#define TROUBLE0  0x00 // NORMAL CONDITION, NO TROUBLE IS ACTIVE
#define TROUBLE1  0x08
#define TROUBLE2  0x10
#define TROUBLE3  0x18
#define TROUBLE4  0x20
#define TROUBLE5  0x28
#define TROUBLE6  0x30
#define TROUBLE7  0x38

#define TRB_NORMAL  TROUBLE0   //   - NO TRB ACTIVE
#define TRB_OPEN    TROUBLE1   //   - CONTACT DEVICES & CONV CKTS
#define TRB_SHORT   TROUBLE2   //   - OUTPUT CIRCUITS/DEVICES

#define BYPASS_FLAG 0x40

#define SCUChkTroubleStatus( CktType, CktNo )            \
  ( SCUGetStatusMask( TROUBLE_MASK, (CktType), (CktNo) ) != TRB_NORMAL )

//
//  Specific Status for I/O Circuits
//  --------------------------------
//
//  IPT CKTS:
#define ALM_NORMAL         ALARM0 //    - NORMAL STATE
#define ALM_THRESHOLD      ALARM4 //    - ANY THING BELOW THIS IS PRE-ALARM
#define ALM_RESETING       ALARM1 //    - ALARM CIRCUITS
#define ALM_VER_IN_PROCESS ALARM2 //    - VERIFIED ALARM ONLY
#define ALM_SHORT          ALARM4 //    - "ACTIVE STATE" FOR ALL INPUTS

// SIG AND STROBE CKTS ARE CONTROLED BY RATE GENERATORS:
#define OPT_NORMAL         RGEN_OFF     // FOR SIGS AND STROBES (ALWAYS OFF)
#define OPT_EVAC           RGEN_SIG_2   // RATE SET IN SIGRATE_SIG_GEN
#define OPT_CONTINUOUS     RGEN_SIG_7   // ALWAYS ON

// RELAY OUTPUT CIRCUITS (CKT_RELAY)
#define OPT_RELAY_NORMAL  ALARM0
#define OPT_RELAY_ACTIVE  ALARM1
                                 // ALERT FOR BOTH IPT & OPT
#define ALERT_NORMAL          ALERT0  //   - NO ALERT ACTIVE
#define ALERT_THRESHOLD       ALERT1  //  ANY THING BELOW THIS IS PRE-ALERT
#define ALERT_RESETING        ALERT1  // ALERT CIRCUITS
#define ALERT_VER_IN_PROCESS  ALERT2  // VERIFIED ALERT ONLY
#define ALERT_SHORT           ALERT1  //"ACTIVE STATE" FOR ALL INPUTS

//
//  Signal Rates (for CKT_SIG_RATE)
//  ------------
//
//  Two rate generators may be programmed through the CKT_SIG_RATE
//  outputs either of these can be selected to signal or strobe circuits
//  (CKT_SIL_SIG, CKT_NSIL_SIG, CKT_SIL_STROBE, CKT_NSIL_STROBE)
//
//    SIGRATE_SIG_GEN is sets normal continuous signal patterns
//                   and is applied to the signal circuits by selecting
//                   them to OPT_SIG_ACTIVE
//
//    SIGRATE_WT_GEN is sets the special patterns used for walktest
//                   and is applied to the signal circuits by selecting
//                   them to OPT_SIG_WALKTEST
//
//  The actual code is set by outputting to the lower 7 bits
//  (SIGRATE_RATE_MASK).  The MSB (SIGRATE_ACTIVE_FLAG) is input
//  and indicates when the SIGRATE_WT_GEN is sending the code (goes
//  to 0 when the code is done).
//
#define SIGRATE_SIG_GEN 0
#define SIGRATE_WT_GEN  1

#define MAX_SIG_RATE_GEN  2

#define SIGRATE_RATE_MASK   0x7f    // BITS FOR WALKTEST CODE
#define SIGRATE_ACTIVE_FLAG 0x80    // SET WHEN RATE IS SOUNDING


//
// Rates for SIGRATE_SIG_GEN
// (Correspond to offsets in SPIRateContFixed[] IN spirate.c)
//
//  **** NOTE THAT THESE MUST BE UNIQUE NUMBERS FOR CONFIG
//      TO WORK (CFUERate.c)
//
#define SIGRATE_OFF         0
#define SIGRATE_PS          4
#define SIGRATE_MARCH_TIME  5
#define SIGRATE_CONTINUOUS  1
#define SIGRATE_TEMPORAL    6
#define SIGRATE_CALIFORNIA  7

//
//  Rate Generator Constants
//  ------------------------
//
//  The following constants identify the patterns
//  for the signal, LED, and buzzer rates.
//
//  The first 8 rates are reserved for signal and strobe circuits,
//  and range from 0 to 7.  They are used with the signal circuit types
//  (e.g. CKT_NSIL_SIG) to specify what rate the signals is connected to.
//
//  The last 8 rates are for the LEDs and the buzzer and can be used
//  with output control messages (e.g. CKT_LED_RATE_CTL) to set these
//  rates.
//
//  The LEDs and buzzer can be configured to any rate generator, while
//  the signals can only use the first 8.
//
//  If you change these, remember to change the SPIRateDatas
//   in spitask/c/spirate.c
//

#define RGEN_OFF            0  // Signal generator 0 - Continuously OFF
#define RGEN_SIG_1          1  // Signal generator 1 - PS or GA as set
#define RGEN_SIG_2          2  // Signal generator 2 - Not configured
#define RGEN_SIG_3          3  // Signal generator 3 - Not configured
#define RGEN_SIG_4          4  // Signal generator 4 - Not configured
#define RGEN_SIG_5          5  // Signal generator 5 - Not configured
#define RGEN_SIG_6          6  // Signal generator 6 - Walktest
#define RGEN_SIG_7          7  // Signal generator 7 - Continuously ON

#define RGEN_LED_ON         8  // Continuously ON
#define RGEN_LED_SLOWFLASH  9  // Continuous Slow Flash (30  SPM)
#define RGEN_LED_FASTFLASH 10  // Continuous Fast Flash (120 SPM)
#define RGEN_LED_CUSTOM    11  // N Cycles at March     (60  SPM)


//
//  Shared Display Queue Status (for CKT_DSU_QSTAT)
//  ---------------------------
//
//  Shows the status of various queues in the DSU.
//
//  Circuit number is the DSU number (see above).
//
//  2 bits are defined for each queue to show if there are any entries
//  in the queue and if so if they have all been viewed
//

#define DSU_ALMQ_MASK 0x03
#define DSU_SPVQ_MASK 0x0c
#define DSU_TRBQ_MASK 0x30
#define DSU_MONQ_MASK 0xc0

#define DSU_ALMQ_EMPTY 0x00        // THE QUEUE IS EMPTY
#define DSU_SPVQ_EMPTY 0x00
#define DSU_TRBQ_EMPTY 0x00
#define DSU_MONQ_EMPTY 0x00

#define DSU_ALMQ_UNVIEWED 0x01     // THE QUEUE HAS UNVIEWED ITEMS
#define DSU_SPVQ_UNVIEWED 0x04
#define DSU_TRBQ_UNVIEWED 0x10
#define DSU_MONQ_UNVIEWED 0x40

#define DSU_ALMQ_VIEWED 0x02       // ALL ITEMS IN QUEUE ARE VIEWED
#define DSU_SPVQ_VIEWED 0x08
#define DSU_TRBQ_VIEWED 0x20
#define DSU_MONQ_VIEWED 0x80


//
//  Unit Status (for CKT_UNIT_STAT)
//  -----------
//
#define  UNIT_INITDONE 0x01  // INITIALISATION COMPLETE
#define     UNIT_RESET 0x02  // SYSTEM RESET IN PROCESS
#define UNIT_CFGUPDATE 0x04  // CONFIG UPDATE MODE ACTIVE
#define     UNIT_TRB_1 0x10  // TROUBLE BITS (UNIT SPECIFIC)
#define     UNIT_TRB_2 0x20
#define     UNIT_TRB_3 0x40
#define     UNIT_TRB_4 0x80

#define UNIT_TRB_MASK 0xf0  // TESTS ALL TROUBLE BITS

//
//  Unit Numbers (for CKT_UNIT_STAT)
//  ------------
//
//  *** IMPORTANT ***
//
//  When a ne unit is created a master message handling task must be
//  created in mcxdefs.c and added to MSUTskLstGenBroadcast.
//
//  The gSCUPartitionMapDflt must also be adjusted.
//

#define UNIT_IIU 0    // "CIRCUIT NUMBERS" FOR UNITS
#define UNIT_FPU 1
#define UNIT_MCU 2
#define UNIT_MSU 3
#define UNIT_SCU 4
#define UNIT_TMU 5
#define UNIT_CFU 6
#define UNIT_SVU 7
#define UNIT_DSU 8
#define UNIT_HWU 9


#define UNIT_INVALID  ((CKT_NO)-1)    // USED AS A DELIMITER IN LISTS

#define MAX_UNIT 10  // NUMBER OF UNITS (ALSO IN gSCUPartitionMap)

//
//  System Timers (for CKT_TIMERS)
//  -------------
//
//
                           // TIMERS ("CIRCUIT NUMBER"):
#define SYSTMR_GA       0      //    AUTO GA
#define SYSTMR_SIG_SIL  1      //    AUTO SIGNAL SILENCE
#define SYSTMR_INHIB    2      //    SIGNAL SILENCE INHIBIT

                           // TIMER STATES
#define SYSTMR_IDLE      0
#define SYSTMR_TIMING    1
#define SYSTMR_CANCELLED 2
#define SYSTMR_EXPIRED   3

//
//  These macros are designed to replace all three parameters
//  in SCUSetStatus, SCUChkStatus to set and check the state
//  of a given timer.
//

#define SYSTMR_GA_IDLE       CKT_TIMERS, SYSTMR_GA, SYSTMR_IDLE
#define SYSTMR_GA_TIMING     CKT_TIMERS, SYSTMR_GA, SYSTMR_TIMING
#define SYSTMR_GA_CANCELLED  CKT_TIMERS, SYSTMR_GA, SYSTMR_CANCELLED
#define SYSTMR_GA_EXPIRED    CKT_TIMERS, SYSTMR_GA, SYSTMR_EXPIRED

#define SYSTMR_SIG_SIL_IDLE       CKT_TIMERS, SYSTMR_SIG_SIL, SYSTMR_IDLE
#define SYSTMR_SIG_SIL_TIMING     CKT_TIMERS, SYSTMR_SIG_SIL, SYSTMR_TIMING
#define SYSTMR_SIG_SIL_CANCELLED  CKT_TIMERS, SYSTMR_SIG_SIL, SYSTMR_CANCELLED
#define SYSTMR_SIG_SIL_EXPIRED    CKT_TIMERS, SYSTMR_SIG_SIL, SYSTMR_EXPIRED

#define SYSTMR_INHIB_IDLE       CKT_TIMERS, SYSTMR_INHIB, SYSTMR_IDLE
#define SYSTMR_INHIB_TIMING     CKT_TIMERS, SYSTMR_INHIB, SYSTMR_TIMING
#define SYSTMR_INHIB_CANCELLED  CKT_TIMERS, SYSTMR_INHIB, SYSTMR_CANCELLED
#define SYSTMR_INHIB_EXPIRED    CKT_TIMERS, SYSTMR_INHIB, SYSTMR_EXPIRED


//
//  Misc Inputs (for CKT_MISC_IPT from IOU)
//  -----------
//
#define MISCIPT_GRNDFLT       0x01
//#define MISCIPT_REMTRB      0x02
#define MISCIPT_PWRFAIL       0x04
#define MISCIPT_BATTRB        0x08

//#define MISCIPT_AUXTRB      0x10
//#define MISCIPT_4WIRETRB    0x20

//#define MISCIPT_CONTTRB     0x40
//#define MISCIPT_CTYTRB      0x80

//
//  Misc Outputs (for CKT_MISC_OPT to IOU)
//  -----------
//
#define MISCOPT_ALMRELAY        0x01
#define MISCOPT_SUPVRELAY       0x02
#define MISCOPT_TRBRELAY        0x04
//#define MISCOPT_AUX_ALM_RELAY   0x10
//#define MISCOPT_ALM_XMIT        0x20

//
//
//  Shared Display Status (for CKT_DSU_STAT)
//  ---------------------
//
//  Circuit number is the shared display (DSU) number which is 0
//  for the master or the cpu number on the external bus for remotes.
//
//  Show current status of the specified display

#define MAX_DSUS 1

#define   DSU_STAT_QUEUE_INFO 0x01  // INFO BUTTON PRESSED
#define  DSU_STAT_MANUAL_DISP 0x02  // MANUAL QUEUE DISPLAY
#define DSU_STAT_COMMAND_MODE 0x04  // COMMAND MODE ACTIVE
#define    DSU_STAT_LAMP_TEST 0x08  // LAMP TEST ACTIVE


//  Buzzer States (for CKT_BUZZER)
//  -----------------------------
//
//  Two bits track the rate and three bits track whether that rate
//  has been silenced.
//
//  Priority for buzzer rates is as following:
//  alarm
//  supervisory
//  trouble
//  turned-off

//  The rate will play provided the corresponding silence flag
//  is not activated.

#define BUZZER_OFF         0x00
#define BUZZER_TRB         0x01
#define BUZZER_SPV         0x02
#define BUZZER_ALM         0x03

#define BUZZER_RATE_MASK    (STATUS)0x03

#define BUZZER_TRB_SIL_FLAG (STATUS)0x04
#define BUZZER_ALM_SIL_FLAG (STATUS)0x08
#define BUZZER_SPV_SIL_FLAG (STATUS)0x10

#define BUZZER_SIL_MASK     (STATUS)0x1C


typedef BYTE NODE_NO; // identifies ZC number on RS-485 bus
#define MAX_NODES     16
#define NODE_REMOTE_PC ((MAX_NODES)+1)// PC configuration is highest node

#define SCUGetNodeNo() ((CPU_NO)(HWUGetNodeNo()))
//
//  CPU Status (for CKT_CPU_STAT)
//  ----------
//
//  Circuit number is the CPU_NO
//
//  CPU number must be unique throughout the system.
//
//  CPU number 0 is always the master CPU.
//  Numbers 1-31 are used for CPUs on the local RS485 bus
//  and 33-63 for the external bus - 32 is invalid.
//
typedef BYTE CPU_NO;

#define      CPUNO_MASTER  0   // MASTER CPU
#define CPUNO_SERVICETOOL  ((MAX_CPU) - 1)  // RESERVED FOR LAPTOP PROGRAMMER

#define CPUNO_INVALID ((CPU_NO)-1)

#define     CPU_ONLINE 0x00  // CPU IS UP AND RUNNING
#define   CPU_INITDONE 0x02  // CPU INITIALISATION COMPLETE (FROM SVU)
#define    CPU_OFFLINE 0x03  // CPU HAS A TROUBLE ACTIVE

/////////////////////////////////////////////////////////////
//  CKT PROCESS INFO DEFINITIONS
//  ----------------------------
//
//  These constants are used for CKT_INPUTS and CKT_OUTPUTS to define how
//  they are processed.  Some are also applied to CKT_SYS_STAT to deturmine
//  how correlations to these status are processed.
//
//  The information is stored in one byte per circuit (4 bits each for
//  type and flag)
//
//  Use the SCUGetProcessType and SCUChkProcessFlag service routines
//  to retrieve the process info for a given CKT_INPUTS or CKT_OUTPUTS
//
//  *** NOTE:  If process types are added or changed the strings in
//            gSTRDSUProcTypes must be updated
//
typedef BYTE PROCESS_TYPE;
typedef BYTE PROCESS_FLAG;

#define PROCESS_TYPE_MASK 0x0f
#define PTYPE_INVALID  0xff      // BIGGER THAN 4 BITS!

                         // INPUT PROCESS TYPES (CKT_INPUTS):
#define PTYPE_ALARM        0x00 // ALARM
#define PTYPE_SUPVLATCH    0x01 // LATCHED SUPERVISORY
#define PTYPE_SUPVNONLATCH 0x02 // NON-LATCHED SUPERVISORY
#define PTYPE_TROUBLE      0x03 // TROUBLE ONLY

                         // OUTPUT PROCESS TYPES (CKT_OUTPUTS):
#define PTYPE_SIGNAL       0x04
#define PTYPE_STROBE       0x05
#define PTYPE_RELAY        0x06

#define PTYPE_MAX          0x07

                         // PROCESS FLAGS:
#define PFLAG_SILENCEABLE   0x10 // NOT USED FOR NOW


extern const PROCESS_TYPE gSCUCktTypeList[];      // ALL I/O CIRCUITS
extern const PROCESS_TYPE gSCUAlmTypeList[];       // CIRCUITS WHICH PUT SYSTEM IN ALARM
extern const PROCESS_TYPE gSCUSpvTypeList[];      // ALL INPUT CIRCUITS
extern const PROCESS_TYPE gSCULatchedIPTypeList[];// INPUT CKTS WHICH MUST BE LATCHED TO RESET
extern const PROCESS_TYPE gSCUIPTypeList[];       // ALL INPUT CIRCUITS
extern const PROCESS_TYPE gSCUSigTypeList[];      // ALL SIGNAL CIRCUITS
extern const PROCESS_TYPE gSCURelayTypeList[];    // ALL RELAY CIRCUITS
extern const PROCESS_TYPE gSCUOPTypeList[];       // ALL OUTPUT CIRCUITS

#define SCUChkProcessType( CKT_TYPE, CKT_NO, PROCESS_TYPE ) \
  ( SCUGetProcessType( (CKT_TYPE), (CKT_NO) ) == (PROCESS_TYPE) )

#define SCUIsAlarmType( PROCESS_TYPE ) \
  ( SCULookupType( gSCUAlmTypeList, (PROCESS_TYPE) ) )

#define SCUIsSpvType( PROCESS_TYPE ) \
  ( SCULookupType( gSCUSpvTypeList, (PROCESS_TYPE) ) )

#define SCUIsLatchedIPType( PROCESS_TYPE ) \
  ( SCULookupType( gSCULatchedIPTypeList, (PROCESS_TYPE) ) )

#define SCUIsIPType( PROCESS_TYPE ) \
  ( SCULookupType( gSCUIPTypeList, (PROCESS_TYPE) ) )

#define SCUIsOPType( PROCESS_TYPE ) \
  ( SCULookupType( gSCUOPTypeList, (PROCESS_TYPE) ) )

#define SCUIsSigType( PROCESS_TYPE ) \
  ( SCULookupType( gSCUSigTypeList, (PROCESS_TYPE) ) )

#define SCUIsRelayType( PROCESS_TYPE ) \
  ( SCULookupType( gSCURelayTypeList, (PROCESS_TYPE) ) )
    

//////////////////////////////////////////////////////////
//
//  FUNCTIONS FOR ACCESSING STATUS MAP
//  ----------------------------------
//
//  These functions set, retrive and test the states of circuits in the
//  status map.
//
//  Many are implemented as macros.
//

//
//  Basic Functions:
//
//     SCUSetStatus - sets the status of a given ckt
//     SCUGetStatus - retrieves the status of a given ckt
//     SCUCmpStatus - compares against a given status (like strcmp)
//     SCUChkStatus - chks if equal to a given status (logical function)
//

extern void  SCUSetStatus( CKT_TYPE, CKT_NO, STATUS );
extern STATUS SCUGetStatus( CKT_TYPE, CKT_NO );
extern int SCUCmpStatus( CKT_TYPE, CKT_NO, STATUS );
extern BOOL SCUChkStatus( CKT_TYPE, CKT_NO, STATUS );

//
//  Mask Functions
//  --------------
//
//  These functions are similar to the base functions but
//  a mask is provided to operate on only part of a status byte
//

#define SCUSetStatus(CktType, CktNo, Status ) \
          SCUSetStatusMask( (STATUS)-1, CktType, CktNo, Status )

#define SCUGetStatus( CktType, CktNo ) \
          SCUGetStatusMask( (STATUS)-1, CktType, CktNo )

extern void  SCUSetStatusMask( MASK, CKT_TYPE, CKT_NO, STATUS );
extern STATUS SCUGetStatusMask( MASK, CKT_TYPE, CKT_NO );
extern int SCUCmpStatusMask( MASK, CKT_TYPE, CKT_NO, STATUS );

#define SCUChkStatusMask( Mask, CktType, CktNo, Status )              \
    ( SCUGetStatusMask( (Mask), (CktType), (CktNo) ) == (Status) )


extern BOOL SCUChkBitStatArray( STATUS *pDest, WORD BitNo );
extern void SCUSetBitStatArray( STATUS *pDest, WORD BitNo, WORD MaxSize, BOOL bTrue );


//
//  Flag Functions
//  --------------
//
//  Operate on bit flags in the status
//
extern void SCUSetFlag( CKT_TYPE, CKT_NO, MASK );
extern void SCUClearFlag( CKT_TYPE, CKT_NO, MASK );
extern BOOL SCUChkFlag( CKT_TYPE, CKT_NO, MASK );

//
//  Bit Offset Functions
//  --------------------
//
//  Treats the each bit in the ckt type as an individual status
//
extern void SCUSetBitOffset( CKT_TYPE, WORD );
extern void SCUClearBitOffset( CKT_TYPE, WORD );
extern BOOL SCUChkBitOffset( CKT_TYPE, WORD );

//
//  Other Useful Status related Functions
//  -------------------------------------
//
//  SCUSetAllStatus - sets the status of all ckts of a given type
//  SCUSetAllStatusMask - as above for selected bits
//  SCUChkAllStatus - returns TRUE if all ckts of a given type are at a
//                   given status
//
//  SCUGetCktCount - returns number of ckts of given type in the status map
//


extern void SCUSetAllStatus( CKT_TYPE, STATUS );
extern void SCUSetAllStatusMask( MASK, CKT_TYPE, STATUS );
extern BOOL SCUChkAllStatus( CKT_TYPE, STATUS );
extern BOOL SCUChkAllStatusMask( MASK, CKT_TYPE, STATUS );
extern BOOL SCUGetAllPTypeStatusMask(  MASK Mask, PROCESS_TYPE PType );
extern void SCUSetAllPTypeStatusMask(  MASK Mask, PROCESS_TYPE PType, STATUS Status );

CKT_NO SCUGetCktCount( CKT_TYPE );

//
//  Alarm and Trouble Functions
//  ---------------------------
//
//  Used for processing the alarm and trouble state in
//  the I/O circuit status
//

#define SCUSetAlarmStatus( CktType, CktNo, state )              \
  ( SCUSetStatusMask( ALARM_MASK, (CktType), (CktNo), (state) ) )

#define SCUSetTroubleStatus( CktType, CktNo, state )       \
  ( SCUSetStatusMask( TROUBLE_MASK, (CktType), (CktNo), (state) ) )

#define SCUGetAlarmStatus( CktType, CktNo )                     \
  ( SCUGetStatusMask( ALARM_MASK, (CktType), (CktNo) ) )

#define SCUGetTroubleStatus( CktType, CktNo )                   \
  ( SCUGetStatusMask( TROUBLE_MASK, (CktType), (CktNo) ) )

#define SCUCmpAlarmStatus( CktType, CktNo, state )              \
  ( SCUCmpStatusMask( ALARM_MASK, (CktType), (CktNo), (state) ) )

#define SCUCmpTroubleStatus( CktType, CktNo, state )            \
  ( SCUCmpStatusMask( TROUBLE_MASK, (CktType), (CktNo), (state) ) )

#define SCUChkAlarmStatus( CktType, CktNo )              \
  ( SCUGetStatusMask( ALARM_MASK, (CktType), (CktNo) ) >= ALM_THRESHOLD )

//////////////////////////////////////
//
//  SYSTEM MODE FUNCTIONS
//  ---------------------
//
//
typedef BYTE SYS_MODE;
    
#define SYSMODE_NORMAL            0
#define SYSMODE_PANEL_CONFIG      1

typedef BYTE SETMODE_FLAG; 
#define     SETMODE_NO_ACTION 0   // NO ACTION
#define    SETMODE_SAVE_FLASH 1   // CONFIG DATA MUST BE LOADED FROM FLASH
#define SETMODE_RECOVER_FLASH 2   // CONFIG DATA SHOULD BE RELOADED

extern SYS_MODE SCUGetSystemMode( void );
extern void SCUSetSystemMode( SYS_MODE SystemMode, SETMODE_FLAG SetModeFlag);

#define SCUChkSysStat( STAT ) \
     ( SCUChkFlag( STAT, (STATUS)1 ) != 0 )   // MACRO FOR CHECKING THESE FLAGS

       
///////////////////////////////////
//
//  SCUChkFeature and bit masks.
//  ----------------------------
//
typedef LWORD SCU_FEATURE_FLAGS;

// Keep it in sync with WIO macros
#define CFG_MAN_SIGSIL  0x00000001ul // ENABLE MANUAL SIGNAL SILENCE
#define CFG_AGENCY_ULC  0x00000002ul // UL/ULC AGENCY
#define CFG_AUTO_DST    0x00000004ul // DAYLIGHT SAVE ACTIVE

extern BOOL SCUChkFeature( SCU_FEATURE_FLAGS BitMask );


////////////////////////////////////////////////////////////
//
//  OTHER FUNCTIONS
//  ---------------
//

extern void SCUInit( void );       // INITIALIZE THE STATUS MAP
extern void SCULoadConfigFromFlash(void);


////////////////////////////////////////////////////////////
//
// Configuration macros
//
typedef BYTE CFG_ID;                       // TABLE TYPES

//#define         CFGID_SCU_ROM_VERSION 0x20
//#define         CFGID_SCU_CONFIG_INFO 0x21
//#define       CFGID_SCU_PARTITION_MAP 0x22
#define         CFGID_ACTUAL_TROUBLES 0x23

#define CFGID_INVALID  ((CFG_ID)-1)

#define     CFGTYPE_ROM 0
#define CFGTYPE_CFGDATA 1
#define     CFGTYPE_RAM 2

typedef struct SCU_LOOKUP_TABLE_ENTRY_t              // TABLE INFO STRUCTURE
  {
  CFG_ID  DataID;                // STRUCTURE ID
  TASK_NO Owner;                 // TASK WHICH OWNES THE DATA
  BYTE   DataType;               // CFGTYPE_ROM ETC.
  WORD   EntrySz;				         // SIZE OF ONE ENTRY
  WORD   MaxEntries;             // MAX NUMBER OF ENTRIES
  void   *pData;                 // START LOCATION IN RAM
  WORD   *pnEntries;             // NUMBER OF ENTRIES IN TABLE
  void   *pDfltData;			       // DEFAULT DATA START ADDRESS IN RAM
  WORD   *pnEntriesDflt;	       // DEFAULT NUMBERS OF ELEMENTS ADDRESS IN RAM
  void   *pCfgBuffer; 		       // CONFIGURATION BUFFER START ADDRESS IN RAM
  FLASH_ADDRESS *pDataFlash;     // DATA ADDRESS IN FLASH
  FLASH_ADDRESS *pnEntriesFlash; // NUMBER OF ELEMENTS ADDRESS IN FLASH
  } SCU_LOOKUP_TABLE_ENTRY;

extern SCU_LOOKUP_TABLE_ENTRY gCFULookupTable[];


///////////////////////////////////////////////////////////
//
//  SYSTEM WIDE CONFIGURATION INFORMATION
//  -------------------------------------
//
//   The following section contains various common system configuration
//  information
//


//
//  CPU Type
//  --------
//

typedef BYTE CPU_TYPE;

#define CPUTYPE_ZC   0   // Zone Controller
#define CPUTYPE_WIO  1   // Router


#define CPUTYPE_PC_CONFIG 0x20   // PC BASED CONFIGURATOR

#define CPUTYPE_INVALID  ((CPU_TYPE)-1)

extern CPU_TYPE SCUGetCPUType( CPU_NO CPUNo );


// Map of CPU numbers to WIO logical circuit type
extern CPU_NO gSCUOuputWIOToCpu[MAX_WIO];
extern CPU_NO gSCUInputWIOToCpu[MAX_WIO];

//
//  System Type
//  -----------
//
//  The system or build type must be the same on all CPU's and reflects
//  table sizes etc. supported in the build.
//
//  It is stored both in the config info and version info.
//
typedef BYTE SYS_TYPE;

#define   SYSTYPE_ZC  0
#define   SYSTYPE_WIO 1

#define SYSTYPE_INVALID  ((SYS_TYPE)-1)

//
//  Language Type
//  -------------
//
//  The language type reflects which message tables have been linked into
//  the firmware.  It does not have to be the same on all CPU's (different
//  annunciators can support different languages).
//
typedef BYTE LANG_TYPE;

#define LANGTYPE_ENGLISH 0
#define  LANGTYPE_FRENCH 1

#define LANGTYPE_INVALID  ((LANG_TYPE)-1)

//
//  Version Numbers
//  -------------------
//
//  The VERS_NO type stores a version number as 3 digits intended to be
//  displayed as Vx.y.z.  Where x is referred to as the major version
//  number 0-255, y is the minro version number 0-255 and z is the
//  incremental version number.
//
//  For compatibility checking, generally the Major and Minor Version numbers
//  must be identical but the incremental version may be different.  For test
//  purposes, versions with the major and minor numbers both 0 are considered
//  to be compatible with anything.
//
typedef LWORD VERS_NO;

#define VERSNO_BASE    0x00000000  // V0.0.0
#define VERSNO_INVALID ((VERS_NO)-1)

#define SCUGetMajorVersNo( VERS_NO ) \
  ( (BYTE)( ((VERS_NO) >> 24) & 0xff ) )

#define SCUGetMinorVersNo( VERS_NO ) \
  ( (BYTE)( ((VERS_NO) >> 16) & 0xff ) )

#define SCUGetMajorMinorVersNo( VERS_NO ) \
  ( (WORD)( (VERS_NO) >> 16 ) )

#define SCUGetIncrVersNo( VERS_NO ) \
  ( (WORD)(VERS_NO & 0xffff ) )

#define SCUChkCompatibleVers( VERS1, VERS2 ) \
  ( SCUGetMajorMinorVersNo( VERS1 ) == 0    \
   || SCUGetMajorMinorVersNo( VERS2 ) == 0  \
   || SCUGetMajorMinorVersNo( VERS1 ) == SCUGetMajorMinorVersNo( VERS2 ) )


// Macros to set the proper version number
// using major minor and increment as separate numbers

#define SCUSetMajorVersNo( major ) ( ((VERS_NO)major & 0x000000ff) << 24 )
#define SCUSetMinorVersNo( minor ) ( ((VERS_NO)minor & 0x000000ff) << 16 )
#define SCUSetIncrVersNo( incr )   ( (VERS_NO)incr & 0x0000ffff )

#define SCUSetVersNo( major, minor, incr )  \
            SCUSetMajorVersNo( major )      \
            | SCUSetMinorVersNo( minor )    \
            | SCUSetIncrVersNo( incr )



//
//  Hardware Info
//  -------------
//
//  On newer CPU boards info is stored in a register on the hardware itself.
//
//  It consists of two fields:
//    HWType - identifies the base type of board we are running on
//    VersNo - version of the board
//
//  At boot time this information is validated against the current build info
//  in gSCUCodeVersion.
//
//  For legacy hardware, without the hardware info register, HWType is
//  set to HWTYPE_BASE and the VersNo is set to VERSNO_BASE.
//
typedef BYTE  HWTYPE;
#define XDR_HWTYPE   XDR_BYTE

#define HWTYPE_ZC       0x00
#define HWTYPE_WIO      0x00

#define HWTYPE_INVALID    ((HWTYPE)-1)

typedef struct SCU_CODE_VERS_ENTRY_t
  {
  CPU_TYPE CPUType;
  char      szProdID[15];      // "FireLink"
  char      szCopyright[40];
  VERS_NO   VersNo;            // MAJOR/MINOR MUST BE SAME ON ALL CPUS
  SYS_TYPE  SysType;
  LANG_TYPE LangType;
  HWTYPE    HWType;            // MUST BE SAME AS gSCUHWInfo
  VERS_NO   HWVers;            // MAJOR/MINOR MUST BE SAME AS gSCUHWInfo
  char date[21];
  int32_t sign;
  } SCU_CODE_VERS_ENTRY;


extern ROM SCU_CODE_VERS_ENTRY gSCUCodeVersion;         // DON'T USE THIS...
#define SCUGetCodeVersion()       \
    ((const SCU_CODE_VERS_ENTRY *)&gSCUCodeVersion) //  ... USE THIS!


//
//  Display Control Switches  (CKT_SW_DSPL)
//  ------------------------
//
//  Used with CKT_SW_DSPL input events or input status msg
//  CktNo identifies switch.
//
//  These inputs are processed by the DSU.
//
#define SW_UP           0
#define SW_SIG_SIL 		  1 // Dual operation switch
#define SW_LEFT         1 //
#define SW_DOWN         2      
#define SW_SYS_RESET 	  3 // Dual operation switch
#define SW_RIGHT        3 //
#define SW_ENTER        4
#define SW_MENU         5
#define SW_CANCEL       6 
#define SW_INFO         7
        
#define SW_MAX          8 

// Software switches   
#define SW_LAMP_TEST    253    
#define SW_BUZZER_SIL   254 // N/A            

typedef BYTE SW_BUTTON;
typedef BYTE SW_STAT;


#define SWI_OFF  (SW_STAT)0
#define SWI_ON   (SW_STAT)1
#define SWI_MASK (SW_STAT)1

// Job data structure definition

#define SCU_JOB_DATA_SIZE 5000 // keep it in sync with  ZC_size 
// Actual data allocated must match 1k boundaries for X-modem to work properly
#define SCU_JOB_DATA_MAX  ((SCU_JOB_DATA_SIZE / 1024 )*1024 + 1024) 

typedef struct SCU_JOB_FILE_t // keep it in sync with SVU_JOB_FILE
  {
  unsigned long size;
  unsigned long checksum;
  BYTE data[SCU_JOB_DATA_MAX];// will hold the protobuffer
  } SCU_JOB_FILE;

extern SCU_JOB_FILE gSCUJobFile;

// Macros to detect default configuration request
#define SCU_SW_DFLT_CFG 0x01
#define SCUChkCfgDfltFlag(p) ((p) & SCU_SW_DFLT_CFG)

extern void SCUGetCfg(void);

// Protocol Buffer message decoding implemented in DecodeProtoBuffer()
BOOL DecodeProtoBuffer( BYTE* pSrc, void* pDst, size_t ValidSize, const uint32_t Type_ID );

// Protocol Buffer message encoding for ZC or CFG_RTC structure
BOOL SCUEncode( void* pSrc, BYTE* pProtoBuff, WORD MaxSize, uint32_t* encodedSize, BYTE CFG_TYPE);

#endif

