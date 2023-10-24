/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1698158230_2_
#define _BUR_1698158230_2_

#include <bur/plctypes.h>

/* Constants */
#ifdef _REPLACE_CONST
 #define MAI_PERSISTERS 3U
 #define NUM_PERSISTERS 4U
#else
 _GLOBAL_CONST unsigned char MAI_PERSISTERS;
 _GLOBAL_CONST unsigned char NUM_PERSISTERS;
#endif


/* Variables */
_GLOBAL plcbit gClearTrace;
_GLOBAL_RETAIN unsigned char gPermConfiguration[2000];
_GLOBAL_RETAIN unsigned char gPermAxisCfgs[1000];
_GLOBAL_RETAIN unsigned char gPermAxisPars[5000];
_GLOBAL_RETAIN unsigned char gPermHomeData[1000];
_GLOBAL_RETAIN plcbit gDataValid[4];
_GLOBAL struct CSVFileMgr_typ gPermBackup[4];
_GLOBAL struct Persistence_typ gPersister[4];
_GLOBAL plcbit gSimulation;





__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"Logical/Infrastructure/Infrastructure.var\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1698158230_2_ */

