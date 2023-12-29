//--------------------------------------------------------------------------------
// File: AxisBasicFn_Fast.c
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

//----------------------------------------------------------------
// Manage setting and reading cyclic positions. 
// Intended to run in a faster task class than most motion tasks.
//----------------------------------------------------------------

// Function - good

plcbit AxisBasicFn_Fast(struct AxisBasic_typ* t)
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
		// Not right now
		t->OUT.Error = 1;
		t->OUT.ErrorCount = 1;
		t->OUT.ErrorID = 29200;
		strcpy(t->OUT.ErrorString, "pAxisObject is 0. No function possible.");
		
		return 0;
	
	}
	
	// Set useFastFunction to signal we are calling this function
	t->Internal.useFastFunction = 1;
	
	axisInternalFastFn(t);

	return 0;
	
}
