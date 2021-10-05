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
		ActualCyclicPosition : MC_CYCLIC_POSITION;
		ActualVelocity : REAL;
		DriveStatus : MC_DRIVESTATUS_TYP;
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

FUNCTION AxisBasicFn_Fast : BOOL (*Manage setting and reading cyclic positions. Intended to run in a faster task class than most motion tasks.*) (*$GROUP=User*)
	VAR_IN_OUT
		t : AxisBasic_typ;
	END_VAR
END_FUNCTION

FUNCTION axisInternalFastFn : BOOL (*Internal: Handle ReadCyclicPosition and MoveCyclicPosition FUBs*) (*$GROUP=User*)
	VAR_IN_OUT
		t : AxisBasic_typ;
	END_VAR
END_FUNCTION

FUNCTION_BLOCK AxisRestorePosition (*DEPRECATED: Handle endless position (Init, Check, Restore) for an axis*)
	VAR_INPUT
		Axis : UDINT;
		Execute : BOOL;
		DataAddress : UDINT;
	END_VAR
	VAR_OUTPUT
		Done : BOOL;
		Busy : BOOL;
		Error : BOOL;
		ErrorID : UINT;
		EndlessPositionInitialized : BOOL;
		DataValid : BOOL;
	END_VAR
	VAR
		Internal : AxisRestorePosition_Int_typ;
	END_VAR
END_FUNCTION_BLOCK

FUNCTION_BLOCK AxisExpandLimit (*Temporarily set the limits of an axis 
-This function block is useful when an axis is controlled using MC_BR_MoveCyclicPos and the axis is outside it's axis limits*)
	VAR_INPUT
		Axis : UDINT; (*Pointer to axis object*)
		Enable : BOOL; (*Enable expanded limits*)
		Tolerance : REAL; (*Additional space to give the axis in axis units*)
		AcknowledgeError : BOOL; (*Acknowledge error and retry setting the limits*)
	END_VAR
	VAR_OUTPUT
		Active : BOOL; (*The expanded limits are active*)
		Busy : BOOL; (*The limits are being set*)
		Error : BOOL;
		ErrorID : UINT;
	END_VAR
	VAR
		Internal : AxisExpandLimit_Internal_typ;
	END_VAR
END_FUNCTION_BLOCK
