//--------------------------------------------------------------------------------
// File: AxisRestorPosition.c
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

//------------------------------------------------------------------------
// DEPRECATED: Handle endless position (Init, Check, Restore) for an axis
//------------------------------------------------------------------------

// C version will not be tested until someone has a problem. 
// This function is DEPRECATED.

void AxisRestorePosition(struct AxisRestorePosition* t)
{
	
	if (t == 0) return;
	
	switch (t->Internal.State) {

		case AXISLIB_RPST_IDLE:

			// Reset outputs 
			t->Busy = 0;
			t->Done = 0;
			t->Error = 0;
			t->ErrorID = 0;

			t->EndlessPositionInitialized = 0;
			t->DataValid = 0;

			// Reset FUBs 
			t->Internal.InitEndlessPos.Execute = 0;
			t->Internal.CheckEndlessPos.Execute = 0;
			t->Internal.Home.Execute = 0;
		
			// Check for Execute 
			if (t->Execute) {

				t->Busy =	1;
			
				t->Internal.InitEndlessPos.Axis = t->Axis;
				t->Internal.InitEndlessPos.DataAddress = t->DataAddress;
				t->Internal.InitEndlessPos.Execute = 1;
		
				t->Internal.State =	AXISLIB_RPST_INIT;
		
			}
		
			break;
		

		case AXISLIB_RPST_INIT:
	
			if (t->Internal.InitEndlessPos.Done) {
		
				t->Internal.InitEndlessPos.Execute = 0;
			
				t->Internal.CheckEndlessPos.Axis = t->Axis;
				t->Internal.CheckEndlessPos.DataAddress = t->DataAddress;
				t->Internal.CheckEndlessPos.Execute = 1;
		
				t->Internal.State =	AXISLIB_RPST_CHECK;
	
			} else if (t->Internal.InitEndlessPos.Error) {
	
				t->Internal.InitEndlessPos.Execute = 0;

				t->Busy = 0;
				t->Done = 0;
				t->Error = 1;
				t->ErrorID = t->Internal.InitEndlessPos.ErrorID;

				t->EndlessPositionInitialized = 0;
				t->DataValid = 0;

				t->Internal.State =	AXISLIB_RPST_DONE;
	
			}

			break;
		

		case AXISLIB_RPST_CHECK:
	
			if (t->Internal.CheckEndlessPos.Done) {
		
				t->Internal.CheckEndlessPos.Execute = 0;
		
				t->EndlessPositionInitialized = t->Internal.CheckEndlessPos.EndlessPositionInitialized;
				t->DataValid = t->Internal.CheckEndlessPos.DataValid;

				if (t->EndlessPositionInitialized && t->DataValid) {
			
					t->Internal.Home.Axis = t->Axis;
					t->Internal.Home.Execute = 1;
					t->Internal.Home.HomingMode = mcHOME_RESTORE_POS;
	
					t->Internal.State =	AXISLIB_RPST_HOME;
				
				} else {
			
					t->Busy = 0;
					t->Done = 1;
					t->Error = 0;
					t->ErrorID = 0;
				
					t->Internal.State =	AXISLIB_RPST_DONE;
				
				}
			
			} else if (t->Internal.CheckEndlessPos.Error) {
		
				t->Internal.CheckEndlessPos.Execute = 0;
			
				t->Busy = 0;
				t->Done = 0;
				t->Error = 1;
				t->ErrorID = t->Internal.CheckEndlessPos.ErrorID;
			
				t->EndlessPositionInitialized = 0;
				t->DataValid = 0;

				t->Internal.State =	AXISLIB_RPST_DONE;
			
			}

			break;
		

		case AXISLIB_RPST_HOME:

			if (t->Internal.Home.Done) {
		
				t->Internal.Home.Execute = 0;
			
				t->Busy = 0;
				t->Done = 1;
				t->Error = 0;
				t->ErrorID = 0;
			
				t->Internal.State =	AXISLIB_RPST_DONE;
		
			} else if (t->Internal.Home.Error) {
		
				t->Internal.Home.Execute = 0;
			
				t->Busy = 0;
				t->Done = 0;
				t->Error = 1;
				t->ErrorID = t->Internal.Home.ErrorID;
			
				t->Internal.State =	AXISLIB_RPST_DONE;
		
			}
		
			break;
		

		case AXISLIB_RPST_DONE:
			
			// Reset FUBs 
			t->Internal.InitEndlessPos.Execute = 0;
			t->Internal.CheckEndlessPos.Execute = 0;
			t->Internal.Home.Execute = 0;
		
			// Check for !Execute
			if (!t->Execute) {
		
				t->Busy = 0;
				t->Done = 0;
				t->Error = 0;
				t->ErrorID = 0;
			
				t->EndlessPositionInitialized = 0;
				t->DataValid = 0;
			
				t->Internal.State =	AXISLIB_RPST_IDLE;
			
			}
		
			break;
		

	}

	// Call FUBs
	MC_BR_InitEndlessPosition(&t->Internal.InitEndlessPos);
	MC_BR_CheckEndlessPosition(&t->Internal.CheckEndlessPos);
	MC_Home(&t->Internal.Home);
	
}
