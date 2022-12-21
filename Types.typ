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
		ReadAxisLimits : MC_BR_ProcessParam;
		ReadActualPosition : MC_ReadActualPosition;
		ReadActualVelocity : MC_ReadActualVelocity;
		ReadAxisInfo : MC_ReadAxisInfo;
		ReadLibraryInfo : MC_BR_GetAxisLibraryInfo;
	END_STRUCT;
	AxisLib_AxisInfo_typ : 	STRUCT 
		libraryInfo : STRING[32];
		AdditionalInfo : McAddInfoType;
		AxisLimits : McCfgAxMoveLimType;
	END_STRUCT;
	AxisLib_AxisState_typ : 	STRUCT  (*PLCOpen state information.*)
		Simulation : BOOL;
		CommunicationReady : BOOL;
		IsHomed : BOOL;
		Referenced : BOOL; (*The axis has been properly referenced. This is set and reset by the application.*)
		ReadyForPowerOn : BOOL;
		PowerOn : BOOL;
		HomeDataValid : BOOL; (*The endless position data is valid.*)
		MotionInhibited : BOOL; (*Not Implemented*)
		AxisWarning : BOOL;
	END_STRUCT;
	AxisLib_PLCOpenState_typ : 	STRUCT  (*PLCOpen state information.*)
		Errorstop : BOOL; (*If set, axis is in state Errorstop.*)
		Disabled : BOOL; (*If set, axis is in state Disabled.*)
		StandStill : BOOL; (*If set, axis is in state StandStill.*)
		Stopping : BOOL; (*If set, axis is in state Stopping.*)
		Homing : BOOL; (*If set, axis is in state Homing.*)
		DiscreteMotion : BOOL; (*If set, axis is in state DiscreteMotion.*)
		SynchronizedMotion : BOOL; (*If set, axis is in state SynchronizedMotion.*)
		ContinuousMotion : BOOL; (*If set, axis is in state ContinousMotion.*)
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
		initHomeSupported : BOOL;
	END_STRUCT;
	AXISLIB_REFST_enum : 
		(
		AXISLIB_REFST_START,
		AXISLIB_REFST_START_CHECK,
		AXISLIB_REFST_START_HOME,
		AXISLIB_REFST_IDLE,
		AXISLIB_REFST_REF_INIT,
		AXISLIB_REFST_REF_HOME
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
		MoveAdditiveForward : BOOL; (*Execute an additive move in forward direction.*)
		MoveAdditiveReverse : BOOL; (*Execute an additive move in reverse direction.*)
		MoveVelocity : BOOL; (*Execute a velocity move.*)
		MoveWaypoint : BOOL;
		JogForward : BOOL; (*Jog the axis forward.*)
		JogReverse : BOOL; (*Jog the axis reverse.*)
		InhibitMotion : BOOL; (*Not Implemented*)
		Halt : BOOL; (*Halt the axis. This command can be interrupted.*)
		Stop : BOOL; (*Stop the axis. This command CANNOT be interrupted.*)
		ClearReference : BOOL;
		ErrorReset : BOOL;
		WaitToInitializeReference : BOOL;
		SaveParameters : BOOL;
		LoadParameters : BOOL;
		SaveWaypoints : BOOL;
		LoadWaypoints : BOOL;
		EnableSwEndLimits : BOOL;
		DisableSwEndLimits : BOOL;
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
		JogJerk : REAL := 0; (*Jerk for jog moves*)
		WaypointIndex : USINT;
		pInitHomingData : UDINT;
		szInitHomingData : UDINT;
	END_STRUCT;
	AxisBasic_IN_CFG_typ : 	STRUCT  (*Configuration inputs.  These are meant to be written only once.*)
		Name : STRING[AXLIB_STRLEN_NAME];
		Active : BOOL;
		HomingPosition : LREAL := 0.0; (*Homing position.*)
		HomingMode : McHomingModeEnum;
		DefaultPosition : LREAL;
		StopDeceleration : REAL := 10000.0; (*Deceleration for stopping.*)
		DisableLagRead : BOOL;
	END_STRUCT;
	AxisBasic_OUT_typ : 	STRUCT  (*Axis manager outputs (read only).*)
		Name : STRING[AXLIB_STRLEN_NAME];
		Active : BOOL;
		Busy : BOOL; (*Axis is currently performing an operation.*)
		Done : BOOL; (*Axis is done with the current operation.*)
		Error : BOOL; (*Axis error is present.*)
		ErrorID : DINT; (*Error ID value for the axis.*)
		ErrorCount : UINT; (*Number of errors present on the axis.*)
		Warning : BOOL; (*Axis warning is present.*)
		Position : LREAL; (*Actual position of the axis [Units].*)
		Velocity : REAL; (*Actual velocity of the axis [Units/s].*)
		Lag : REAL;
		State : AxisLib_AxisState_typ;
		PLCOpen : McAxisPLCopenStateEnum; (*Extended PLCopen state*)
		PLCOpenDiscrete : AxisLib_PLCOpenState_typ;
		Info : AxisLib_AxisInfo_typ;
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
		FubError : BOOL;
		CyclicReadPar : McAcpAxCycParIDType;
	END_STRUCT;
	AxisBasic_Int_FUB_typ : 	STRUCT 
		Status : AxisStatus;
		Power : MC_Power;
		Reference : AxisReference;
		Home : MC_Home;
		MoveAbsolute : MC_MoveAbsolute;
		MoveWaypoint : MC_MoveAbsolute;
		MoveAdditive : MC_MoveAdditive;
		MoveVelocity : MC_MoveVelocity;
		Jog : MC_BR_JogLimitPosition;
		Halt : MC_Halt;
		Stop : MC_Stop;
		ReadAxisError : MC_ReadAxisError;
		Reset : MC_Reset;
		WriteParameter : MC_WriteParameter;
		CyclicRead : MC_BR_CyclicProcessParID_AcpAx;
	END_STRUCT;
	AxisBasic_Api_typ : 	STRUCT 
		pAxisObject : REFERENCE TO McAxisType; (*Pointer to the axis object (global variable from mapp configuration).*)
		pRestorePosition : UDINT; (*Address of a permanent variable to use the InitEndlessPosition and mcHOMING_RESTORE_POSITION homing method. If this value is non-zero, the position will be restored on startup.*)
		IN : AxisBasic_ApiIn_typ;
		OUT : AxisBasic_OUT_typ;
	END_STRUCT;
	AxisBasic_ApiIn_typ : 	STRUCT 
		CMD : AxisBasic_IN_CMD_typ; (*Input commands.*)
		PAR : AxisBasic_IN_PAR_typ; (*Input parameters.*)
		Waypoint : ARRAY[0..WAYPOINT_MAI]OF AxisBasic_WayPoint_typ;
	END_STRUCT;
	AxisBasic_WayPoint_typ : 	STRUCT 
		Name : STRING[AXLIB_STRLEN_NAME];
		Position : LREAL;
		Velocity : REAL;
		Acceleration : REAL;
		Deceleration : REAL;
		Jerk : REAL;
		Direction : McDirectionEnum := mcDIR_POSITIVE; (*Direction for basic moves (not including jog moves).*)
	END_STRUCT;
END_TYPE
