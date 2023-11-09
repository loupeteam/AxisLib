(*
* File: Types.typ
* Copyright (c) 2023 Loupe
* https://loupe.team
* 
* This file is part of AxisLib, licensed under the MIT License.
*
*)

(********************************************************************
This task defines the behavior of the axes in the system. Axes begin
motion in the excute piper state. Stop triggers a graceful stop (i.e.
finish what your doing and then stop) with the inverters still 
powered on (recoverable). Abort triggers an immediate stop of the
axes with the inverters powered off (unrecoverable). Axis errors are
collected in execute and idle and can be reset without effecting
other axes.
********************************************************************)
PROGRAM _INIT
	
	AxisConfig;
	

END_PROGRAM

PROGRAM _CYCLIC
	
	// Piper
	PiperModuleInterface;

	FOR axisIdx := 0 TO MAI_AXIS DO
		
		gMotorBasic[axisIdx].IN.PAR.Acceleration := Configuration.acceleration;
		gMotorBasic[axisIdx].IN.PAR.Deceleration := Configuration.deceleration;
		gMotorBasic[axisIdx].IN.PAR.Velocity := Configuration.velocity;
		
		// cyclically call axis basic
		AxisBasicCyclic(gMotorBasic[axisIdx], Configuration.Axis[axisIdx].cfg, task.internal.axisInternal[axisIdx]);
		
		//sim.axisMoving[axisIdx] := gMotorBasic[axisIdx].OUT.PLCOpenDiscrete.ContinuousMotion;
		
	END_FOR
	
	// reset commands
	FOR axisIdx := 0 TO MAI_AXIS DO
		task.status.axisErrorReset[axisIdx] := FALSE;
		gMotorBasic[axisIdx].IN.CMD.MoveVelocity := FALSE;
		gMotorBasic[axisIdx].IN.CMD.ErrorReset := FALSE;
	END_FOR
	
END_PROGRAM
