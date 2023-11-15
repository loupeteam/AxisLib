3.6.4 - Fix reset detection in AxisReference()

3.6.3 - Prevent Cyclic and Fast function from running while axis is not active
 
3.6.2 - Restore homing parameters in axis object after doing a home

3.6.1 - Temporary fix in Reference to fix axis home resetting when AxisExpandLimit is used.

3.6.0 - AxisExpandLimit - New function for temporarily expanding the axis position limits so the axis is within the limits

3.5.0 - Restore homing parameters in axis object after doing initial home

3.4.0 - Remove clear reference behavior when homing is lost
		Add CFG.Active and OUT.Active (straight through mapping only)
		Add CFG.Name

3.3.0 - Reinitialize reference if the drive is reset
		Tweak startup conditions to be a little safer

3.2.0 - Add PLCOpen State enum to consolidate BOOLs

3.1.0 - Make Power friendly to other Power function blocks
		Add Period and Scale
		Add IO Support
		Add ActualPositionPrecise
		Fix bug with AxisReference FUB startup if in Errorstop

3.0.0 - 20160603 -	NOT BACKWARDS COMPATIBLE
					Major refactor of AxisBasic_typ.
				 	Change to C.
					Add AxisReference FUB.
					Add AxisStatus FUB.
					
2.3.2 - 20160420 - Add data for Referenced. This is stored in the AxisBasic type, but it is set and reset by the application.

2.3.1 - 20160412 - Add data for CyclicPositions. 
					Add AxisBasicFn_Fast() for CyclicPositions.
					
2.3.0 - 20150904 - 	Fix bug in reset behavior wrt NetworkInit

2.2.0 - 20140213 -	Jog enable kept high while jogging is active.
					This fixes an issue that would cause axes not to jog if they were command too fast.
					Added AxisRestorePosition FUB.
					Reworked Jog to use MC_BR_JogVelocity FUB.
					This implies that during jogging, Busy will remain TRUE and Done will remain FALSE.
					Added AxisRestorePosition FUB to AxisBasic functionality.

2.1.0 - 20130531 -	Added Halt capability. 
					Changed JogStop to JogHalt. This allows jog commands to be resumed while decelerating.

2.0.0 - 20121109 -	THIS IS INCOMPATIBLE WITH PREVIOUS VERSIONS!
					Changed MC_Reset handling to be safe.
					Reworked handling of ReadActualPosition and ReadActualVelocity to clean up NWCT.
					Changed ErrorString from STRING[79][0..3] to STRING[329] to be more compatible with ErrorLib
					Added default values to AxisBasic_IN_PAR_typ and AxisBasic_IN_CFG_typ
					Removed ReadTorque functionality
					Completely reworked error handling to take advantage of MC_BR_ReadAxisError
					Changed behavior of MC_Power.Enable
					Removed CMDAcknowledgeErrorOld because edge detection is not necessary
					if the CMD is reset every scan.
					Reset TEST.CMD.AcknowledgeError CMD inside the function at the end.
					Reset IN.CMD.AcknowledgeError CMD inside the function at the end.
					REMOVED LADDER FN AND RENAMED AxisBasicFnST_Cyclic to AxisBasicFn_Cyclic
					Added TrackSetPosition functionality to AxisBasicFnST_Cyclic
					Added TEST interface.
					Slightly reordered AxisBasic_IN_PAR_typ.
					Removed ErrorState and State from OUT.STAT (there is no state machine)
					Added "and not(stopping)" to Jog.Execute and "and not(t.IN.CMD.Stop)" to JogStop.Execute in AxisBasicFn_Cyclic (LD).
					Added "and not(stopping)" to Jog.Execute and "and not(t.IN.CMD.Stop)" to JogStop.Execute in AxisBasicFnST_Cyclic.

1.2.0 - 20110216 -	Added RevisionHistory. 
					Changed library name from AxisMgrLib to AxisLib.
					Changed fn name from AxisMgrBasicFnCyclic to AxisBasicFn_Cyclic.
					Changed data types from AxisMgr..._typ to Axis..._typ
					Added AxisBasicFnST_Cyclic.
					