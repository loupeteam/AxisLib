(*******************************************************************
* COPYRIGHT -- Automation Resources Group
********************************************************************
* Program: ServoProg
* File: ServoProgInit.st
* Author: David
* Created: March 07, 2011
********************************************************************
* Implementation of program ServoProg
********************************************************************)


PROGRAM _INIT

	// emptyPar is used to check for empty axis parameter structures
	brsmemset(ADR(emptyPar), 0, SIZEOF(emptyPar));

	// Initialize axes
	FOR i:=0 TO MAI_MOTOR BY 1 DO

		gMotorBasic[i].pAxisObject := ADR(gMotorNcm[i]);
		gMotorBasic[i].ErrorTextModuleName := 'acp10etxen';
		gMotorBasic[i].pEndlessPosition := ADR(gEndlessPosition[i]);	
		
		// CFG
		gMotorBasic[i].IN.CFG.HomingMode:=		mcHOME_DEFAULT;
			
		gMotorBasic[i].IN.CFG.StopDeceleration:=	100000;
	
		// Only initialize parameters if they are empty
		IF( brsmemcmp(ADR(gMotorBasic[i].IN.PAR), ADR(emptyPar), SIZEOF(gMotorBasic[i].IN.PAR)) = 0 )THEN
			
			// Default parameters
			gMotorBasic[i].IN.PAR.Position:=			0;
			gMotorBasic[i].IN.PAR.Distance:=			0;
			gMotorBasic[i].IN.PAR.Direction:=			mcPOSITIVE_DIR;
			gMotorBasic[i].IN.PAR.Velocity:=			9000;
			gMotorBasic[i].IN.PAR.Acceleration:=		90000;
			gMotorBasic[i].IN.PAR.Deceleration:=		90000;
	
			gMotorBasic[i].IN.PAR.JogVelocity:=		2000;
			gMotorBasic[i].IN.PAR.JogAcceleration:=	20000;
			gMotorBasic[i].IN.PAR.JogDeceleration:=	20000;
			
		END_IF
			
		// Error collector
		axisErrorCollector[i].Axis := ADR(gMotorNcm[i]);
		axisErrorCollector[i].ErrorSourceName := 'Motor ';
		appendArrayIndex(i,ADR(axisErrorCollector[i].ErrorSourceName));
		axisErrorCollector[i].DataObjectName := 'acp10etxen';
		axisErrorCollector[i].pErrorCollector := ADR(gErrorCollector);
		axisErrorCollector[i].Enable := TRUE;
		
		// Expand Limit
		axisExpandLimit[i].Axis := ADR(gMotorNcm[i]);
		
	END_FOR
	
END_PROGRAM
