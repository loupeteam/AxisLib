/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1698850590_4_
#define _BUR_1698850590_4_

#include <bur/plctypes.h>

/* Constants */
#ifdef _REPLACE_CONST
 #define MAI_MOTOR 3U
 #define NUM_MOTOR 4U
#else
 _GLOBAL_CONST unsigned char MAI_MOTOR;
 _GLOBAL_CONST unsigned char NUM_MOTOR;
#endif


/* Variables */
_GLOBAL_RETAIN struct MC_ENDLESS_POSITION gEndlessPosition[4];
_GLOBAL struct AxisBasic_typ gMotorBasic[4];
_GLOBAL struct ACP10AXIS_typ gMotorNcm[4];
_GLOBAL struct R_TRIG homeCheck;
_GLOBAL plcbit homeAll;





__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"C:/Projects/Loupe/AxisLib/src/Ar/Servo/ServoControl.var\\\" scope \\\"global\\\"\\n\"");
__asm__(".ascii \"iecfile \\\"Logical/Libraries/standard/standard.fun\\\" scope \\\"global\\\"\\n\"");

/* Additional IEC dependencies */
__asm__(".ascii \"iecdep \\\"C:/Projects/Loupe/AxisLib/src/Ar/Servo/ServoControl.typ\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1698850590_4_ */

