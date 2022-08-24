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

//---------------------------------------------------------------
// Handle axis referencing, including allowing jogging without a 
// reference and maintaining reference status
//---------------------------------------------------------------

// Axis == 0 -

// Recover from drive reset -
// Check that it plays well with AxisExpandLimits -

// Startup -
	// WaitToInitialize -
	// Errorstop on startup -
	// No data -
	// Bad data -
	// Valid data -
	// Restore homing parameters after initial direct home -

// Reference -
	// Without endless pos -
	// With endless pos -
	// Restore homing parameters after direct home -

// ClearReference -
	// Without endless pos -
	// With endless pos -
	// Next reference -
	// Next startup -

// Loss of HomingOk -
	// Internal home -
	// External home -
	// Encoder error -

// Autohome -

// Check NWCT -
	// No spam on loss of network -
	// No spam on Axis == 0 -


void AxisReference(struct AxisReference* t)
{
	
	if (t == 0) return;
	
	// Check Axis
	if (t->Axis == 0) {
		
		t->Done = 0;
		t->Busy = 0;
		t->CommandAborted = 0;
		t->Error = 1;
		t->ErrorID = 29200;
		t->Referenced = 0;
		t->EndlessPositionInitialized = 0;
		t->DataValid = 0;
		
		return;
	
	}
	
	// Get drive status information
//	t->internal.readDriveStatus.Axis = t->Axis;
//	t->internal.readDriveStatus.Enable = !t->internal.readDriveStatus.Error;
//	t->internal.readDriveStatus.AdrDriveStatus = (UDINT)&(t->internal.driveStatus);				
//	MC_BR_ReadDriveStatus(&t->internal.readDriveStatus);
	
	t->internal.readStatus.Axis = t->Axis;
	t->internal.readStatus.Enable = !t->internal.readStatus.Error;
	MC_ReadStatus(&t->internal.readStatus);
		
	
	// Reset internal commands
	if (!t->Reference) {
		t->internal.Reference = 0;
	}
	
	if (!t->ClearReference) {
		t->internal.ClearReference = 0;
	}
	
	// If drive is reset, reinitialize
//	if (t->internal.driveStatus.NetworkInit && !t->internal.oldNetworkInit && t->internal.driveStatus.ResetDone) {
		t->internal.state = AXISLIB_REFST_START;
//	}

//	t->internal.oldNetworkInit = t->internal.driveStatus.NetworkInit;
		
	
	// Main state machine
	//--------------------
	
	switch (t->internal.state) {

		case AXISLIB_REFST_START:

			// Initialize outputs
			t->Done = 0;
			t->Busy = 0;
			t->CommandAborted = 0;
			t->Error = 0;
			t->ErrorID = 0;			
			t->Referenced = 0;
			t->EndlessPositionInitialized = 0;
			t->DataValid = 0;

			// Reset FUBs
//			t->internal.checkEndlessPos.Execute = 0;
//			t->internal.initEndlessPos.Execute = 0;
			t->internal.home.Execute = 0;
			
//			if (	t->internal.driveStatus.NetworkInit 
//				&& t->internal.readDriveStatus.Valid
//				&& !t->WaitToInitialize 
//				&& (t->internal.readStatus.Disabled || t->internal.readStatus.StandStill)
//				&& t->internal.readStatus.Valid
//			) {
//			
//				t->Busy = 1;
//			
//				memcpy(&t->internal.initHomeParam, &((ACP10AXIS_typ*)t->Axis)->move.homing.parameter, sizeof(t->internal.initHomeParam));
//			
//				if (t->EndlessPositionDataAddress != 0) {
//					t->internal.state = AXISLIB_REFST_START_CHECK;
//				} else {
					t->internal.state = AXISLIB_REFST_START_HOME;
//				}
//			}
		
			break;
		
		
		case AXISLIB_REFST_START_CHECK:
			
			// Check endless position
			// If data is valid, initialize, then home restore pos
			// If bad data, do not initialize, then direct home to DefaultPosition
	
//			t->internal.checkEndlessPos.Axis = t->Axis;
//			t->internal.checkEndlessPos.DataAddress = t->EndlessPositionDataAddress;
//			t->internal.checkEndlessPos.Execute = 1;
		
//			if (t->internal.checkEndlessPos.Done) {
//		
//				t->internal.checkEndlessPos.Execute = 0;
//		
//				t->EndlessPositionInitialized = t->internal.checkEndlessPos.EndlessPositionInitialized;
//				t->DataValid = t->internal.checkEndlessPos.DataValid;
//
//				if (t->DataValid) {
//					t->internal.state = AXISLIB_REFST_START_INIT;
//				} else {
//					t->internal.state = AXISLIB_REFST_START_HOME;
//				}
//			
//			} else if (t->internal.checkEndlessPos.Error) {
//		
//				t->internal.checkEndlessPos.Execute = 0;
//			
//				t->Error = 1;
//				t->ErrorID = t->internal.checkEndlessPos.ErrorID;
//			
//				t->EndlessPositionInitialized = 0;
//				t->DataValid = 0;
//
				t->internal.state = AXISLIB_REFST_START_HOME;
//		
//			}

			break;
			
		
		case AXISLIB_REFST_START_INIT:
	
//			t->internal.initEndlessPos.Axis = t->Axis;
//			t->internal.initEndlessPos.DataAddress = t->EndlessPositionDataAddress;
//			t->internal.initEndlessPos.Execute = 1;
//
//			if (t->internal.initEndlessPos.Done) {
//	
//				t->internal.initEndlessPos.Execute=	0;
//			
//				t->EndlessPositionInitialized = 1;
//				t->internal.state = AXISLIB_REFST_START_HOME;
//		
//			} else if (t->internal.initEndlessPos.Error) {
//	
//				t->internal.initEndlessPos.Execute = 0;
//
//				t->Error = 1;
//				t->ErrorID = t->internal.initEndlessPos.ErrorID;
//
//				t->EndlessPositionInitialized = 0;
//				
				t->internal.state = AXISLIB_REFST_START_HOME;
//	
//			}
	
			break;
		
		
		case AXISLIB_REFST_START_HOME:

			t->internal.home.Axis = t->Axis;
			t->internal.home.Execute = 1;
			t->internal.home.Position = t->DefaultPosition;
			
			if (t->DataValid && t->EndlessPositionInitialized) {
				t->internal.home.HomingMode = mcHOMING_RESTORE_POSITION;
			} else {
				t->internal.home.HomingMode = mcHOMING_DIRECT;
			}
			
//			if (t->internal.home.Done && t->internal.driveStatus.HomingOk) {
//		
//				t->internal.home.Execute = 0;
//			
//				memcpy(&((ACP10AXIS_typ*)t->Axis)->move.homing.parameter, &t->internal.initHomeParam, sizeof(t->internal.initHomeParam));
//			
//				t->Busy = 0;
//			
//				if (t->internal.home.HomingMode == mcHOME_RESTORE_POS) {
//					t->Referenced = 1;
//				}
//				
//				t->internal.state = AXISLIB_REFST_IDLE;
//				
//			} else if (t->internal.home.CommandAborted) {
//				
//				t->internal.home.Execute = 0;
//			
//				memcpy(&((ACP10AXIS_typ*)t->Axis)->move.homing.parameter, &t->internal.initHomeParam, sizeof(t->internal.initHomeParam));
//			
//				t->Busy = 0;
//				
//				t->internal.state = AXISLIB_REFST_IDLE;
//				
//			} else if (t->internal.home.Error) {
//		
//				t->internal.home.Execute = 0;
//			
//				memcpy(&((ACP10AXIS_typ*)t->Axis)->move.homing.parameter, &t->internal.initHomeParam, sizeof(t->internal.initHomeParam));
//			
//				t->Busy = 0;
//				t->Error = 1;
//				t->ErrorID = t->internal.home.ErrorID;
//			
				t->internal.state = AXISLIB_REFST_IDLE;
//		
//			}
		
			break;
		
		
		case AXISLIB_REFST_IDLE:
			
			// Set status
			t->Busy = 0;
			
			if (!t->Reference && !t->ClearReference) {
				// This will wipe out errors on startup. A little weird, but probably not critical.
				t->Done = 0;
				t->CommandAborted = 0;
				t->Error = 0;
				t->ErrorID = 0;
			}
							
			// Reset FUBs 
//			t->internal.checkEndlessPos.Execute = 0;
//			t->internal.initEndlessPos.Execute = 0;
			t->internal.home.Execute = 0;
		
			// Check for commands
			if (t->Reference && !t->internal.Reference) {
				
				t->internal.Reference = 1;
			
//				memcpy(&t->internal.initHomeParam, &((ACP10AXIS_typ*)t->Axis)->move.homing.parameter, sizeof(t->internal.initHomeParam));
				
				t->Done = 0;
				t->Busy = 1;
				t->CommandAborted = 0;
				t->Error = 0;
				t->ErrorID = 0;
				t->Referenced = 0;
				
				if (t->EndlessPositionDataAddress != 0) {
					t->internal.state = AXISLIB_REFST_REF_INIT;
				} else {
					t->internal.state = AXISLIB_REFST_REF_HOME;
				}
		
			} else if ((t->ClearReference && !t->internal.ClearReference) || t->internal.needToClearReference) {
				
				t->internal.ClearReference = 1;
				t->internal.needToClearReference = 0;

				if (t->EndlessPositionDataAddress != 0) {
					
					t->Done = 0;
					t->Busy = 1;
					t->CommandAborted = 0;
					t->Error = 0;
					t->ErrorID = 0;
					t->Referenced = 0;
				
					t->internal.state = AXISLIB_REFST_CLEAR_INIT;
				
				} else {
				
					t->Done = 1;
					t->Busy = 0;
					t->CommandAborted = 0;
					t->Error = 0;
					t->ErrorID = 0;
					t->Referenced = 0;
					
				}
				
			}
		
			break;
		
		
		case AXISLIB_REFST_REF_INIT:
	
//			t->internal.initEndlessPos.Axis = t->Axis;
//			t->internal.initEndlessPos.DataAddress = t->EndlessPositionDataAddress;
//			t->internal.initEndlessPos.Execute = 1;
//
//			if (t->internal.initEndlessPos.Done) {
//	
//				t->internal.initEndlessPos.Execute = 0;
//			
//				t->EndlessPositionInitialized = 1;
//				
//				t->internal.state = AXISLIB_REFST_REF_HOME;
//		
//			} else if (t->internal.initEndlessPos.Error) {
//	
//				t->internal.initEndlessPos.Execute = 0;
//
//				t->Error = 1;
//				t->ErrorID = t->internal.initEndlessPos.ErrorID;
//				t->EndlessPositionInitialized = 0;
//				
				t->internal.state = AXISLIB_REFST_REF_HOME;
//	
//			}
	
			break;
		
		
		case AXISLIB_REFST_REF_HOME:

			t->internal.home.Axis = t->Axis;
			t->internal.home.Execute = 1;
			t->internal.home.Position = t->Position;
			t->internal.home.HomingMode = t->HomingMode;
			
//			if (t->internal.home.Done && t->internal.driveStatus.HomingOk) {
//		
//				t->internal.home.Execute = 0;
//			
//				memcpy(&((ACP10AXIS_typ*)t->Axis)->move.homing.parameter, &t->internal.initHomeParam, sizeof(t->internal.initHomeParam));
//			
//				t->Done = 1;
//				t->Busy = 0;
//				t->Referenced = 1;
//				t->DataValid = t->EndlessPositionInitialized;
//								
//				t->internal.state = AXISLIB_REFST_IDLE;
//				
//			} else if (t->internal.home.CommandAborted) {
//				
//				t->internal.home.Execute = 0;
//			
//				memcpy(&((ACP10AXIS_typ*)t->Axis)->move.homing.parameter, &t->internal.initHomeParam, sizeof(t->internal.initHomeParam));
//				
//				t->CommandAborted = 1;
//				t->Busy = 0;
//				t->Referenced = 0;
//				
//				t->internal.state = AXISLIB_REFST_IDLE;
//				
//			} else if (t->internal.home.Error) {
//		
//				t->internal.home.Execute = 0;
//			
//				memcpy(&((ACP10AXIS_typ*)t->Axis)->move.homing.parameter, &t->internal.initHomeParam, sizeof(t->internal.initHomeParam));
//			
//				t->Error = 1;
//				t->ErrorID = t->internal.home.ErrorID;
//				t->Busy = 0;
//				t->Referenced = 0;
//			
				t->internal.state = AXISLIB_REFST_IDLE;
//		
//			}
		
			break;
		
		
		case AXISLIB_REFST_CLEAR_INIT:
		
			// Init to internal and clear external to REALLY clear the reference
//			t->internal.initEndlessPos.Axis = t->Axis;
//			t->internal.initEndlessPos.DataAddress = (UDINT)&(t->internal.endlessPosition);
//			t->internal.initEndlessPos.Execute = 1;
//
//			if (t->internal.initEndlessPos.Done) {
//	
//				t->internal.initEndlessPos.Execute = 0;
//			
//				if (t->EndlessPositionDataAddress != 0) {
//					memset((void*)t->EndlessPositionDataAddress, 0, sizeof(MC_ENDLESS_POSITION));
//				}
//				
//				t->Done = 1;
//				t->Busy = 0;
//				t->Referenced = 0;
//				t->EndlessPositionInitialized = 0;
//				t->DataValid = 0;
//				
//				t->internal.state = AXISLIB_REFST_IDLE;
//		
//			} else if (t->internal.initEndlessPos.Error) {
//	
//				t->internal.initEndlessPos.Execute = 0;
//
//				t->Error = 1;
//				t->ErrorID = t->internal.initEndlessPos.ErrorID;
//				t->Busy = 0;
//				t->Referenced = 0;
//				t->EndlessPositionInitialized = 0;
//				t->DataValid = 0;
//				
				t->internal.state = AXISLIB_REFST_IDLE;
//	
//			}
		
			break;
		
		
	} // switch (state)
	
	// Call FUBs
//	MC_BR_InitEndlessPosition(&t->internal.initEndlessPos);
//	MC_BR_CheckEndlessPosition(&t->internal.checkEndlessPos);
	MC_Home(&t->internal.home);
	
	// If you lose HomingOk, you are either homing, or you had an error that killed it
	// If you are homing, then you need to reset Referenced, but you don't need to clear the endless position
	// If you have an error, then you need to do the same as a ClearReference above.
	// 20180426: DO NOT CLEAR REFERENCE ON ERROR. It can cause you to lose a good reference with SafeMC, in particular.
	
//	if (!t->internal.driveStatus.HomingOk) {
		
//		if (!t->internal.readStatus.Homing && t->Referenced) {
//			t->internal.needToClearReference = 1;
//		}
		
//		t->Referenced = 0;
		
//	}

}
