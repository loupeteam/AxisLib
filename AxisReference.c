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
		t->RestorePositionInitialized = 0;
		t->DataValid = 0;
		
		return;
	
	}
	
	
	if( strcmp( t->Library, "McPureVAx" ) == 0){
		t->internal.initHomeSupported = 0;
		t->RestorePositionVariableAddress = 0;
		t->HomingMode = mcHOMING_DIRECT;
	}
	else{
		t->internal.initHomeSupported = 1;
	}
	
	// Get axis status
	// Is it okay for AxisReference to depend on AxisStatus?
	t->internal.Status.Axis = t->Axis;
	t->internal.Status.Enable = !t->internal.Status.Error;
	AxisStatus(&t->internal.Status);
	
	
	// Reset internal commands
	if (!t->Reference) {
		t->internal.reference = 0;
	}
	
	if (!t->ClearReference) {
		t->internal.clearReference = 0;
	}
	
	
	// If drive is reset, reinitialize
	// TODO: Make sure this works. It probably won't. Can we just check this with PLCopen state STARTUP?
	if (t->internal.Status.CommunicationReady && !t->internal.oldCommunicationReady && (t->internal.Status.Info.AdditionalInfo.StartupCount > t->internal.oldStartupCount)) {
		t->internal.state = AXISLIB_REFST_START;
	}

	// If these don't change simultaneously this won't work
	t->internal.oldCommunicationReady = t->internal.Status.CommunicationReady;
	t->internal.oldStartupCount = t->internal.Status.Info.AdditionalInfo.StartupCount;
		
	
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
			t->RestorePositionInitialized = 0;
			t->DataValid = 0;

			// Reset FUBs
			t->internal.CheckRestorePos.Execute = 0;
			t->internal.InitHome.Execute = 0;
			t->internal.Home.Execute = 0;
			
			if (t->internal.Status.CommunicationReady 
				&& t->internal.Status.Valid
				&& !t->WaitToInitialize 
				&& ((t->internal.Status.Info.AdditionalInfo.PLCopenState == mcAXIS_DISABLED) || (t->internal.Status.Info.AdditionalInfo.PLCopenState == mcAXIS_STANDSTILL))
			) {
			
				t->Busy = 1;
			
				if( strcmp( t->Library, "McPureVAx" ) == 0){
					t->internal.state = AXISLIB_REFST_REF_HOME;
				} else if (t->RestorePositionVariableAddress != 0) {
					t->internal.state = AXISLIB_REFST_START_CHECK;
				} else {
					t->internal.state = AXISLIB_REFST_START_HOME;
				}
			}
		
			break;
		
		
		case AXISLIB_REFST_START_CHECK:
			
			// Check endless position
			// If data is valid, initialize, then home restore pos
			// If bad data, do not initialize, then direct home to DefaultPosition
	
			t->internal.CheckRestorePos.Axis = (McAxisType*)t->Axis;
			t->internal.CheckRestorePos.DataAddress = t->RestorePositionVariableAddress;
			t->internal.CheckRestorePos.Execute = 1;
		
			if (t->internal.CheckRestorePos.Done) {
		
				t->internal.CheckRestorePos.Execute = 0;
		
				t->RestorePositionInitialized = t->internal.CheckRestorePos.DataInUse;
				t->DataValid = t->internal.CheckRestorePos.DataValid;

				if (t->DataValid) {
					t->internal.state = AXISLIB_REFST_START_INIT;
				} else {
					t->internal.state = AXISLIB_REFST_START_HOME;
				}
			
			} else if (t->internal.CheckRestorePos.Error) {
		
				t->internal.CheckRestorePos.Execute = 0;
			
				t->Error = 1;
				t->ErrorID = t->internal.CheckRestorePos.ErrorID;
			
				t->RestorePositionInitialized = 0;
				t->DataValid = 0;

				t->internal.state = AXISLIB_REFST_START_HOME;
		
			}

			break;
			
		
		case AXISLIB_REFST_START_INIT:
	
			t->internal.InitHome.Axis = (McAxisType*)t->Axis;
			t->internal.InitHome.HomingParameters.RestorePositionVariableAddress = t->RestorePositionVariableAddress;
			t->internal.InitHome.HomingParameters.HomingMode = mcHOMING_RESTORE_POSITION;
			t->internal.InitHome.Execute = 1;

			if (t->internal.InitHome.Done) {
	
				t->internal.InitHome.Execute=	0;
			
				t->RestorePositionInitialized = 1;
				t->internal.state = AXISLIB_REFST_START_HOME;
		
			} else if (t->internal.InitHome.Error) {
	
				t->internal.InitHome.Execute = 0;

				t->Error = 1;
				t->ErrorID = t->internal.InitHome.ErrorID;

				t->RestorePositionInitialized = 0;
				
				t->internal.state = AXISLIB_REFST_START_HOME;
	
			}
	
			break;
		
		
		case AXISLIB_REFST_START_HOME:

			t->internal.Home.Axis = (McAxisType*)t->Axis;
			t->internal.Home.Execute = 1;
			t->internal.Home.Position = t->DefaultPosition;
			
			if (t->DataValid && t->RestorePositionInitialized) {
				t->internal.Home.HomingMode = mcHOMING_INIT;
			} else {
				t->internal.Home.HomingMode = mcHOMING_DIRECT;
			}
			
			if (t->internal.Home.Done && t->internal.Status.IsHomed) {
		
				t->internal.Home.Execute = 0;
			
				t->Busy = 0;
			
				if (t->internal.Home.HomingMode == mcHOMING_INIT && t->internal.InitHome.HomingParameters.HomingMode == mcHOMING_RESTORE_POSITION) {
					t->Referenced = 1;
				}
				
				t->internal.state = AXISLIB_REFST_IDLE;
				
			} else if (t->internal.Home.CommandAborted) {
				
				t->internal.Home.Execute = 0;
			
				t->Busy = 0;
				
				t->internal.state = AXISLIB_REFST_IDLE;
				
			} else if (t->internal.Home.Error) {
		
				t->internal.Home.Execute = 0;
			
				t->Busy = 0;
				t->Error = 1;
				t->ErrorID = t->internal.Home.ErrorID;
			
				t->internal.state = AXISLIB_REFST_IDLE;
		
			}
		
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
			t->internal.CheckRestorePos.Execute = 0;
			t->internal.InitHome.Execute = 0;
			t->internal.Home.Execute = 0;
		
			// Check for commands
			if (t->Reference && !t->internal.reference) {
				
				t->internal.reference = 1;
				
				t->Done = 0;
				t->Busy = 1;
				t->CommandAborted = 0;
				t->Error = 0;
				t->ErrorID = 0;
				t->Referenced = 0;
				
				t->internal.state = AXISLIB_REFST_REF_INIT;
		
			} else if ((t->ClearReference && !t->internal.clearReference) || t->internal.needToClearReference) {
				
				t->internal.clearReference = 1;
				t->internal.needToClearReference = 0;

				if (t->RestorePositionVariableAddress != 0) {
					
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
			
			if( t->internal.initHomeSupported ){
				
				t->internal.InitHome.Axis = (McAxisType*)t->Axis;
				t->internal.InitHome.HomingParameters.RestorePositionVariableAddress = t->RestorePositionVariableAddress;
				t->internal.InitHome.HomingParameters.HomingMode = t->HomingMode;
				t->internal.InitHome.HomingParameters.Position = t->Position;
				// TODO: Make sure all relevant homing parameters are set for whatever homing mode we may be using
				t->internal.InitHome.Execute = 1;

				if (t->internal.InitHome.Done) {
	
					t->internal.InitHome.Execute = 0;
			
					if (t->HomingMode == mcHOMING_RESTORE_POSITION) {
						t->RestorePositionInitialized = 1;
					}
				
					t->internal.state = AXISLIB_REFST_REF_HOME;
		
				} else if (t->internal.InitHome.Error) {
	
					t->internal.InitHome.Execute = 0;

					t->Error = 1;
					t->ErrorID = t->internal.InitHome.ErrorID;
					t->RestorePositionInitialized = 0;
				
					t->internal.state = AXISLIB_REFST_IDLE;
	
				}
			}
			else{
				t->internal.state = AXISLIB_REFST_REF_HOME;
			}
			break;
		
		
		case AXISLIB_REFST_REF_HOME:

			t->internal.Home.Axis = (McAxisType*)t->Axis;
			t->internal.Home.Execute = 1;

			if( t->internal.initHomeSupported ){
				t->internal.Home.HomingMode = mcHOMING_INIT;
			}
			else{
				t->internal.Home.HomingMode = 	t->HomingMode;
				t->internal.Home.Position =		t->Position;
			}
			
			if (t->internal.Home.Done && t->internal.Status.IsHomed) {
		
				t->internal.Home.Execute = 0;
			
				t->Done = 1;
				t->Busy = 0;
				t->Referenced = 1;
				t->DataValid = t->RestorePositionInitialized;
								
				t->internal.state = AXISLIB_REFST_IDLE;
				
			} else if (t->internal.Home.CommandAborted) {
				
				t->internal.Home.Execute = 0;
				
				t->CommandAborted = 1;
				t->Busy = 0;
				t->Referenced = 0;
				
				t->internal.state = AXISLIB_REFST_IDLE;
				
			} else if (t->internal.Home.Error) {
		
				t->internal.Home.Execute = 0;
			
				t->Error = 1;
				t->ErrorID = t->internal.Home.ErrorID;
				t->Busy = 0;
				t->Referenced = 0;
			
				t->internal.state = AXISLIB_REFST_IDLE;
		
			}
		
			break;
		
		
		case AXISLIB_REFST_CLEAR_INIT:
		
			if( t->internal.initHomeSupported ){

				// Init to internal and clear external to REALLY clear the reference
				t->internal.InitHome.Axis = (McAxisType*)t->Axis;
				t->internal.InitHome.HomingParameters.RestorePositionVariableAddress = (UDINT)&(t->internal.restorePos);
				t->internal.InitHome.HomingParameters.HomingMode = mcHOMING_RESTORE_POSITION;
				t->internal.InitHome.Execute = 1;

				if (t->internal.InitHome.Done) {
	
					t->internal.InitHome.Execute = 0;
			
					if (t->RestorePositionVariableAddress != 0) {
						memset((void*)t->RestorePositionVariableAddress, 0, sizeof(McAcpAxRestorePosType));
					}
				
					t->Done = 1;
					t->Busy = 0;
					t->Referenced = 0;
					t->RestorePositionInitialized = 0;
					t->DataValid = 0;
				
					t->internal.state = AXISLIB_REFST_IDLE;
		
				} else if (t->internal.InitHome.Error) {
	
					t->internal.InitHome.Execute = 0;

					t->Error = 1;
					t->ErrorID = t->internal.InitHome.ErrorID;
					t->Busy = 0;
					t->Referenced = 0;
					t->RestorePositionInitialized = 0;
					t->DataValid = 0;
				
					t->internal.state = AXISLIB_REFST_IDLE;
	
				}
			}
			else{
				t->Referenced = 0;
			}
			break;
		
		
	} // switch (state)
	
	// Call FUBs
	// TODO: check if we need to reset some FUB inputs at some point
	// I think all the important items are written on-use, but I'm not sure
	MC_BR_CheckRestorePositionData(&t->internal.CheckRestorePos);
	MC_BR_InitHome_AcpAx(&t->internal.InitHome);
	MC_Home(&t->internal.Home);
	
	// If you lose HomingOk, you are either homing, or you had an error that killed it
	// If you are homing, then you need to reset Referenced, but you don't need to clear the endless position
	// If you have an error, then you need to do the same as a ClearReference above.
	// 20180426: DO NOT CLEAR REFERENCE ON ERROR. It can cause you to lose a good reference with SafeMC, in particular.
	
	if (!t->internal.Status.IsHomed) {
		
		//		if (!t->internal.readStatus.Homing && t->Referenced) {
		//			t->internal.needToClearReference = 1;
		//		}
		
		t->Referenced = 0;
		
	}

}
