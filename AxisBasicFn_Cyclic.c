//--------------------------------------------------------------------------------
// Copyright: Automation Resources Group
// Author:    dfblackburn
// Created:   May 19, 2016
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

// Axis == 0 -

// TEST enabled -
	// CMD -
	// PAR -

// TEST disabled -
	// CMD -
	// PAR -

// TEST STAT -

// Fast Function -

// Active -
// Power -
	// dumb power -
	// smart shouldPower -
// InitModPos -
	// Factor or Period set in CFG -
	// Factor or Period set in nc map -
// Reference -
// MoveAbsolute -
// MoveAdditive -
// MoveVelocity -
// Jog -
// Halt -
// Stop -

// Set Busy -
// Set Done -
	
// ReadAxisError FUB inputs -
// Setting ErrorID and ErrorCount -
	// ReadAxisError Error -
	// Other errors -
// Setting Error -
// Setting Warning -
// Setting WarningCount -
// Setting ErrorString -

// Reset -

// Check NWCT - good
	// No spam on loss of network -
	// No spam on 0 pAxis -

plcbit AxisBasicFn_Cyclic(struct AxisBasic_typ* t)
{
	
	if (t == 0) return 0;
	
	if (t->IN.CFG.Active == 0) {
		memset(&t->OUT, 0, sizeof(t->OUT));
		t->OUT.Active = 0;
		return 0;
	}
	
	// Check for 0 axis
	if (t->pAxisObject == 0) {
		
		memset(&t->OUT, 0, sizeof(t->OUT));
		
		// TODO: Should we set OUT.Active = IN.CFG.Active here?
		// Not right now.
		t->OUT.Error = 1;
		t->OUT.ErrorCount = 1;
		t->OUT.ErrorID = 29200;
		strcpy(t->OUT.ErrorString, "pAxisObject is 0. No function possible.");
		
		return 0;
	
	}
	
	// Handle TEST interface							
	if (t->TEST.Enable) {

		// Overwrite IN.CMDs and IN.PARs while in TEST mode 
		memcpy(&(t->IN.CMD), &(t->TEST.CMD), sizeof(t->IN.CMD));
		memcpy(&(t->IN.PAR), &(t->TEST.PAR), sizeof(t->IN.PAR));

	} else {

		// Clear TEST.CMDs and load TEST.PARs from machine while not in TEST mode 
		memset(&(t->TEST.CMD), 0, sizeof(t->TEST.CMD));
		memcpy(&(t->TEST.PAR), &(t->IN.PAR), sizeof(t->IN.PAR));
		
	}
	

	// Active
	t->OUT.Active = t->IN.CFG.Active;

	
	// Power
	t->Internal.FUB.Power.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.Power.Enable = t->IN.CMD.Power; // 'dumb Power' input
	//t->Internal.FUB.Power.Enable = t->IN.CMD.Power && t->OUT.DriveStatus.ControllerReady && t->OUT.DriveStatus.NetworkInit; // TODO: 'smart shouldPower' input - Don't use Errorstop based on PLCOpen state diagram
	if(t->Internal.FUB.Power.Enable || t->Internal.FUB.Power.Busy || t->Internal.FUB.Power.Error){
		MC_Power(&t->Internal.FUB.Power);
	}

	
	// InitModPos
//	if (t->Internal.FUB.InitModPos.Period != t->IN.CFG.Period
//	|| t->Internal.FUB.InitModPos.Factor != t->IN.CFG.Factor) {
//	
//		if (t->IN.CFG.Factor == 0) t->IN.CFG.Factor = 1;
//		t->Internal.FUB.InitModPos.Period = t->IN.CFG.Period;
//		t->Internal.FUB.InitModPos.Factor = t->IN.CFG.Factor;
//		t->Internal.FUB.InitModPos.Execute = 1;
//	
//	}
//
//	t->Internal.FUB.InitModPos.Axis = (UDINT)t->pAxisObject;
//	
//	MC_BR_InitModPos(&t->Internal.FUB.InitModPos);
//	
//	t->Internal.FUB.InitModPos.Execute = 0;
	
	
	// Reference									
	t->Internal.FUB.Reference.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.Reference.RestorePositionVariableAddress = t->pRestorePosition;
//	t->Internal.FUB.Reference.DefaultPosition = t->IN.CFG.DefaultPosition;
	t->Internal.FUB.Reference.WaitToInitialize = t->IN.CMD.WaitToInitializeReference;
	t->Internal.FUB.Reference.Reference = t->IN.CMD.Reference;
	t->Internal.FUB.Reference.Position = t->IN.CFG.HomingPosition;
//	t->Internal.FUB.Reference.HomingMode = t->IN.CFG.HomingMode;
	t->Internal.FUB.Reference.ClearReference = t->IN.CMD.ClearReference;
	
	AxisReference(&t->Internal.FUB.Reference);
	
	t->OUT.Referenced = t->Internal.FUB.Reference.Referenced;
	t->OUT.EndlessPositionInitialized = t->Internal.FUB.Reference.EndlessPositionInitialized;
	t->OUT.DataValid = t->Internal.FUB.Reference.DataValid;


	// Move Absolute 
	t->Internal.FUB.MoveAbsolute.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.MoveAbsolute.Execute = t->IN.CMD.MoveAbsolute;
	t->Internal.FUB.MoveAbsolute.Position = t->IN.PAR.Position;
	t->Internal.FUB.MoveAbsolute.Velocity = t->IN.PAR.Velocity;
	t->Internal.FUB.MoveAbsolute.Acceleration = t->IN.PAR.Acceleration;
	t->Internal.FUB.MoveAbsolute.Deceleration = t->IN.PAR.Deceleration;
	t->Internal.FUB.MoveAbsolute.Direction = t->IN.PAR.Direction;

	MC_MoveAbsolute(&t->Internal.FUB.MoveAbsolute);


	// Move Additive 
	t->Internal.FUB.MoveAdditive.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.MoveAdditive.Execute = t->IN.CMD.MoveAdditive;
	t->Internal.FUB.MoveAdditive.Distance = t->IN.PAR.Distance;
	t->Internal.FUB.MoveAdditive.Velocity = t->IN.PAR.Velocity;
	t->Internal.FUB.MoveAdditive.Acceleration = t->IN.PAR.Acceleration;
	t->Internal.FUB.MoveAdditive.Deceleration = t->IN.PAR.Deceleration;

	MC_MoveAdditive(&t->Internal.FUB.MoveAdditive);


	// Move Velocity 
	t->Internal.FUB.MoveVelocity.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.MoveVelocity.Execute = t->IN.CMD.MoveVelocity;
	t->Internal.FUB.MoveVelocity.Velocity = t->IN.PAR.Velocity;
	t->Internal.FUB.MoveVelocity.Acceleration = t->IN.PAR.Acceleration;
	t->Internal.FUB.MoveVelocity.Deceleration = t->IN.PAR.Deceleration;
	t->Internal.FUB.MoveVelocity.Direction = t->IN.PAR.Direction;

	MC_MoveVelocity(&t->Internal.FUB.MoveVelocity);


	// Jog 
	t->Internal.FUB.Jog.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.Jog.Enable = t->IN.CMD.JogForward || t->IN.CMD.JogReverse || t->Internal.FUB.Jog.Jogging;
	t->Internal.FUB.Jog.Velocity = t->IN.PAR.JogVelocity;
	t->Internal.FUB.Jog.Acceleration = t->IN.PAR.JogAcceleration;
	t->Internal.FUB.Jog.Deceleration = t->IN.PAR.JogDeceleration;
	t->Internal.FUB.Jog.JogPositive = t->IN.CMD.JogForward;
	t->Internal.FUB.Jog.JogNegative = t->IN.CMD.JogReverse;

	MC_BR_JogVelocity(&t->Internal.FUB.Jog);


	// Halt												
	t->Internal.FUB.Halt.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.Halt.Execute = t->IN.CMD.Halt;
//	t->Internal.FUB.Halt.Deceleration = t->IN.CFG.StopDeceleration;

	MC_Halt(&t->Internal.FUB.Halt);


	// Stop												
	t->Internal.FUB.Stop.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.Stop.Execute = t->IN.CMD.Stop;
//	t->Internal.FUB.Stop.Deceleration = t->IN.CFG.StopDeceleration;

	MC_Stop(&t->Internal.FUB.Stop);


	// Busy 
	t->OUT.Busy = t->Internal.FUB.Reference.Busy
		||	t->Internal.FUB.MoveAbsolute.Busy
		||	t->Internal.FUB.MoveAdditive.Busy
		||	(t->Internal.FUB.MoveVelocity.Busy && t->Internal.FUB.MoveVelocity.Execute && !(t->Internal.FUB.MoveVelocity.InVelocity))
		||	t->Internal.FUB.Jog.Busy
		||	(t->Internal.FUB.MoveCyclicPosition.Busy && (!t->Internal.FUB.MoveCyclicPosition.Valid || (t->OUT.ActualPosition != t->IN.PAR.Position)) )
		||	t->Internal.FUB.Halt.Busy
		||	t->Internal.FUB.Stop.Busy
		||	t->Internal.FUB.Reset.Busy;
					

	// Done 
	t->OUT.Done = t->Internal.FUB.Reference.Done
		||	t->Internal.FUB.MoveAbsolute.Done
		||	t->Internal.FUB.MoveAdditive.Done
		||	t->Internal.FUB.MoveVelocity.InVelocity
		||	(t->Internal.FUB.MoveCyclicPosition.Valid && (t->OUT.ActualPosition == t->IN.PAR.Position))
		||	t->Internal.FUB.Halt.Done
		||	t->Internal.FUB.Stop.Done
		||	t->Internal.FUB.Reset.Done;
					

	// Get axis error information 
	t->Internal.FUB.ReadAxisError.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.ReadAxisError.Enable = !(t->Internal.FUB.ReadAxisError.Error);
//	t->Internal.FUB.ReadAxisError.Acknowledge = t->IN.CMD.AcknowledgeError;
//	t->Internal.FUB.ReadAxisError.Mode = mcTEXT;
//
//	t->Internal.FUB.ReadAxisError.Configuration.Format = mcWRAP + mcNULL;
//	t->Internal.FUB.ReadAxisError.Configuration.LineLength = sizeof(t->Internal.ErrorString[0]);
//	t->Internal.FUB.ReadAxisError.Configuration.DataLength = sizeof(t->Internal.ErrorString);
//	t->Internal.FUB.ReadAxisError.Configuration.DataAddress = (UDINT)&(t->Internal.ErrorString);
//	strcpy(t->Internal.FUB.ReadAxisError.Configuration.DataObjectName, t->ErrorTextModuleName);

	MC_ReadAxisError(&t->Internal.FUB.ReadAxisError);


	if (t->Internal.FUB.ReadAxisError.Valid) {
//		t->OUT.WarningCount = t->Internal.FUB.ReadAxisError.AxisWarningCount;
//		t->OUT.ErrorID = t->Internal.FUB.ReadAxisError.ErrorRecord.Number;
//		t->OUT.ErrorCount = t->Internal.FUB.ReadAxisError.AxisErrorCount + t->Internal.FUB.ReadAxisError.FunctionBlockErrorCount;
	}

	if (t->Internal.FUB.ReadAxisError.Error) {
		t->OUT.ErrorID = t->Internal.FUB.ReadAxisError.ErrorID;
		t->OUT.ErrorCount = 1;
	}


	// Set Error and Warning
	if (t->OUT.ErrorCount != 0) {
		t->OUT.Error = 1;
	} else {
		t->OUT.Error = 0;
	}
	
	if ( 1 /*t->Internal.FUB.ReadAxisError.AxisWarningCount != 0*/) {
		t->OUT.Warning = 1;
	} else {
		t->OUT.Warning = 0;
	}
	

	// Copy Internal.ErrorString[0..3] to OUT.STAT.ErrorString 
	strcpy(t->OUT.ErrorString, t->Internal.ErrorString[0]);

	if (strcmp(t->Internal.ErrorString[1], "") != 0) {
		strcat(t->OUT.ErrorString, " / ");
		strcat(t->OUT.ErrorString, t->Internal.ErrorString[1]);
	}

	if (strcmp(t->Internal.ErrorString[2], "") != 0) {
		strcat(t->OUT.ErrorString, " / ");
		strcat(t->OUT.ErrorString, t->Internal.ErrorString[2]);
	}

	if (strcmp(t->Internal.ErrorString[3], "") != 0) {
		strcat(t->OUT.ErrorString, " / ");
		strcat(t->OUT.ErrorString, t->Internal.ErrorString[3]);
	}


	// Reset
	t->Internal.FUB.Reset.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.Reset.Execute = t->IN.CMD.Reset;

	MC_Reset(&t->Internal.FUB.Reset);

	
	// Axis Status
	t->Internal.FUB.Status.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.Status.Enable = !t->internal.Status.Error;
	AxisStatus(&t->Internal.FUB.Status);
	

	// Reset AcknowledgeError and Reset
	t->IN.CMD.AcknowledgeError = 0;
	t->TEST.CMD.AcknowledgeError = 0;

	t->IN.CMD.Reset = 0;
	t->TEST.CMD.Reset = 0;


	// Map TEST.STAT items												
	t->TEST.STAT.Busy = t->OUT.Busy;
	t->TEST.STAT.Done = t->OUT.Done;
	t->TEST.STAT.Warning = t->OUT.Warning;
	t->TEST.STAT.WarningCount = t->OUT.WarningCount;
	t->TEST.STAT.Error = t->OUT.Error;
	t->TEST.STAT.ErrorCount = t->OUT.ErrorCount;
	t->TEST.STAT.ErrorID = t->OUT.ErrorID;
	t->TEST.STAT.ActualPosition = t->OUT.ActualPosition;
	t->TEST.STAT.ActualVelocity = t->OUT.ActualVelocity;
//	t->TEST.STAT.NetworkInit = t->OUT.DriveStatus.NetworkInit;
//	t->TEST.STAT.ControllerStatus = t->OUT.DriveStatus.ControllerStatus;
//	t->TEST.STAT.HomingOk = t->OUT.DriveStatus.HomingOk;

	return 0;
	
}
