(********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Program: ServoProg
 * File: ServoProg.typ
 * Author: David
 * Created: March 07, 2011
 ********************************************************************
 * Local data types of program ServoProg
 ********************************************************************)

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
