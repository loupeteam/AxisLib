(*
 * File: Types.typ
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of AxisLib, licensed under the MIT License.
 *)

TYPE
	Configuration_typ : 	STRUCT 
		Axis : ARRAY[0..MAI_AXIS]OF AxisConfiguration_typ;
		velocity : REAL;
		acceleration : REAL;
		deceleration : REAL;
	END_STRUCT;
	AxisConfiguration_typ : 	STRUCT 
		power : BOOL;
		cfg : AxisBasic_IN_CFG_typ;
	END_STRUCT;
	Task_typ : 	STRUCT 
		cmd : Task_Cmd_typ;
		parameters : AxisBasic_IN_PAR_typ;
		status : Task_Status_typ;
		internal : Task_Internal_typ;
	END_STRUCT;
	Task_Cmd_typ : 	STRUCT 
		New_Member : BOOL;
	END_STRUCT;
	Task_Status_typ : 	STRUCT 
		state : ARRAY[0..MAI_AXIS]OF State_enum;
		axisError : ARRAY[0..MAI_AXIS]OF BOOL;
		axisErrorReset : ARRAY[0..MAI_AXIS]OF BOOL;
	END_STRUCT;
	Task_Internal_typ : 	STRUCT 
		axisInternal : ARRAY[0..MAI_AXIS]OF AxisBasic_Internal_typ;
		MC_MoveVelocity : ARRAY[0..MAI_AXIS]OF MC_MoveVelocity;
	END_STRUCT;
	(*Sim_typ : 	STRUCT 
		axisMoving : ARRAY[0..MAI_AXIS]OF BOOL;
	END_STRUCT;*)
	State_enum : 
		(
		ST_POWER_ON,
		ST_MOVE_VELOCITY,
		ST_ERROR
		);

END_TYPE
