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

//-----------------------------------------------------------------
// Internal: Handle ReadCyclicPosition and MoveCyclicPosition FUBs
//-----------------------------------------------------------------

// Check Active - good
// Status - good
// Move - good
// SetHardwareInputs - good

plcbit axisInternalFastFn(struct AxisBasic_typ* t)
{

	if (t == 0) return 0;
	
	if (t->IN.CFG.Active == 0) {
		memset(&t->OUT, 0, sizeof(t->OUT));
		t->OUT.Active = 0;
		return 0;
	}
	
	// AxisStatus
	t->Internal.FUB.Status.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.Status.Enable = 1; // I am a little concerned about this, but it works so far
	t->Internal.FUB.Status.ReadCyclicPositionParID = t->IN.CFG.ReadCyclicPositionParID;
	
	AxisStatus(&t->Internal.FUB.Status);
	
	t->OUT.ActualPosition = t->Internal.FUB.Status.ActualPosition;
	t->OUT.ActualPositionPrecise = t->Internal.FUB.Status.ActualCyclicPosition.Integer + t->Internal.FUB.Status.ActualCyclicPosition.Real;
	memcpy(&t->OUT.ActualCyclicPosition, &t->Internal.FUB.Status.ActualCyclicPosition, sizeof(t->OUT.ActualCyclicPosition));
	t->OUT.ActualVelocity = t->Internal.FUB.Status.ActualVelocity;
	memcpy(&t->OUT.DriveStatus, &t->Internal.FUB.Status.DriveStatus, sizeof(t->OUT.DriveStatus));
	memcpy(&t->OUT.PLCOpenState, &t->Internal.FUB.Status.PLCOpenState, sizeof(t->OUT.PLCOpenState));

	
	// MoveCyclicPosition
	t->Internal.FUB.MoveCyclicPosition.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.MoveCyclicPosition.Enable = t->IN.CMD.MoveCyclicPosition;
	memcpy(&t->Internal.FUB.MoveCyclicPosition.CyclicPosition, &t->IN.PAR.CyclicPosition, sizeof(t->IN.PAR.CyclicPosition));
	t->Internal.FUB.MoveCyclicPosition.Velocity = t->IN.PAR.Velocity;
	t->Internal.FUB.MoveCyclicPosition.Acceleration = t->IN.PAR.Acceleration;
	t->Internal.FUB.MoveCyclicPosition.Deceleration = t->IN.PAR.Deceleration;
	
	MC_BR_MoveCyclicPosition(&t->Internal.FUB.MoveCyclicPosition);
	
	
	// SetHardwareInputs
	t->Internal.FUB.SetHardwareInputs.Axis = (UDINT)t->pAxisObject;
	t->Internal.FUB.SetHardwareInputs.Enable = 1;
	t->Internal.FUB.SetHardwareInputs.HomeSwitch = t->IO.diHomeSwitch;
	t->Internal.FUB.SetHardwareInputs.NegHWSwitch = t->IO.diNegHWSwitch;
	t->Internal.FUB.SetHardwareInputs.PosHWSwitch = t->IO.diPosHWSwitch;
	t->Internal.FUB.SetHardwareInputs.Trigger1 = t->IO.diTrigger1;
	t->Internal.FUB.SetHardwareInputs.Trigger2 = t->IO.diTrigger2;

	MC_BR_SetHardwareInputs(&t->Internal.FUB.SetHardwareInputs);
	
	
	// Velocity, accel, and decel are not required. 
	// Might want to think about this some.

	return 0;
	
}
