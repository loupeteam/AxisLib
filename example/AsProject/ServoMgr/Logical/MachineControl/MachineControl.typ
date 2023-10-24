(********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Package: MachineControl
 * File: MachineControl.typ
 * Author: David Blackburn
 * Created: November 10, 2011
 ********************************************************************
 * Data types of package MachineControl
 ********************************************************************)

TYPE
	SUBSTATES : 
		(
		SUB_STATE_INIT,
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
		);
	MODE_MACHINE_ENUM : 
		(
		MODE_MACHINE_AUTO,
		MODE_MACHINE_MANUAL_PROGRAM,
		MODE_MACHINE_MANUAL_JOG
		);

END_TYPE
