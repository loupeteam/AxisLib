/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1697823387_5_
#define _BUR_1697823387_5_

#include <bur/plctypes.h>

/* Datatypes and datatypes of function blocks */
typedef enum MOTOR_INDEX_GENERIC
{	CNC_MOTOR_1,
	CNC_MOTOR_2,
	CNC_VMOTOR_3,
	CNC_VMOTOR_4,
	END_OF_MOTOR_GENERIC
} MOTOR_INDEX_GENERIC;

typedef enum MOTOR_INDEX_NAMES
{	CNC_MOTOR_Y,
	CNC_MOTOR_Z,
	CNC_VMOTOR_Y,
	CNC_VMOTOR_Z,
	END_OF_MOTOR
} MOTOR_INDEX_NAMES;






__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"C:/Projects/Loupe/AxisLib/src/Ar/Servo/ServoControl.typ\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1697823387_5_ */

