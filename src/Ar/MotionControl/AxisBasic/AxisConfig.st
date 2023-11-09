(*
 * File: Types.typ
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of AxisLib, licensed under the MIT License.
 *)

ACTION AxisConfig:
	
	// Assign axis pointers.
	gMotorBasic[Axis_1].pAxisObject := ADR(gAxis_1);
	gMotorBasic[Axis_2].pAxisObject := ADR(gAxis_2);
	gMotorBasic[Axis_3].pAxisObject := ADR(gAxis_3);
	gMotorBasic[Axis_4].pAxisObject := ADR(gAxis_4);

END_ACTION
