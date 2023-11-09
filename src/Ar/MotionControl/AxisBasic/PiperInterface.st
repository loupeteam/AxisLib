(*
* File: Types.typ
* Copyright (c) 2023 Loupe
* https://loupe.team
* 
* This file is part of AxisLib, licensed under the MIT License.
*
*)

(*
The intent for this piper implementation is to power on and move velocity
all the axes in execute, respond to errors in execute without the need for
system wide abort/reset, and allow enabling/disabling of axes at runtime.
*)
ACTION PiperModuleInterface: 
	
	//Give this module a name so it is easier to debug
	ModuleInterface.ModuleName:=	'AxisMgr';
	// cyclically clear ModuleStatus so we don't have to worry about clearing it when states change
	brsmemset(ADR(ModuleInterface.ModuleStatus), 0, SIZEOF(ModuleInterface.ModuleStatus));

	//Add a module to the Piper
	Module.ModuleInterface:=	ADR(ModuleInterface);
	Module.Piper:= 	ADR(gMachine);
	Module();

	// Handle any machine states that this module needs to respond to
	CASE ModuleInterface.PiperState OF
		MACH_ST_BOOTING: // Wait for axes to come on line (CommunicationReady) and restore position
			
			CASE ModuleInterface.PiperSubState OF
			
				SUB_STATE_INIT:
					ModuleInterface.ModuleSubStateRequest := SUB_STATE_BOOTING_COMM_READY;
					
				SUB_STATE_BOOTING_COMM_READY:
					groupStatus := TRUE;
					ModuleInterface.ModuleStatus := 'Waiting on axis:';
								
					FOR axisIdx := 0 TO MAI_AXIS DO
						IF Configuration.Axis[axisIdx].cfg.Active THEN
							IF gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Errorstop THEN
								// let axis pass through
							ELSIF NOT gMotorBasic[axisIdx].OUT.State.CommunicationReady THEN
								groupStatus := FALSE;
								ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[axisIdx].OUT.Name,' '));
							END_IF
						END_IF
					END_FOR	
					
					IF groupStatus THEN
						ModuleInterface.ModuleResponse := ModuleInterface.PiperState;
					END_IF
		
			END_CASE
			
		MACH_ST_CLEARING:
			groupStatus := TRUE;
			FOR axisIdx := 0 TO MAI_AXIS DO
				IF gMotorBasic[axisIdx].OUT.ErrorCount > 0 THEN
					groupStatus := FALSE;
					gMotorBasic[axisIdx].IN.CMD.ErrorReset := TRUE;	
				END_IF
			END_FOR
			
			IF groupStatus THEN
				ModuleInterface.ModuleResponse := ModuleInterface.PiperState;
			END_IF
			
		//MACH_ST_STOPPED:		
		//MACH_ST_STARTING:	
		MACH_ST_IDLE,
		//MACH_ST_SUSPENDED:
//		MACH_ST_EXECUTE:
//			
//			FOR axisIdx := 0 TO MAI_AXIS DO
//				
//				IF gMotorBasic[axisIdx].OUT.ErrorCount > 0 THEN
//					task.status.state[axisIdx] := ST_ERROR;
//				END_IF
//			
//				CASE task.status.state[axisIdx] OF
//				
//					ST_POWER_ON:
//				
//						gMotorBasic[axisIdx].IN.CMD.Power := TRUE;
//						IF gMotorBasic[axisIdx].OUT.State.PowerOn AND (gMachine.OUT.State = MACH_ST_EXECUTE) THEN
//							task.status.state[axisIdx] := ST_MOVE_VELOCITY;
//						END_IF
//				
//					ST_MOVE_VELOCITY:
//				
//						IF Configuration.Axis[axisIdx].cfg.Active AND Configuration.Axis[axisIdx].power THEN
//							gMotorBasic[axisIdx].IN.CMD.MoveVelocity := TRUE;
//						END_IF
//						
//					ST_ERROR:
//						
//						task.status.axisError[axisIdx] := TRUE;
//						gMotorBasic[axisIdx].IN.CMD.ErrorReset := task.status.axisErrorReset[axisIdx];
//						IF gMotorBasic[axisIdx].OUT.ErrorCount = 0 THEN
//							task.status.state[axisIdx] := ST_POWER_ON;
//						END_IF
//					
//				END_CASE
//				
//			END_FOR
			
		MACH_ST_STOPPING:
			
			CASE ModuleInterface.PiperSubState OF
				
				SUB_STATE_INIT:
					ModuleInterface.ModuleSubStateRequest := SUB_STATE_STOP;
									
				SUB_STATE_STOP:
					groupStatus := TRUE;
					ModuleInterface.ModuleStatus := 'Stopping axis:';
										
					FOR axisIdx := 0 TO MAI_AXIS DO
						IF Configuration.Axis[axisIdx].cfg.Active AND Configuration.Axis[axisIdx].power THEN
							gMotorBasic[axisIdx].IN.CMD.Stop := gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.ContinuousMotion
																OR gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.DiscreteMotion
																OR gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Homing
																OR gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.SynchronizedMotion;
							IF gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Errorstop THEN
								// let axis pass through
							ELSIF gMotorBasic[axisIdx].IN.CMD.Stop OR gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Stopping THEN
								groupStatus := FALSE;
								ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[axisIdx].OUT.Name,' '));
							END_IF
						END_IF
					END_FOR	
							
					IF groupStatus THEN
						ModuleInterface.ModuleResponse := ModuleInterface.PiperState;
					END_IF
					
			END_CASE
			
		MACH_ST_ABORTING:
			
			CASE ModuleInterface.PiperSubState OF
				
				SUB_STATE_INIT:
					ModuleInterface.ModuleSubStateRequest := SUB_STATE_STOP;
									
				SUB_STATE_STOP:
					groupStatus := TRUE;
					ModuleInterface.ModuleStatus := 'Stopping axis:';
										
					FOR axisIdx := 0 TO MAI_AXIS DO
						IF Configuration.Axis[axisIdx].cfg.Active AND Configuration.Axis[axisIdx].power THEN
							gMotorBasic[axisIdx].IN.CMD.Stop := gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.ContinuousMotion
																OR gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.DiscreteMotion
																OR gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Homing
																OR gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.SynchronizedMotion;
							IF gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Errorstop THEN
								// let axis pass through
							ELSIF gMotorBasic[axisIdx].IN.CMD.Stop OR gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Stopping THEN
								groupStatus := FALSE;
								ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[axisIdx].OUT.Name,' '));
							END_IF
						END_IF
					END_FOR	
							
					IF groupStatus THEN
						ModuleInterface.ModuleSubStateRequest := SUB_STATE_ABORTING_POWER_OFF;
					END_IF
				
				SUB_STATE_ABORTING_POWER_OFF:
					groupStatus := TRUE;
					ModuleInterface.ModuleStatus := 'Powering Off axis:';
										
					FOR axisIdx := 0 TO MAI_AXIS DO
						IF Configuration.Axis[axisIdx].power THEN
							gMotorBasic[axisIdx].IN.CMD.Power := FALSE;
							IF gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Errorstop THEN
								// let axis pass through
							ELSIF NOT gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Disabled THEN
								groupStatus := FALSE;
								ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[axisIdx].OUT.Name,' '));
							END_IF
						END_IF
					END_FOR	
							
					IF groupStatus THEN
						ModuleInterface.ModuleResponse := ModuleInterface.PiperState;
					END_IF
				
			END_CASE
			
		//MACH_ST_ABORTED:
		//MACH_ST_HOLDING:
		//MACH_ST_HELD:
		//MACH_ST_UNHOLDING:
		//MACH_ST_SUSPENDING:
		//MACH_ST_UNSUSPENDING:
		MACH_ST_RESETTING:
			
			CASE ModuleInterface.PiperSubState OF
				
				SUB_STATE_INIT:
					ModuleInterface.ModuleSubStateRequest := SUB_STATE_RESETTING_POWER_ON;
							
				SUB_STATE_RESETTING_POWER_ON:
					groupStatus := TRUE;
					ModuleInterface.ModuleStatus := 'Powering on axis:';
										
					FOR axisIdx := 0 TO MAI_AXIS DO
						task.status.state[axisIdx] := ST_POWER_ON;
						IF Configuration.Axis[axisIdx].cfg.Active AND Configuration.Axis[axisIdx].power THEN
							IF gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.Errorstop THEN
								// let axis pass through
							ELSIF NOT gMotorBasic[axisIdx].OUT.State.PowerOn THEN
								groupStatus := FALSE;
								gMotorBasic[axisIdx].IN.CMD.Power := TRUE;
								ModuleInterface.ModuleStatus := CONCAT(ModuleInterface.ModuleStatus, CONCAT(gMotorBasic[axisIdx].OUT.Name,' '));
							END_IF
						END_IF
					END_FOR	
							
					IF groupStatus THEN
						ModuleInterface.ModuleResponse := ModuleInterface.PiperState;
					END_IF
				
			END_CASE
			
		//MACH_ST_COMPLETING:
		//MACH_ST_COMPLETE:		
		//MACH_ST_BYPASSED:
		
		ELSE
			ModuleInterface.ModuleResponse:=	ModuleInterface.PiperState;
		
	END_CASE
	
END_ACTION