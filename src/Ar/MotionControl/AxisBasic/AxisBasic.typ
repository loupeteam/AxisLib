(*
* File: AxisBasic.typ
* Copyright (c) 2023 Loupe
* https://loupe.team
* 
* This file is part of AxisBasic, licensed under the MIT License.
*)

TYPE
	Configuration_typ : 	STRUCT 
		Axis : ARRAY[0..MAI_MOTOR]OF AxisConfiguration_typ;
	END_STRUCT;
	AxisConfiguration_typ : 	STRUCT 
		Active : BOOL;
		Power : BOOL;
	END_STRUCT;
	axisHmi_typ : 	STRUCT 
		axis : ARRAY[0..MAI_MOTOR]OF axisHmiStatus_typ;
	END_STRUCT;
	axisHmiStatus_typ : 	STRUCT 
		powered : BOOL;
		referenced : BOOL;
		active : BOOL;
	END_STRUCT;
END_TYPE
