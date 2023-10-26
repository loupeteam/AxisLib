(********************************************************************
 * COPYRIGHT -- B&R Industrial Automation
 ********************************************************************
 * Library: AxisLib
 * File: AxisLib.fun
 * Author: blackburnd
 * Created: March 30, 2010
 ********************************************************************
 * Functions and function blocks of library AxisLib
 ********************************************************************)

FUNCTION_BLOCK AxisStatus (*Gather status information about an axis*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Axis : REFERENCE TO McAxisType;
		Enable : BOOL;
		DisablePositionRead : BOOL;
	END_VAR
	VAR_OUTPUT
		Position : LREAL;
		Velocity : REAL;
		Info : AxisLib_AxisInfo_typ;
		AxisWarning : BOOL;
		CommunicationReady : BOOL;
		IsHomed : BOOL;
		PowerOn : BOOL;
		ReadyForPowerOn : BOOL;
		Simulation : BOOL;
		Valid : BOOL;
		Busy : BOOL;
		Error : BOOL;
		ErrorID : UINT;
	END_VAR
	VAR
		internal : AxisStatus_Int_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK AxisReference (*Handle axis referencing, including allowing jogging without a reference and maintaining reference status*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Axis : REFERENCE TO McAxisType;
		RestorePositionVariableAddress : UDINT;
		DefaultPosition : LREAL;
		WaitToInitialize : BOOL;
		Reference : BOOL;
		Position : LREAL;
		HomingMode : USINT;
		pInitHomingData : UDINT;
		szInitHomingData : UDINT;
		ClearReference : BOOL;
		Library : STRING[32];
	END_VAR
	VAR_OUTPUT
		Done : BOOL;
		Busy : BOOL;
		CommandAborted : BOOL; (*function block is aborted by another command*)
		Error : BOOL;
		ErrorID : UINT;
		Referenced : BOOL;
		DataValid : BOOL;
	END_VAR
	VAR
		internal : AxisReference_Int_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION AxisBasicCyclic : BOOL (*Manage an axis for basic movements and status reporting*)
	VAR_INPUT
		interface : AxisBasic_Api_typ;
		configuration : AxisBasic_IN_CFG_typ;
		internal : AxisBasic_Internal_typ;
	END_VAR
END_FUNCTION
