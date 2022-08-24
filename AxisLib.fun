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
		Axis : UDINT;
		Enable : BOOL;
		ReadCyclicPositionParID : UINT; (*ParID that is read cyclically, 0... set position*)
	END_VAR
	VAR_OUTPUT
		ActualPosition : REAL;
		ActualCyclicPosition : LREAL;
		ActualVelocity : REAL;
		DriveStatus : AxisLib_DriveStatus_typ;
		PLCOpenState : AxisLib_PLCOpenState_typ;
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
		Axis : UDINT;
		EndlessPositionDataAddress : UDINT;
		DefaultPosition : REAL;
		WaitToInitialize : BOOL;
		Reference : BOOL;
		Position : REAL;
		HomingMode : USINT;
		ClearReference : BOOL;
	END_VAR
	VAR_OUTPUT
		Done : BOOL;
		Busy : BOOL;
		CommandAborted : BOOL; (*function block is aborted by another command*)
		Error : BOOL;
		ErrorID : UINT;
		Referenced : BOOL;
		EndlessPositionInitialized : BOOL;
		DataValid : BOOL;
	END_VAR
	VAR
		internal : AxisReference_Int_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION AxisBasicFn_Cyclic : BOOL (*Manage an axis for basic movements and status reporting*)
	VAR_IN_OUT
		t : AxisBasic_typ; (*Axis control object*)
	END_VAR
END_FUNCTION
