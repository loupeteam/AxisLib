(********************************************************************
 * COPYRIGHT --  
 ********************************************************************
 * Program: MachineProg
 * File: PiperModuleInterface.st
 * Author: Josh
 * Created: October 03, 2013
 ********************************************************************
 * Implementation of program MachineProg
 ********************************************************************) 

ACTION PiperModuleInterface: 
	
	// Give this module a name so it is easier to debug
	ModuleInterface.ModuleName := 'Servo Manager';

	// Add a module to the Piper
	Module.ModuleInterface := ADR(ModuleInterface);
	Module.Piper := ADR(gMachine);
	Module();

	// Handle any machine states that this module needs to respond to
	CASE ModuleInterface.PiperState OF	
		
		MACH_ST_BOOTING: // Wait for axes to come on line (NetworkInit) and restore position
			groupStatus := 1;
			ModuleInterface.ModuleStatus := 'Waiting on axis:';
			
			FOR i:=0 TO MAI_MOTOR BY 1 DO
				IF Configuration.Axis[i].Active THEN

					// Stay here if NOT(NetworkInit)
					IF NOT gMotorBasic[i].OUT.DriveStatus.NetworkInit THEN
						groupStatus := 0;
						ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT('Net-', CONCAT( CONCAT(tempStr, ':'), CONCAT(gMotorBasic[i].IN.CFG.Name, ' '))));
					END_IF

					IF NOT gMotorBasic[i].OUT.DataValid AND gSimulation THEN
						gMotorBasic[i].IN.CFG.HomingMode := mcHOME_DIRECT;
						gMotorBasic[i].IN.CMD.Reference := 1;
				
					END_IF;
				
					// Leave if currently OK and 
					// no endless position is configured, data is invalid, OR homing is done
					groupStatus := groupStatus AND (	(gMotorBasic[i].pEndlessPosition = 0) 
												OR	(gMotorBasic[i].OUT.DataValid = FALSE)
												OR	(gMotorBasic[i].OUT.DriveStatus.HomingOk) );
					
				END_IF
			END_FOR				
			
			IF groupStatus THEN
				ModuleInterface.ModuleResponse:=	ModuleInterface.PiperState;
			END_IF	

		//MACH_ST_STOPPED
		
		MACH_ST_RESETTING: // Power axes. Restore Position if appropriate
			
			CASE ModuleInterface.PiperSubState OF
				
				SUB_STATE_INIT:
					ModuleInterface.ModuleSubStateRequest:=	SUB_STATE_SERVO_POWER;
				
				SUB_STATE_SERVO_POWER:
					groupStatus := 1;
					ModuleInterface.ModuleStatus := 'Waiting on axis:';
				
					FOR i:=0 TO MAI_MOTOR BY 1 DO
						IF Configuration.Axis[i].Power THEN

							gMotorBasic[i].IN.CMD.Power := 1;

							IF NOT gMotorBasic[i].OUT.DriveStatus.ControllerStatus THEN
								groupStatus := 0;
								ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[i].IN.CFG.Name, ' '));
							END_IF

						END_IF
					END_FOR				
					
					IF groupStatus THEN
					
						ModuleInterface.ModuleResponse := ModuleInterface.PiperState;	

					END_IF					
					
			END_CASE

			//MACH_ST_IDLE:
			//MACH_ST_STARTING:
			//MACH_ST_EXECUTE:
			//MACH_ST_COMPLETING:
			//MACH_ST_COMPLETE:
			//MACH_ST_HOLDING:
			//MACH_ST_HELD:
			//MACH_ST_UNHOLDING:
			//MACH_ST_SUSPENDING:
			//MACH_ST_SUSPENDED:
			//MACH_ST_UNSUSPENDING:
			
		MACH_ST_ABORTING,		
		MACH_ST_STOPPING: // Stop the axes if they are moving and power down
			
			CASE ModuleInterface.PiperSubState OF
			
				SUB_STATE_INIT:
					ModuleInterface.ModuleSubStateRequest:=	SUB_STATE_SERVO_STOP;
					
				
				SUB_STATE_SERVO_STOP:
					groupStatus := 1;
					ModuleInterface.ModuleStatus := 'Waiting on axis:';
					
					
					FOR i:=0 TO MAI_MOTOR BY 1 DO
						IF Configuration.Axis[i].Active THEN

							// Reset all move commands
							gMotorBasic[i].IN.CMD.Reference := 0;
							gMotorBasic[i].IN.CMD.MoveAbsolute := 0;
							gMotorBasic[i].IN.CMD.MoveAdditive := 0;
							gMotorBasic[i].IN.CMD.MoveVelocity := 0;
							gMotorBasic[i].IN.CMD.JogForward := 0;
							gMotorBasic[i].IN.CMD.JogReverse := 0;
							gMotorBasic[i].IN.CMD.MoveCyclicPosition := 0;
							gMotorBasic[i].IN.CMD.Halt := 0;
						
							// If you are moving, then stop
							gMotorBasic[i].IN.CMD.Stop := gMotorBasic[i].OUT.PLCOpenState.ContinuousMotion 
							OR gMotorBasic[i].OUT.PLCOpenState.DiscreteMotion 
							OR gMotorBasic[i].OUT.PLCOpenState.Homing
							OR gMotorBasic[i].OUT.PLCOpenState.SynchronizedMotion;
					
							IF gMotorBasic[i].IN.CMD.Stop OR gMotorBasic[i].OUT.PLCOpenState.Stopping THEN
								groupStatus := 0;
								ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[i].IN.CFG.Name, ' '));
							END_IF
							
						END_IF
					END_FOR				
					
					IF groupStatus THEN
						//If a drive is powered off (i.e. because of an error), we power everything off. 
						powerOffRequired := FALSE;
						FOR i:= 0 TO MAI_MOTOR BY 1 DO
							IF ((Configuration.Axis[i].Active) AND (Configuration.Axis[i].Power) AND (NOT gMotorBasic[i].OUT.DriveStatus.ControllerStatus)) THEN
								powerOffRequired := TRUE;
							END_IF
						END_FOR	
						//If we are in aborting we should also power off
						IF (ModuleInterface.PiperState = MACH_ST_ABORTING) OR (powerOffRequired) THEN
							ModuleInterface.ModuleSubStateRequest:=	SUB_STATE_SERVO_POWER_OFF;
						//If we are stopping we are done. Keep things powered
						ELSE
							ModuleInterface.ModuleResponse:=	ModuleInterface.PiperState;							
						END_IF	
					END_IF			
				
				SUB_STATE_SERVO_POWER_OFF:
					groupStatus := 1;
					ModuleInterface.ModuleStatus := 'Waiting on axis:';
					
					FOR i:=0 TO MAI_MOTOR BY 1 DO
						IF Configuration.Axis[i].Power THEN

							gMotorBasic[i].IN.CMD.Power := 0;

							IF NOT(gMotorBasic[i].OUT.PLCOpenState.Disabled) THEN
								groupStatus := 0;
								ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[i].IN.CFG.Name, ' '));
							END_IF
							
						END_IF
					END_FOR				
					
					IF groupStatus THEN
						ModuleInterface.ModuleResponse:=	ModuleInterface.PiperState;
					END_IF		
				
			END_CASE
		
		//MACH_ST_ABORTED,

		MACH_ST_CLEARING: // Check for axis ready to get out of clearing state
			groupStatus := 1;
			ModuleInterface.ModuleStatus := 'Waiting on axis:';

			FOR i:=0 TO MAI_MOTOR BY 1 DO
				IF Configuration.Axis[i].Active THEN

					IF (axisErrorCollector[i].Error
						OR NOT(gMotorBasic[i].OUT.DriveStatus.NetworkInit)
						OR NOT(gMotorBasic[i].OUT.DriveStatus.ControllerReady)
						) THEN
						groupStatus := 0;
						ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[i].IN.CFG.Name, ' '));
					END_IF

					IF (axisErrorCollector[i].AxisError) THEN
						groupStatus := 0;
						ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[i].IN.CFG.Name, ' '));
					END_IF
					
				END_IF
			END_FOR
			
						
			IF groupStatus THEN
				ModuleInterface.ModuleResponse := ModuleInterface.PiperState;
			END_IF
			
		MACH_ST_STOPPED:			
			FOR i:=0 TO MAI_MOTOR BY 1 DO
				IF gMotorBasic[i].IN.CMD.Power AND NOT gMotorBasic[i].OUT.DriveStatus.ControllerStatus THEN
					gMachine.IN.CMD.Abort := TRUE;								
				END_IF
			END_FOR
			ModuleInterface.ModuleResponse := ModuleInterface.PiperState;
			ModuleInterface.ModuleStatus := '';
			
		MACH_ST_ABORTED:			

			ModuleInterface.ModuleResponse := ModuleInterface.PiperState;
			ModuleInterface.ModuleStatus := '';
			
			FOR i:=0 TO MAI_MOTOR BY 1 DO
				gMotorBasic[i].IN.CMD.Power := 0;								
			END_FOR		
			
		ELSE 
			ModuleInterface.ModuleResponse := ModuleInterface.PiperState;
			ModuleInterface.ModuleStatus := '';
		
	END_CASE	

	
	// Check for axis errors		
	FOR i:=0 TO MAI_MOTOR BY 1 DO
		IF Configuration.Axis[i].Active THEN

			IF axisErrorCollector[i].AxisError AND gMotorBasic[i].OUT.DriveStatus.ControllerStatus THEN	
				axisErrorCollector[i].Reset := TRUE;			
				gMachine.IN.CMD.Stop := TRUE;
			ELSIF axisErrorCollector[i].AxisError AND NOT gMotorBasic[i].OUT.DriveStatus.ControllerStatus THEN	
				axisErrorCollector[i].Reset := TRUE;			
				gMachine.IN.CMD.Abort := TRUE;
			END_IF
			
		END_IF
	END_FOR
	
END_ACTION
