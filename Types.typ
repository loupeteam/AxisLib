(********************************************************************
 * COPYRIGHT -- Automation Resources Group
 ********************************************************************
 * Library: AxisLib
 * Author: blackburnd
 * Created: March 30, 2010
 ********************************************************************
 * Data types of library AxisLib
 ********************************************************************)
(*Status*)

TYPE
	AxisStatus_Int_typ : 	STRUCT 
		ReadActualPosition : MC_ReadActualPosition;
		ReadActualVelocity : MC_ReadActualVelocity;
		ReadAxisInfo : MC_ReadAxisInfo;
	END_STRUCT;
	AxisLib_AxisInfo_typ : 	STRUCT 
		AdditionalInfo : McAddInfoType;
		AxisWarning : BOOL;
		CommunicationReady : BOOL;
		IsHomed : BOOL;
		PowerOn : BOOL;
		ReadyForPowerOn : BOOL;
		Simulation : BOOL;
	END_STRUCT;
END_TYPE

(*Reference*)

TYPE
	AxisReference_Int_typ : 	STRUCT 
		clearReference : BOOL;
		needToClearReference : BOOL;
		oldCommunicationReady : BOOL;
		oldStartupCount : UDINT;
		reference : BOOL;
		restorePos : McAcpAxRestorePosType;
		state : AXISLIB_REFST_enum;
		CheckRestorePos : MC_BR_CheckRestorePositionData;
		InitHome : MC_BR_InitHome_AcpAx;
		Home : MC_Home;
		Status : AxisStatus;
	END_STRUCT;
	AXISLIB_REFST_enum : 
		(
		AXISLIB_REFST_START,
		AXISLIB_REFST_START_CHECK,
		AXISLIB_REFST_START_INIT,
		AXISLIB_REFST_START_HOME,
		AXISLIB_REFST_IDLE,
		AXISLIB_REFST_REF_INIT,
		AXISLIB_REFST_REF_HOME,
		AXISLIB_REFST_CLEAR_INIT
		);
END_TYPE

(*Basic axis*)

TYPE
	AxisBasic_typ : 	STRUCT  (*Axis manager data type.*)
		pAxisObject : REFERENCE TO McAxisType; (*Pointer to the axis object (global variable from mapp configuration).*)
		pRestorePosition : UDINT; (*Address of a permanent variable to use the InitEndlessPosition and mcHOMING_RESTORE_POSITION homing method. If this value is non-zero, the position will be restored on startup.*)
		IN : AxisBasic_IN_typ; (*Axis manager inputs (read/write).*)
		OUT : AxisBasic_OUT_typ; (*Axis manager outputs (read only).*)
		TEST : AxisBasic_TEST_typ; (*Adds functionality to test axis*)
		Internal : AxisBasic_Internal_typ; (*Internal axis manager variables (not intended to be read or written).*)
	END_STRUCT;
	AxisBasic_IN_typ : 	STRUCT  (*Axis manager inputs (read/write).*)
		CMD : AxisBasic_IN_CMD_typ; (*Input commands.*)
		PAR : AxisBasic_IN_PAR_typ; (*Input parameters.*)
		CFG : AxisBasic_IN_CFG_typ; (*Configuration inputs.  These are meant to be written only once.*)
	END_STRUCT;
	AxisBasic_IN_CMD_typ : 	STRUCT  (*Input commands.*)
		Power : BOOL; (*Enable power on the axis.*)
		Reference : BOOL; (*Reference the axis.*)
		MoveAbsolute : BOOL; (*Execute an absolute move.*)
		MoveAdditive : BOOL; (*Execute an additive move.*)
		MoveVelocity : BOOL; (*Execute a velocity move.*)
		JogForward : BOOL; (*Jog the axis forward.*)
		JogReverse : BOOL; (*Jog the axis reverse.*)
		InhibitMotion : BOOL; (*Not Implemented*)
		Halt : BOOL; (*Halt the axis. This command can be interrupted.*)
		Stop : BOOL; (*Stop the axis. This command CANNOT be interrupted.*)
		ClearReference : BOOL;
		ErrorReset : BOOL;
		WaitToInitializeReference : BOOL;
	END_STRUCT;
	AxisBasic_IN_PAR_typ : 	STRUCT  (*Input parameters.*)
		Position : LREAL := 0; (*Target position for absolute moves.*)
		Distance : REAL := 0; (*Distance for additive moves.*)
		Velocity : REAL := 0; (*Velocity for basic moves (not including jog moves).*)
		Acceleration : REAL := 0; (*Acceleration for basic moves (not including jog moves).*)
		Deceleration : REAL := 0; (*Deceleration for basic moves (not including jog moves and stopping).*)
		Direction : McDirectionEnum := mcDIR_POSITIVE; (*Direction for basic moves (not including jog moves).*)
		JogVelocity : REAL := 0; (*Velocity for jog moves.*)
		JogAcceleration : REAL := 0; (*Acceleration for jog moves.*)
		JogDeceleration : REAL := 0; (*Deceleration for jog moves.*)
	END_STRUCT;
	AxisBasic_IN_CFG_typ : 	STRUCT  (*Configuration inputs.  These are meant to be written only once.*)
		Name : STRING[AXLIB_STRLEN_NAME];
		Active : BOOL;
		HomingPosition : LREAL := 0.0; (*Homing position.*)
		HomingMode : McHomingModeEnum := mcHOMING_DIRECT; (*Homing mode.*)
		DefaultPosition : LREAL;
		StopDeceleration : REAL := 10000.0; (*Deceleration for stopping.*)
	END_STRUCT;
	AxisBasic_OUT_typ : 	STRUCT  (*Axis manager outputs (read only).*)
		Active : BOOL;
		MotionInhibited : BOOL; (*Not Implemented*)
		ActualPosition : LREAL; (*Actual position of the axis [Units].*)
		ActualVelocity : REAL; (*Actual velocity of the axis [Units/s].*)
		AxisInfo : AxisLib_AxisInfo_typ;
		PLCOpenState : McAxisPLCopenStateEnum; (*PLCOpen state information.*)
		Referenced : BOOL; (*The axis has been properly referenced. This is set and reset by the application.*)
		RestorePositionInitialized : BOOL; (*The endless position data has been initialized for the axis.*)
		DataValid : BOOL; (*The endless position data is valid.*)
		Done : BOOL; (*Axis is done with the current operation.*)
		Busy : BOOL; (*Axis is currently performing an operation.*)
		Warning : BOOL; (*Axis warning is present.*)
		Error : BOOL; (*Axis error is present.*)
		ErrorCount : UINT; (*Number of errors present on the axis.*)
		ErrorID : DINT; (*Error ID value for the axis.*)
	END_STRUCT;
	AxisBasic_TEST_typ : 	STRUCT 
		Enable : BOOL; (*Enable Test mode (Use Values from test CMD's and Pars)*)
		CMD : AxisBasic_IN_CMD_typ;
		PAR : AxisBasic_IN_PAR_typ;
		STAT : AxisBasic_TEST_STAT_typ;
	END_STRUCT;
	AxisBasic_TEST_STAT_typ : 	STRUCT  (*Axis status information.*)
		Busy : BOOL; (*Axis is currently performing an operation.*)
		Done : BOOL; (*Axis is done with the current operation.*)
		Warning : BOOL; (*Axis warning is present.*)
		Error : BOOL; (*Axis error is present.*)
		ErrorCount : UINT; (*Number of errors present on the axis.*)
		ErrorID : UINT; (*Error ID value for the axis.*)
		ActualPosition : LREAL; (*Actual position of the axis [Units].*)
		ActualVelocity : REAL; (*Actual velocity of the axis [Units/s].*)
		CommunicationReady : BOOL; (*shows that the network is initialized*)
		PowerOn : BOOL; (*shows that the controller is turned on*)
		IsHomed : BOOL; (*shows that the axis is referenced*)
	END_STRUCT;
	AxisBasic_Internal_typ : 	STRUCT  (*Internal axis manager variables (not intended to be read or written).*)
		FUB : AxisBasic_Int_FUB_typ;
		ResetOK : BOOL; (*It is OK to call MC_Reset. Necessary because of timing between Errorstop state and error reporting.*)
	END_STRUCT;
	AxisBasic_Int_FUB_typ : 	STRUCT 
		Status : AxisStatus;
		Power : MC_Power;
		Reference : AxisReference;
		Home : MC_Home;
		MoveAbsolute : MC_MoveAbsolute;
		MoveAdditive : MC_MoveAdditive;
		MoveVelocity : MC_MoveVelocity;
		Jog : MC_BR_JogVelocity;
		Halt : MC_Halt;
		Stop : MC_Stop;
		ReadAxisError : MC_ReadAxisError;
		Reset : MC_Reset;
	END_STRUCT;
END_TYPE
