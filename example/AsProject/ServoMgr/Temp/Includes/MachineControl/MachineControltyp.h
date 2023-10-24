/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1698158343_1_
#define _BUR_1698158343_1_

#include <bur/plctypes.h>

/* Datatypes and datatypes of function blocks */
typedef enum SUBSTATES
{	SUB_STATE_INIT,
	SUB_STATE_SAFETY_RESET,
	SUB_STATE_SERVO_POWER,
	SUB_STATE_ENABLE_AXIS_ENAGAGE,
	SUB_STATE_ENABLE_PATH_SYNC,
	SUB_STATE_STARTING_SET_AXES,
	SUB_STATE_STARTING_CNC_LIMITS,
	SUB_STATE_STARTING_AXIS_LIMITS,
	SUB_STATE_STARTING_CNC_START,
	SUB_STATE_STARTING_CNC_STARTED,
	SUB_STATE_STARTING_CNC_FAILED,
	SUB_STATE_STOPPING_STOP_CNC,
	SUB_STATE_DISABLE_AXIS_ENAGAGE,
	SUB_STATE_SERVO_STOP,
	SUB_STATE_SERVO_POWER_OFF
} SUBSTATES;

typedef enum MODE_MACHINE_ENUM
{	MODE_MACHINE_AUTO,
	MODE_MACHINE_MANUAL_PROGRAM,
	MODE_MACHINE_MANUAL_JOG
} MODE_MACHINE_ENUM;






__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"Logical/MachineControl/MachineControl.typ\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1698158343_1_ */

