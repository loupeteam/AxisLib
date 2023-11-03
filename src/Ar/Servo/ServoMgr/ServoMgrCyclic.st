(*
* File: ServoMgrCyclic.st
* Copyright (c) 2023 Loupe
* https://loupe.team
* 
* This file is part of ServoMgr, licensed under the MIT License.
*)

PROGRAM _CYCLIC
	
	// Call PiperModuleInterface to interface with the rest of the machine
	PiperModuleInterface;
	
	// Call AxisBasicFn_Cyclic to control the axis
	FOR i:=0 TO MAI_MOTOR BY 1 DO
		
		IF Configuration.Axis[i].Active THEN

			IF homeAll THEN				
				gMotorBasic[i].IN.CMD.Reference :=1;
			END_IF
							
			AxisBasicFn_Cyclic(gMotorBasic[i]);
			// Reset edge-sensitive gAxis commands set by HMI
			gMotorBasic[i].IN.CMD.MoveAbsolute := FALSE;
			gMotorBasic[i].IN.CMD.MoveAdditive := FALSE;
			gMotorBasic[i].IN.CMD.Halt := FALSE;
			gMotorBasic[i].IN.CMD.Stop := FALSE;
			gMotorBasic[i].IN.CMD.ClearReference := FALSE;
			gMotorBasic[i].IN.CMD.Reference := FALSE;
			// Error collection
			axisErrorCollector[i]();	
			
			axisHmi.axis[i].active:=	gMotorBasic[i].OUT.Active;
			axisHmi.axis[i].powered:=	gMotorBasic[i].OUT.DriveStatus.ControllerStatus;
			axisHmi.axis[i].referenced:=gMotorBasic[i].OUT.Referenced;		

		END_IF	

	END_FOR

	homeAll:=0;
	
	//Homing check
	homeCheck(CLK := (gMachine.OUT.State = MACH_ST_RESETTING));
	IF homeCheck.Q THEN
		FOR i:=0 TO MAI_MOTOR BY 1 DO
			IF Configuration.Axis[i].Power AND NOT gMotorBasic[i].OUT.Referenced THEN
				errcolAddError(ADR(axisErrorCollector[i].ErrorSourceName),ADR('Axis not referenced'),0,0,gErrorCollector);								
			END_IF

		END_FOR
	END_IF
	
END_PROGRAM
