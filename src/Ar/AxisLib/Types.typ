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
		ReadDriveStatus : MC_BR_ReadDriveStatus;
		ReadActualPosition : MC_ReadActualPosition;
		ReadCyclicPosition : MC_BR_ReadCyclicPosition;
		ReadActualVelocity : MC_ReadActualVelocity;
		ReadStatus : MC_ReadStatus;
	END_STRUCT;
	AxisLib_PLCOpenState_typ : 	STRUCT  (*PLCOpen state information.*)
		State : AXISLIB_PLCOPEN_ST_enum;
		Errorstop : BOOL; (*If set, axis is in state Errorstop.*)
		Disabled : BOOL; (*If set, axis is in state Disabled.*)
		StandStill : BOOL; (*If set, axis is in state StandStill.*)
		Stopping : BOOL; (*If set, axis is in state Stopping.*)
		Homing : BOOL; (*If set, axis is in state Homing.*)
		DiscreteMotion : BOOL; (*If set, axis is in state DiscreteMotion.*)
		SynchronizedMotion : BOOL; (*If set, axis is in state SynchronizedMotion.*)
		ContinuousMotion : BOOL; (*If set, axis is in state ContinousMotion.*)
	END_STRUCT;
	AXISLIB_PLCOPEN_ST_enum : 
		(
		AXISLIB_PLCOPEN_ST_INVALID := 0,
		AXISLIB_PLCOPEN_ST_ERRORSTOP,
		AXISLIB_PLCOPEN_ST_DISABLED,
		AXISLIB_PLCOPEN_ST_STANDSTILL,
		AXISLIB_PLCOPEN_ST_STOPPING,
		AXISLIB_PLCOPEN_ST_HOMING,
		AXISLIB_PLCOPEN_ST_DISCRETEMOTN,
		AXISLIB_PLCOPEN_ST_SYNCMOTN,
		AXISLIB_PLCOPEN_ST_CONTMOTN
		);
END_TYPE

(*Reference*)

TYPE
	AxisReference_Int_typ : 	STRUCT 
		state : AXISLIB_REFST_enum;
		driveStatus : MC_DRIVESTATUS_TYP;
		endlessPosition : MC_ENDLESS_POSITION;
		Reference : BOOL;
		ClearReference : BOOL;
		needToClearReference : BOOL;
		oldNetworkInit : BOOL;
		initHomeParam : ACP10HOMPA_typ;
		readDriveStatus : MC_BR_ReadDriveStatus;
		readStatus : MC_ReadStatus;
		checkEndlessPos : MC_BR_CheckEndlessPosition;
		initEndlessPos : MC_BR_InitEndlessPosition;
		home : MC_Home;
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
		pAxisObject : REFERENCE TO ACP10AXIS_typ; (*Pointer to the axis object (global variable from nc mapping file).*)
		pEndlessPosition : UDINT; (*Address of a permanent variable to use the InitEndlessPosition and mcHOME_RESTORE_POS homing method. If this value is non-zero, the position will be restored on startup.*)
		ErrorTextModuleName : STRING[12] := 'acp10etxen'; (*Name of the acp10 error text module to be used for the drive.*)
		IN : AxisBasic_IN_typ; (*Axis manager inputs (read/write).*)
		IO : AxisBasic_IO_typ; (*IO to map to generic IO. To use this, the IO must have "FORCED" in the axis configuration*)
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
		MoveCyclicPosition : BOOL; (*Track a cyclic set position.*)
		JogForward : BOOL; (*Jog the axis forward.*)
		JogReverse : BOOL; (*Jog the axis reverse.*)
		Halt : BOOL; (*Halt the axis. This command can be interrupted.*)
		Stop : BOOL; (*Stop the axis. This command CANNOT be interrupted.*)
		ClearReference : BOOL;
		AcknowledgeError : BOOL; (*Acknowledge errors on the axis.*)
		Reset : BOOL;
		WaitToInitializeReference : BOOL;
	END_STRUCT;
	AxisBasic_IN_PAR_typ : 	STRUCT  (*Input parameters.*)
		Position : REAL := 1000.0; (*Target position for absolute moves.*)
		Distance : REAL := 1000.0; (*Distance for additive moves.*)
		CyclicPosition : MC_CYCLIC_POSITION := (Integer:=0,Real:=0); (*Cyclic position input for tracking set position.*)
		Velocity : REAL := 1000.0; (*Velocity for basic moves (not including jog moves).*)
		Acceleration : REAL := 10000.0; (*Acceleration for basic moves (not including jog moves).*)
		Deceleration : REAL := 10000.0; (*Deceleration for basic moves (not including jog moves and stopping).*)
		Direction : USINT := mcPOSITIVE_DIR; (*Direction for basic moves (not including jog moves).*)
		JogVelocity : REAL := 100.0; (*Velocity for jog moves.*)
		JogAcceleration : REAL := 10000.0; (*Acceleration for jog moves.*)
		JogDeceleration : REAL := 10000.0; (*Deceleration for jog moves.*)
	END_STRUCT;
	AxisBasic_IN_CFG_typ : 	STRUCT  (*Configuration inputs.  These are meant to be written only once.*)
		Name : STRING[AXLIB_STRLEN_NAME];
		Active : BOOL;
		HomingPosition : REAL := 0.0; (*Homing position.*)
		HomingMode : USINT := mcHOME_DEFAULT; (*Homing mode.*)
		DefaultPosition : REAL;
		StopDeceleration : REAL := 10000.0; (*Deceleration for stopping.*)
		ReadCyclicPositionParID : UINT; (*ParID to use for reading the cyclic position. 0 implies SGEN_S_SET.*)
		Factor : UDINT; (*PLCOpen Scale factor*)
		Period : UDINT; (*Axis period*)
	END_STRUCT;
	AxisBasic_IO_typ : 	STRUCT  (*Axis IO, to use these you must add FORCE to the axis init table IO configuration*)
		diHomeSwitch : BOOL;
		diNegHWSwitch : BOOL;
		diPosHWSwitch : BOOL;
		diTrigger1 : BOOL;
		diTrigger2 : BOOL;
	END_STRUCT;
	AxisBasic_OUT_typ : 	STRUCT  (*Axis manager outputs (read only).*)
		Active : BOOL;
		ActualPosition : REAL; (*Actual position of the axis [Units].*)
		ActualPositionPrecise : LREAL; (*Actual position of the axis [Units].*)
		ActualCyclicPosition : MC_CYCLIC_POSITION; (*Actual high resolution position of the axis [Units]*)
		ActualVelocity : REAL; (*Actual velocity of the axis [Units/s].*)
		DriveStatus : MC_DRIVESTATUS_TYP; (*Drive status information.*)
		PLCOpenState : AxisLib_PLCOpenState_typ; (*PLCOpen state information.*)
		Referenced : BOOL; (*The axis has been properly referenced. This is set and reset by the application.*)
		EndlessPositionInitialized : BOOL; (*The endless position data has been initialized for the axis.*)
		DataValid : BOOL; (*The endless position data is valid.*)
		Done : BOOL; (*Axis is done with the current operation.*)
		Busy : BOOL; (*Axis is currently performing an operation.*)
		Warning : BOOL; (*Axis warning is present.*)
		WarningCount : UINT; (*Number of warnings present on the axis.*)
		Error : BOOL; (*Axis error is present.*)
		ErrorCount : UINT; (*Number of errors present on the axis.*)
		ErrorID : UINT; (*Error ID value for the axis.*)
		ErrorString : STRING[AXLIB_STRLEN_ERROR]; (*Error description string.*)
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
		WarningCount : UINT; (*Number of warnings present on the axis.*)
		Error : BOOL; (*Axis error is present.*)
		ErrorCount : UINT; (*Number of errors present on the axis.*)
		ErrorID : UINT; (*Error ID value for the axis.*)
		ActualPosition : REAL; (*Actual position of the axis [Units].*)
		ActualVelocity : REAL; (*Actual velocity of the axis [Units/s].*)
		NetworkInit : BOOL; (*shows that the network is initialized*)
		ControllerStatus : BOOL; (*shows that the controller is turned on*)
		HomingOk : BOOL; (*shows that the axis is referenced*)
	END_STRUCT;
	AxisBasic_Internal_typ : 	STRUCT  (*Internal axis manager variables (not intended to be read or written).*)
		FUB : AxisBasic_Int_FUB_typ;
		ErrorString : ARRAY[0..3]OF STRING[79]; (*Error description string.*)
		ResetOK : BOOL; (*It is OK to call MC_Reset. Necessary because of timing between Errorstop state and error reporting.*)
		useFastFunction : BOOL; (*AxisBasicFn_Fast is being used for cyclic positions*)
	END_STRUCT;
	AxisBasic_Int_FUB_typ : 	STRUCT 
		Status : AxisStatus;
		Power : MC_Power;
		SetHardwareInputs : {REDUND_UNREPLICABLE} MC_BR_SetHardwareInputs;
		Reference : AxisReference;
		Home : MC_Home;
		MoveAbsolute : MC_MoveAbsolute;
		MoveAdditive : MC_MoveAdditive;
		MoveVelocity : MC_MoveVelocity;
		Jog : MC_BR_JogVelocity;
		MoveCyclicPosition : MC_BR_MoveCyclicPosition;
		Halt : MC_Halt;
		Stop : MC_Stop;
		ReadAxisError : MC_BR_ReadAxisError;
		Reset : MC_Reset;
		InitModPos : MC_BR_InitModPos;
	END_STRUCT;
	AxisExpandLimit_Internal_typ : 	STRUCT 
		_MC_BR_InitAxisSubjectPar : MC_BR_InitAxisSubjectPar;
		ApplicationLimits : AxisApplicationLimits_typ;
		ExpandedLimits : AxisApplicationLimits_typ;
		_AcknowledgeError : BOOL;
		State : USINT;
	END_STRUCT;
	AxisApplicationLimits_typ : {REDUND_UNREPLICABLE} 	STRUCT 
		pos_sw_end : {REDUND_UNREPLICABLE} REAL;
		neg_sw_end : {REDUND_UNREPLICABLE} REAL;
	END_STRUCT;
END_TYPE

(*DEPRECATED: Restore Position*)

TYPE
	AxisRestorePosition_Int_typ : 	STRUCT 
		State : AXISLIB_RPST_enum;
		InitEndlessPos : MC_BR_InitEndlessPosition;
		CheckEndlessPos : MC_BR_CheckEndlessPosition;
		Home : MC_Home;
	END_STRUCT;
	AXISLIB_RPST_enum : 
		(
		AXISLIB_RPST_IDLE,
		AXISLIB_RPST_INIT,
		AXISLIB_RPST_CHECK,
		AXISLIB_RPST_HOME,
		AXISLIB_RPST_DONE
		);
END_TYPE