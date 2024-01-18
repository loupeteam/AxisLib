(*
* File: MachineControl.typ
* Copyright (c) 2023 Loupe
* https://loupe.team
* 
* This file is part of the MachineControl package, licensed under the MIT License.
*)

TYPE
	SUBSTATES : 
		(
		SUB_STATE_INIT,
		SUB_STATE_BOOTING_COMM_READY,
		SUB_STATE_RESETTING_POWER_ON,
		SUB_STATE_STOPPING_CYCLE_STOP,
		SUB_STATE_STOP,
		SUB_STATE_ABORTING_POWER_OFF
		);
	MODE_MACHINE_ENUM : 
		(
		MODE_MACHINE_AUTO,
		MODE_MACHINE_MANUAL_PROGRAM,
		MODE_MACHINE_MANUAL_JOG
		);
END_TYPE
