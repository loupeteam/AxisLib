//--------------------------------------------------------------------------------
// Copyright: Automation Resources Group
// Author:    dfblackburn
// Created:   May 23, 2016
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

//-----------------------------------------
// Gather status information about an axis
//-----------------------------------------

// Axis == 0 -

// AxisInfo -
// Position -
// CyclicPosition -
// Velocity -
// PLC Open State -
	// Errorstop -
	// Disabled -
	// Standstill -
	// Stopping -
	// Homing -
	// DiscreteMotion -
	// SynchronizedMotion -
	// ContinuousMotion -

// FUB status -
	// Valid -
	// Busy -
	// Error -
	// ErrorID -

// Check NWCT -
	// No spam on loss of network -
	// No spam on Axis == 0 -


void AxisStatus(struct AxisStatus* t)
{

	if (t == 0) return;
	
	if (t->Axis == 0) {
		
		t->ActualPosition = 0;
		memset(&t->ActualCyclicPosition, 0, sizeof(t->ActualCyclicPosition));
		t->ActualVelocity = 0;
		memset(&t->AxisInfo, 0, sizeof(t->AxisInfo));
		
		t->Valid = 0;
		t->Busy = 0;
		t->Error = 1;
		t->ErrorID = 29200;
		
		return;
	
	}
	
	
	// Axis Info
	t->internal.ReadAxisInfo.Axis = (McAxisType*)t->Axis;
	t->internal.ReadAxisInfo.Enable = t->Enable && !t->internal.ReadAxisInfo.Error;
	
	MC_ReadAxisInfo(&t->internal.ReadAxisInfo);

	memcpy(&t->AxisInfo, &t->internal.ReadAxisInfo.AdditionalInfo, sizeof(t->internal.ReadAxisInfo.AdditionalInfo));
	t->AxisInfo.AxisWarning = t->internal.ReadAxisInfo.AxisWarning;
	t->AxisInfo.CommunicationReady = t->internal.ReadAxisInfo.CommunicationReady;
	t->AxisInfo.IsHomed = t->internal.ReadAxisInfo.IsHomed;
	t->AxisInfo.PowerOn = t->internal.ReadAxisInfo.PowerOn;
	t->AxisInfo.ReadyForPowerOn = t->internal.ReadAxisInfo.ReadyForPowerOn;
	t->AxisInfo.Simulation = t->internal.ReadAxisInfo.Simulation;

	
	// Position
	t->internal.ReadActualPosition.Axis = (UDINT)t->Axis;
	t->internal.ReadActualPosition.Enable = t->Enable && !t->internal.ReadActualPosition.Error && t->AxisInfo.CommunicationReady;

	MC_ReadActualPosition(&t->internal.ReadActualPosition);

	t->ActualPosition = t->internal.ReadActualPosition.Position;
	
	
	//	TODO: add MC_BR_ReadCyclicPosition FUB for more specificity in where the position is read from
	
	
	// Velocity
	t->internal.ReadActualVelocity.Axis = (UDINT)t->Axis;
	t->internal.ReadActualVelocity.Enable = t->Enable && !t->internal.ReadActualVelocity.Error && t->AxisInfo.CommunicationReady;

	MC_ReadActualVelocity(&t->internal.ReadActualVelocity);

	t->ActualVelocity = t->internal.ReadActualVelocity.Velocity;

	
	// FUB status
	t->Valid = t->internal.ReadAxisInfo.Valid
		&& t->internal.ReadActualPosition.Valid
		&& t->internal.ReadActualVelocity.Valid
	
	t->Busy = t->internal.ReadAxisInfo.Busy
		|| t->internal.ReadActualPosition.Busy
		|| t->internal.ReadActualVelocity.Busy
	
	t->Error = t->internal.ReadAxisInfo.Error
		|| t->internal.ReadActualPosition.Error
		|| t->internal.ReadActualVelocity.Error
	
	if (t->internal.ReadAxisInfo.Error) {
		t->ErrorID = t->internal.ReadAxisInfo.ErrorID;
	
	} else if (t->internal.ReadActualPosition.Error) {
		t->ErrorID = t->internal.ReadActualPosition.ErrorID;
	
	} else if (t->internal.ReadActualVelocity.Error) {
		t->ErrorID = t->internal.ReadActualVelocity.ErrorID;
	
	} else {
		t->ErrorID = 0;
	
	}
	
} // AxisStatus()
