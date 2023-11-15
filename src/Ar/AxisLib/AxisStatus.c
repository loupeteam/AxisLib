//--------------------------------------------------------------------------------
// File: AxisStatus.c
// Copyright (c) 2023 Loupe
// https://loupe.team
// 
// This file is part of AxisLib, licensed under the MIT License.
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

// Axis == 0 - good

// DriveStatus - good
// Position - good
// CyclicPosition - good
// Velocity - good
// PLC Open State - good
	// Errorstop - good
	// Disabled - good
	// Standstill - good
	// Stopping - good
	// Homing - good
	// DiscreteMotion - good
	// SynchronizedMotion - good
	// ContinuousMotion - good

// FUB status - good
	// Valid - good
	// Busy - good
	// Error - not tested
	// ErrorID - not tested

// Check NWCT - good
	// No spam on loss of network - good
	// No spam on Axis == 0 - good


void AxisStatus(struct AxisStatus* t)
{

	if (t == 0) return;
	
	if (t->Axis == 0) {
		
		t->ActualPosition = 0;
		memset(&t->ActualCyclicPosition, 0, sizeof(t->ActualCyclicPosition));
		t->ActualVelocity = 0;
		memset(&t->DriveStatus, 0, sizeof(t->DriveStatus));
		memset(&t->PLCOpenState, 0, sizeof(t->PLCOpenState));
		
		t->Valid = 0;
		t->Busy = 0;
		t->Error = 1;
		t->ErrorID = 29200;
		
		return;
	
	}
	
	// Drive status
	t->internal.ReadDriveStatus.Axis = (UDINT)t->Axis;
	t->internal.ReadDriveStatus.Enable = t->Enable && !t->internal.ReadDriveStatus.Error;
	t->internal.ReadDriveStatus.AdrDriveStatus = (UDINT)&(t->DriveStatus);				

	MC_BR_ReadDriveStatus(&t->internal.ReadDriveStatus);


	// Position
	t->internal.ReadActualPosition.Axis = (UDINT)t->Axis;
	t->internal.ReadActualPosition.Enable = t->Enable && !t->internal.ReadActualPosition.Error && t->DriveStatus.NetworkInit;

	MC_ReadActualPosition(&t->internal.ReadActualPosition);

	t->ActualPosition = t->internal.ReadActualPosition.Position;
	
	
	// Cyclic position
	t->internal.ReadCyclicPosition.Axis = (UDINT)t->Axis;
	t->internal.ReadCyclicPosition.Enable = t->Enable && !t->internal.ReadCyclicPosition.Error && t->DriveStatus.NetworkInit;
	t->internal.ReadCyclicPosition.ParID = t->ReadCyclicPositionParID;

	MC_BR_ReadCyclicPosition(&t->internal.ReadCyclicPosition);

	memcpy(&t->ActualCyclicPosition, &t->internal.ReadCyclicPosition.CyclicPosition, sizeof(t->ActualCyclicPosition));
	
	
	// Velocity
	t->internal.ReadActualVelocity.Axis = (UDINT)t->Axis;
	t->internal.ReadActualVelocity.Enable = t->Enable && !t->internal.ReadActualVelocity.Error && t->DriveStatus.NetworkInit;

	MC_ReadActualVelocity(&t->internal.ReadActualVelocity);

	t->ActualVelocity = t->internal.ReadActualVelocity.Velocity;

	
	// PLC Open state 
	t->internal.ReadStatus.Axis = (UDINT)t->Axis;
	t->internal.ReadStatus.Enable = t->Enable && !t->internal.ReadStatus.Error;

	MC_ReadStatus(&t->internal.ReadStatus);

	t->PLCOpenState.Errorstop = t->internal.ReadStatus.Errorstop;
	t->PLCOpenState.Disabled = t->internal.ReadStatus.Disabled;
	t->PLCOpenState.StandStill = t->internal.ReadStatus.StandStill;
	t->PLCOpenState.Stopping = t->internal.ReadStatus.Stopping;
	t->PLCOpenState.Homing = t->internal.ReadStatus.Homing;
	t->PLCOpenState.DiscreteMotion = t->internal.ReadStatus.DiscreteMotion;
	t->PLCOpenState.SynchronizedMotion = t->internal.ReadStatus.SynchronizedMotion;
	t->PLCOpenState.ContinuousMotion = t->internal.ReadStatus.ContinuousMotion;

	if (t->PLCOpenState.Errorstop) {
		t->PLCOpenState.State = AXISLIB_PLCOPEN_ST_ERRORSTOP;

	} else if (t->PLCOpenState.Disabled) {
		t->PLCOpenState.State = AXISLIB_PLCOPEN_ST_DISABLED;

	} else if (t->PLCOpenState.StandStill) {
		t->PLCOpenState.State = AXISLIB_PLCOPEN_ST_STANDSTILL;

	} else if (t->PLCOpenState.Stopping) {
		t->PLCOpenState.State = AXISLIB_PLCOPEN_ST_STOPPING;

	} else if (t->PLCOpenState.Homing) {
		t->PLCOpenState.State = AXISLIB_PLCOPEN_ST_HOMING;

	} else if (t->PLCOpenState.DiscreteMotion) {
		t->PLCOpenState.State = AXISLIB_PLCOPEN_ST_DISCRETEMOTN;

	} else if (t->PLCOpenState.SynchronizedMotion) {
		t->PLCOpenState.State = AXISLIB_PLCOPEN_ST_SYNCMOTN;

	} else if (t->PLCOpenState.ContinuousMotion) {
		t->PLCOpenState.State = AXISLIB_PLCOPEN_ST_CONTMOTN;

	} else {
		t->PLCOpenState.State = AXISLIB_PLCOPEN_ST_INVALID;		
	}
	
	// FUB status
	t->Valid = t->internal.ReadDriveStatus.Valid
		&& t->internal.ReadActualPosition.Valid
		&& t->internal.ReadCyclicPosition.Valid
		&& t->internal.ReadActualVelocity.Valid
		&& t->internal.ReadStatus.Valid;
	
	t->Busy = t->internal.ReadDriveStatus.Busy
		|| t->internal.ReadActualPosition.Busy
		|| t->internal.ReadCyclicPosition.Busy
		|| t->internal.ReadActualVelocity.Busy
		|| t->internal.ReadStatus.Busy;
	
	t->Error = t->internal.ReadDriveStatus.Error
		|| t->internal.ReadActualPosition.Error
		|| t->internal.ReadCyclicPosition.Error
		|| t->internal.ReadActualVelocity.Error
		|| t->internal.ReadStatus.Error;
	
	if (t->internal.ReadDriveStatus.Error) {
		t->ErrorID = t->internal.ReadDriveStatus.ErrorID;
	
	} else if (t->internal.ReadActualPosition.Error) {
		t->ErrorID = t->internal.ReadActualPosition.ErrorID;
	
	} else if (t->internal.ReadCyclicPosition.Error) {
		t->ErrorID = t->internal.ReadCyclicPosition.ErrorID;
	
	} else if (t->internal.ReadActualVelocity.Error) {
		t->ErrorID = t->internal.ReadActualVelocity.ErrorID;
	
	} else if (t->internal.ReadStatus.Error) {
		t->ErrorID = t->internal.ReadStatus.ErrorID;
	
	} else {
		t->ErrorID = 0;
	
	}
	
} // AxisStatus()
