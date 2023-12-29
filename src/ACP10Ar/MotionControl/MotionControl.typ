(*
* File: MotionControl.typ
* Copyright (c) 2023 Loupe
* https://loupe.team
* 
* This file is part of AxisBasic, licensed under the MIT License.
*)

TYPE
	MOTOR_INDEX_GENERIC : 
		(
		CNC_MOTOR_1, (*0*)
		CNC_MOTOR_2,
		CNC_VMOTOR_3,
		CNC_VMOTOR_4,
		END_OF_MOTOR_GENERIC
	);
	MOTOR_INDEX_NAMES : 
		(
		CNC_MOTOR_Y,
		CNC_MOTOR_Z,
		CNC_VMOTOR_Y,
		CNC_VMOTOR_Z,
		END_OF_MOTOR
	);

END_TYPE
