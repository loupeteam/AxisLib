//--------------------------------------------------------------------------------
// Copyright: Loupe
// Author:    Josh Polansky
// Created:   September 28, 2022
//--------------------------------------------------------------------------------

#include <bur/plctypes.h>
#ifdef __cplusplus
extern "C"
{
#endif

#include "AxisLib.h"
#include <string.h>

#ifdef __cplusplus
};
#endif

//---------------------------------------------------------
// Manage an axis for basic movements and status reporting
//---------------------------------------------------------


plcbit AxisBasicCyclic(struct AxisBasic_Api_typ* Api, struct AxisBasic_IN_CFG_typ* configuration, struct AxisBasic_Internal_typ* internal){
	
	if (Api == 0) return 0;
	
	if (configuration->Active == 0) {
		memset(&Api->OUT, 0, sizeof(Api->OUT));
		Api->OUT.Active = 0;
		return 0;
	}
	
	// Check for 0 axis
	if (Api->pAxisObject == 0) {
		
		memset(&Api->OUT, 0, sizeof(Api->OUT));
		
		// TODO: Should we set OUT.Active = IN.CFG.Active here?
		// Not right now.
		Api->OUT.Error = 1;
		Api->OUT.ErrorCount = 1;
		Api->OUT.ErrorID = 29200;
		
		return 0;
	
	}
		
	// Active
	Api->OUT.Active = configuration->Active;
	strcpy( Api->OUT.Name, configuration->Name );
	
	
	// Axis Status
	internal->FUB.Status.Axis = Api->pAxisObject;
	internal->FUB.Status.Enable = !internal->FUB.Status.Error;
	AxisStatus(&internal->FUB.Status);
	memcpy(&Api->OUT.Info, &internal->FUB.Status.Info, sizeof(internal->FUB.Status.Info));
	Api->OUT.Position = internal->FUB.Status.Position;
	Api->OUT.Velocity = internal->FUB.Status.Velocity;
	
	
	// Power
	internal->FUB.Power.Axis = Api->pAxisObject;
	internal->FUB.Power.Enable = Api->IN.CMD.Power; // 'dumb Power' input
	internal->FUB.Power.Enable = Api->IN.CMD.Power && internal->FUB.Status.ReadyForPowerOn && internal->FUB.Status.CommunicationReady; // TODO: 'smart shouldPower' input - Don't use Errorstop based on PLCOpen state diagram
	if(internal->FUB.Power.Enable || internal->FUB.Power.Busy || internal->FUB.Power.Error){
		MC_Power(&internal->FUB.Power);
	}
	
	
	// Reference									
	internal->FUB.Reference.Axis = Api->pAxisObject;
	internal->FUB.Reference.RestorePositionVariableAddress = Api->pRestorePosition;
	internal->FUB.Reference.DefaultPosition = configuration->DefaultPosition;
	internal->FUB.Reference.WaitToInitialize = Api->IN.CMD.WaitToInitializeReference;
	internal->FUB.Reference.Reference = Api->IN.CMD.Reference;
	internal->FUB.Reference.Position = configuration->HomingPosition;
	internal->FUB.Reference.HomingMode = configuration->HomingMode;
	internal->FUB.Reference.pInitHomingData = Api->IN.PAR.pInitHomingData;
	internal->FUB.Reference.szInitHomingData = Api->IN.PAR.szInitHomingData;
	internal->FUB.Reference.ClearReference = Api->IN.CMD.ClearReference;
	strcpy( &internal->FUB.Reference.Library, &internal->FUB.Status.Info.libraryInfo );
	AxisReference(&internal->FUB.Reference);
	
	Api->OUT.State.Referenced = internal->FUB.Reference.Referenced;
	Api->OUT.State.HomeDataValid = internal->FUB.Reference.DataValid;


	// Move Absolute 
	internal->FUB.MoveAbsolute.Axis = Api->pAxisObject;
	internal->FUB.MoveAbsolute.Execute = Api->IN.CMD.MoveAbsolute;
	internal->FUB.MoveAbsolute.Position = Api->IN.PAR.Position;
	internal->FUB.MoveAbsolute.Velocity = Api->IN.PAR.Velocity;
	internal->FUB.MoveAbsolute.Acceleration = Api->IN.PAR.Acceleration;
	internal->FUB.MoveAbsolute.Deceleration = Api->IN.PAR.Deceleration;
	internal->FUB.MoveAbsolute.Direction = Api->IN.PAR.Direction;

	MC_MoveAbsolute(&internal->FUB.MoveAbsolute);

	// Move Absolute 
	internal->FUB.MoveWaypoint.Axis = 			Api->pAxisObject;
	internal->FUB.MoveWaypoint.Execute = 		Api->IN.CMD.MoveWaypoint;
	internal->FUB.MoveWaypoint.Position = 		Api->IN.Waypoint[Api->IN.PAR.WaypointIndex].Position;
	internal->FUB.MoveWaypoint.Velocity = 		Api->IN.Waypoint[Api->IN.PAR.WaypointIndex].Velocity;
	internal->FUB.MoveWaypoint.Acceleration = 	Api->IN.Waypoint[Api->IN.PAR.WaypointIndex].Acceleration;
	internal->FUB.MoveWaypoint.Deceleration = 	Api->IN.Waypoint[Api->IN.PAR.WaypointIndex].Deceleration;
	internal->FUB.MoveWaypoint.Direction = 		Api->IN.Waypoint[Api->IN.PAR.WaypointIndex].Direction;

	MC_MoveAbsolute(&internal->FUB.MoveWaypoint);

	// Move Additive Forward
	internal->FUB.MoveAdditive.Axis = Api->pAxisObject;
	internal->FUB.MoveAdditive.Execute = (Api->IN.CMD.MoveAdditiveForward || Api->IN.CMD.MoveAdditiveReverse);
	internal->FUB.MoveAdditive.Distance = (Api->IN.CMD.MoveAdditiveForward ? Api->IN.PAR.Distance : (-1) * Api->IN.PAR.Distance);
	internal->FUB.MoveAdditive.Velocity = Api->IN.PAR.Velocity;
	internal->FUB.MoveAdditive.Acceleration = Api->IN.PAR.Acceleration;
	internal->FUB.MoveAdditive.Deceleration = Api->IN.PAR.Deceleration;

	MC_MoveAdditive(&internal->FUB.MoveAdditive);


	// Move Velocity 
	internal->FUB.MoveVelocity.Axis = Api->pAxisObject;
	internal->FUB.MoveVelocity.Execute = Api->IN.CMD.MoveVelocity;
	internal->FUB.MoveVelocity.Velocity = Api->IN.PAR.Velocity;
	internal->FUB.MoveVelocity.Acceleration = Api->IN.PAR.Acceleration;
	internal->FUB.MoveVelocity.Deceleration = Api->IN.PAR.Deceleration;
	internal->FUB.MoveVelocity.Direction = Api->IN.PAR.Direction;

	MC_MoveVelocity(&internal->FUB.MoveVelocity);


	// Jog with limited position	
	internal->FUB.Jog.Axis = Api->pAxisObject;
	internal->FUB.Jog.Enable = Api->IN.CMD.JogForward || Api->IN.CMD.JogReverse || internal->FUB.Jog.Jogging;
	internal->FUB.Jog.Velocity = Api->IN.PAR.JogVelocity;
	internal->FUB.Jog.Acceleration = Api->IN.PAR.JogAcceleration;
	internal->FUB.Jog.Deceleration = Api->IN.PAR.JogDeceleration;
	internal->FUB.Jog.Jerk = Api->IN.PAR.JogJerk;
	internal->FUB.Jog.JogPositive = Api->IN.CMD.JogForward;
	internal->FUB.Jog.JogNegative = Api->IN.CMD.JogReverse;
	internal->FUB.Jog.FirstPosition = internal->FUB.Status.Info.AxisLimits.MovementLimits.Internal.Position.LowerLimit;
	internal->FUB.Jog.LastPosition = internal->FUB.Status.Info.AxisLimits.MovementLimits.Internal.Position.UpperLimit;

	MC_BR_JogLimitPosition(&internal->FUB.Jog);


	// Halt												
	internal->FUB.Halt.Axis = Api->pAxisObject;
	internal->FUB.Halt.Execute = (Api->IN.CMD.Halt		
		|| (internal->FUB.MoveAdditive.Busy && !internal->FUB.MoveAdditive.Execute)
		|| (internal->FUB.MoveAbsolute.Busy && !internal->FUB.MoveAbsolute.Execute)		
		|| (internal->FUB.MoveVelocity.Busy && !internal->FUB.MoveVelocity.Execute)		
		|| (internal->FUB.MoveWaypoint.Busy && !internal->FUB.MoveWaypoint.Execute))
		&& !internal->FUB.Halt.Busy;
	internal->FUB.Halt.Deceleration = configuration->StopDeceleration;

	MC_Halt(&internal->FUB.Halt);


	// Stop												
	internal->FUB.Stop.Axis = Api->pAxisObject;
	internal->FUB.Stop.Execute = Api->IN.CMD.Stop;
	internal->FUB.Stop.Deceleration = configuration->StopDeceleration;

	MC_Stop(&internal->FUB.Stop);
	
	
	// Enable/disable SW limits. 
	internal->FUB.WriteParameter.Axis = Api->pAxisObject;
	internal->FUB.WriteParameter.Execute = (Api->IN.CMD.EnableSwEndLimits || Api->IN.CMD.DisableSwEndLimits);
	internal->FUB.WriteParameter.ParameterNumber = mcPAR_SW_END_IGNORE;
	internal->FUB.WriteParameter.Value = (Api->IN.CMD.DisableSwEndLimits ? 1 : 0);

	MC_WriteParameter(&internal->FUB.WriteParameter);
	

	// Busy 
	Api->OUT.Busy = internal->FUB.Reference.Busy
		||	internal->FUB.MoveAbsolute.Busy
		||	internal->FUB.MoveAdditive.Busy
		||	(internal->FUB.MoveVelocity.Busy && internal->FUB.MoveVelocity.Execute && !(internal->FUB.MoveVelocity.InVelocity))
		||	internal->FUB.Jog.Busy
		||	internal->FUB.Halt.Busy
		||	internal->FUB.Stop.Busy
		||	internal->FUB.Reset.Busy
		||  internal->FUB.WriteParameter.Busy;
					

	// Done 
	Api->OUT.Done = internal->FUB.Reference.Done
		||	internal->FUB.MoveAbsolute.Done
		||	internal->FUB.MoveAdditive.Done
		||	internal->FUB.MoveVelocity.InVelocity
		||	internal->FUB.Halt.Done
		||	internal->FUB.Stop.Done
		||	internal->FUB.Reset.Done
		||  internal->FUB.WriteParameter.Done;
					


	//	Api->OUT.State.MotionInhibited = internal->FUB.Status.AxisWarning;
	Api->OUT.State.AxisWarning = internal->FUB.Status.AxisWarning;
	Api->OUT.State.CommunicationReady = internal->FUB.Status.CommunicationReady;
	Api->OUT.State.IsHomed = internal->FUB.Status.IsHomed;
	Api->OUT.State.AxisWarning = internal->FUB.Status.AxisWarning;
	Api->OUT.State.PowerOn = internal->FUB.Status.PowerOn;
	Api->OUT.State.ReadyForPowerOn = internal->FUB.Status.ReadyForPowerOn;
	Api->OUT.State.Simulation = internal->FUB.Status.Simulation;

	Api->OUT.State.Referenced = internal->FUB.Reference.Referenced;
	Api->OUT.State.HomeDataValid = internal->FUB.Reference.DataValid;
	
	Api->OUT.PLCOpen = internal->FUB.Status.Info.AdditionalInfo.PLCopenState;
	Api->OUT.PLCOpenDiscrete.ContinuousMotion = Api->OUT.PLCOpen == mcAXIS_CONTINUOUS_MOTION;
	Api->OUT.PLCOpenDiscrete.Disabled = Api->OUT.PLCOpen == mcAXIS_DISABLED;
	Api->OUT.PLCOpenDiscrete.DiscreteMotion = Api->OUT.PLCOpen == mcAXIS_DISCRETE_MOTION;
	Api->OUT.PLCOpenDiscrete.Errorstop = Api->OUT.PLCOpen == mcAXIS_ERRORSTOP;
	Api->OUT.PLCOpenDiscrete.Homing = Api->OUT.PLCOpen == mcAXIS_HOMING;
	Api->OUT.PLCOpenDiscrete.StandStill = Api->OUT.PLCOpen == mcAXIS_STANDSTILL;
	Api->OUT.PLCOpenDiscrete.Stopping = Api->OUT.PLCOpen == mcAXIS_STOPPING;
	Api->OUT.PLCOpenDiscrete.SynchronizedMotion = Api->OUT.PLCOpen == mcAXIS_SYNCHRONIZED_MOTION;
	
	// Get axis error information
	internal->FUB.ReadAxisError.Axis = Api->pAxisObject;
	internal->FUB.ReadAxisError.Enable = !(internal->FUB.ReadAxisError.Error);
	Api->OUT.ErrorCount = 0;
	
	do {
		// read all axis errors
		internal->FUB.ReadAxisError.ReadNext = 1;
		MC_ReadAxisError(&internal->FUB.ReadAxisError);
	
		if (internal->FUB.ReadAxisError.Valid && internal->FUB.ReadAxisError.AxisErrorID) {
			Api->OUT.ErrorID = internal->FUB.ReadAxisError.RecordID;
			Api->OUT.ErrorCount++;	
		}
	
		// reset for positive edge
		internal->FUB.ReadAxisError.ReadNext = 0;
		MC_ReadAxisError(&internal->FUB.ReadAxisError);
	} while (internal->FUB.ReadAxisError.AxisErrorID && !internal->FUB.ReadAxisError.Error);

	if (internal->FUB.ReadAxisError.Error) {
		Api->OUT.ErrorID = internal->FUB.ReadAxisError.ErrorID;
		Api->OUT.ErrorCount = 1;
	}


	// Set Error and Warning
	if (Api->OUT.ErrorCount != 0) {
		Api->OUT.Error = 1;
	} else {
		Api->OUT.Error = 0;
	}
	
	Api->OUT.Warning = internal->FUB.Status.AxisWarning;

	
	// Reset
	internal->FUB.Reset.Axis = Api->pAxisObject;
	internal->FUB.Reset.Execute = Api->IN.CMD.ErrorReset;

	MC_Reset(&internal->FUB.Reset);
	

	// Reset Reset
	Api->IN.CMD.ErrorReset = 0;

	return 0;
	
}
