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
		t->ActualVelocity = 0;
		memset(&t->AxisInfo, 0, sizeof(t->AxisInfo));
		
		t->Valid = 0;
		t->Busy = 0;
		t->Error = 1;
		t->ErrorID = 29200;
		
		return;
	
	}
	
	
	// Axis Info
	t->internal.ReadAxisInfo.Axis = t->Axis;
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
	t->internal.ReadActualPosition.Axis = t->Axis;
	t->internal.ReadActualPosition.Enable = t->Enable && !t->internal.ReadActualPosition.Error && t->AxisInfo.CommunicationReady;

	MC_ReadActualPosition(&t->internal.ReadActualPosition);

	t->ActualPosition = t->internal.ReadActualPosition.Position;
	
	
	//	TODO: add MC_BR_ReadCyclicPosition FUB for more specificity in where the position is read from
	
	
	// Velocity
	t->internal.ReadActualVelocity.Axis = t->Axis;
	t->internal.ReadActualVelocity.Enable = t->Enable && !t->internal.ReadActualVelocity.Error && t->AxisInfo.CommunicationReady;

	MC_ReadActualVelocity(&t->internal.ReadActualVelocity);

	t->ActualVelocity = t->internal.ReadActualVelocity.Velocity;

	
	// limits
	// get limits from the configuration (not the drive) on startup
	// if the configuration is ever changed at runtime this will not know
	t->internal.ReadAxisLimits.Execute = 1;
	memset(&t->internal.ReadAxisLimits.AdvancedParameters.Name, 0, sizeof(t->internal.ReadAxisLimits.AdvancedParameters.Name));
	t->internal.ReadAxisLimits.Component = t->Axis;
	t->internal.ReadAxisLimits.DataAddress = &t->AxisInfo.AxisLimits;
	t->internal.ReadAxisLimits.DataType = mcCFG_AX_MOVE_LIM;
	t->internal.ReadAxisLimits.ExecutionMode = mcEM_IMMEDIATELY;
	t->internal.ReadAxisLimits.Mode = mcPPM_READ;
	
	MC_BR_ProcessParam(&t->internal.ReadAxisLimits);


	t->internal.ReadLibraryInfo.Axis = t->Axis;
	t->internal.ReadLibraryInfo.Execute = 1;	
	MC_BR_GetAxisLibraryInfo( &t->internal.ReadLibraryInfo );
	strcpy( &t->AxisInfo.libraryInfo, t->internal.ReadLibraryInfo.Info.Name );
	
	// FUB status
	t->Valid = t->internal.ReadAxisInfo.Valid
		&& t->internal.ReadActualPosition.Valid
		&& t->internal.ReadActualVelocity.Valid
		&& t->internal.ReadAxisLimits.Done;
	
	t->Busy = t->internal.ReadAxisInfo.Busy
		|| t->internal.ReadActualPosition.Busy
		|| t->internal.ReadActualVelocity.Busy
		|| t->internal.ReadAxisLimits.Busy;
	
	t->Error = t->internal.ReadAxisInfo.Error
		|| t->internal.ReadActualPosition.Error
		|| t->internal.ReadActualVelocity.Error
		|| t->internal.ReadAxisLimits.Error;
	
	if (t->internal.ReadAxisInfo.Error) {
		t->ErrorID = t->internal.ReadAxisInfo.ErrorID;
	
	} else if (t->internal.ReadActualPosition.Error) {
		t->ErrorID = t->internal.ReadActualPosition.ErrorID;
	
	} else if (t->internal.ReadActualVelocity.Error) {
		t->ErrorID = t->internal.ReadActualVelocity.ErrorID;
		
	} else if (t->internal.ReadAxisLimits.Error) {
		t->ErrorID = t->internal.ReadAxisLimits.ErrorID;
	
	} else {
		t->ErrorID = 0;
	
	}
	
} // AxisStatus()
