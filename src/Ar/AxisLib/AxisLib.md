![Automation Resources Group](http://automationresourcesgroup.s3.amazonaws.com/ARGLogos/arg-logo-245.png)

# AxisLib Library
The AxisLib library provides a clean interface for basic motion control.

Every motion axis has a set of basic functionality in common. This includes powering and homing the axis, performing basic moves, and reading back status information. In addition to this basic functionality, every axis has its own set of specific functionality. Since it is impossible to make one piece of software that can handle both the common and specific functionality of any arbitrary axis, the software that handles common functionality must not interfere with axis-specific functionality.

The AxisLib library bundles the most common motion control commands and status information together into one clean interface. This leads to significantly faster development of motion applications. Also, the library is designed to work in parallel with any other motion functionality. This allows any application specific motion code to be layered on top of the basic functionality while still taking advantage of all the library has to offer.

# Usage
The AxisLib functionality can be integrated into any project using a data structure and a function call. For an example of how to use this in a project, please see the ARG Automation Studio Starter Project at [https://github.com/autresgrp/StarterProject](https://github.com/autresgrp/StarterProject).

## Initialization
The AxisLib library provides an interface to the acp10mc library, commonly referred to as PLCOpen motion control. This means that all of the steps to create a motion axis must still be followed. For details on this, please see the B&R motion control training manuals or the Automation Studio Online Help.

To use the AxisLib functionality, a variable must be declared of type **AxisBasic_typ**. This variable should then be initialized in the INIT routine of your program. This initialization consists of setting configuration settings and initial motion parameters.

	BasicAxis.pAxisObject:=			ADR(gNCaxis);
	BasicAxis.ErrorTextModuleName:=	'acp10etxen';
	
	BasicAxis.IN.CFG.HomingPosition:=	0;
	BasicAxis.IN.CFG.HomingMode:=		mcHOME_DEFAULT;
	
	BasicAxis.IN.PAR.Position:=			0;
	BasicAxis.IN.PAR.Distance:=			0;
	BasicAxis.IN.PAR.Direction:=		mcPOSITIVE_DIR;
	BasicAxis.IN.PAR.Velocity:=			1000;
	BasicAxis.IN.PAR.Acceleration:=		10000;
	BasicAxis.IN.PAR.Deceleration:=		10000;
	
	BasicAxis.IN.PAR.JogVelocity:=		100;
	BasicAxis.IN.PAR.JogAcceleration:=	10000;
	BasicAxis.IN.PAR.JogDeceleration:=	10000;
	
	BasicAxis.IN.CFG.StopDeceleration:=	10000;

The **pAxisObject** configuration setting contains the address of the ACP10AXIS_typ variable listed in the NC mapping table. This creates the link between the BasicAxis variable and the physical axis. Please see the motion training manuals or the Automation Studio Online Help for more information on NC mapping tables.

## Cyclic Operation
To control the axis cyclically, the **AxisBasicFn_Cyclic()** function must be called in the CYCLIC routine of your program, once every scan, unconditionally. This function should not be called on the same BasicAxis variable more than once per scan.

	AxisBasicFn_Cyclic( BasicAxis );

The BasicAxis can then be used as an interface for higher level motion sequencing programs.

	CASE State OF
	
		ST_IDLE:
	
			BasicAxis.IN.CMD.Power:=	0;
	
			IF( Start )THEN
	
	  			Start:=	0;
	
	  			IF( 		BasicAxis.OUT.DriveStatus.ControllerReady
	   				AND NOT	BasicAxis.OUT.DriveStatus.ControllerStatus
					)THEN
					
					State:=	ST_POWERON;
	
				END_IF
	
			END_IF
			
		ST_POWERON:
		
			BasicAxis.IN.CMD.Power:=	1;
					
			IF( BasicAxis.OUT.DriveStatus.ControllerStatus )THEN
	  
				State:=	ST_HOME;
				
			END_IF
			
		ST_HOME:
		
			BasicAxis.IN.CMD.Reference:=	1;
			
			IF( 	BasicAxis.OUT.Done
	  			AND	BasicAxis.OUT.DriveStatus.HomingOk
	   			)THEN						
			
				BasicAxis.IN.CMD.Reference:=	0;
			
				State:=	ST_JOG;
				
			END_IF
		
		ST_JOG:
		
			BasicAxis.IN.CMD.JogForward:=	1;
				
			IF( Stop )THEN
	  			
				Stop:=	0;

	  			BasicAxis.IN.CMD.JogForward:=	0;
			
				State:=	ST_IDLE;
				
			END_IF
					
	END_CASE

# Reference

## BasicAxis Data Structure
The main data structure of the AxisLib library is the BasicAxis structure (AxisBasic_typ datatype). This provides the interface to higher level programs and also stores all necessary internal information. It is divided into inputs (BasicAxis.IN), outputs (BasicAxis.OUT), a test interface (BasicAxis.TEST) and internals (BasicAxis.Internal).

### Inputs
The BasicAxis inputs are divided into commands (IN.CMD), parameters (IN.PAR), and configuration settings (IN.CFG). Commands are used to initiate operations, and parameters and configuration settings determine how the commands will be processed. The difference between parameters and configuration settings is that configuration settings are generally set only once, while parameters might be set any time a command is issued.

#### Commands
* **Power** - Power the axis. If set to 1, the axis will be powered. If set to 0, the axis will be depowered.
* **Reference** - Home the axis. Uses the **HomingPosition** and **HomingMode** parameters.
* **MoveAbsolute** - Perform a move to an absolute position. Uses the **Position**, **Velocity**, **Acceleration**, **Deceleration**, and **Direction** parameters.
* **MoveAdditive** - Perform an additive move. The **Distance** parameter is added to the current target position (NOT the current actual position). Uses the **Distance**, **Velocity**, **Acceleration**, and **Deceleration** parameters.
* **MoveVelocity** - Perform a velocity move. The axis accelerates to the **Velocity** parameter and continues until another command is issued. If a new velocity is desired, the **MoveVelocity** command must be toggled. Uses the **Velocity**, **Acceleration**, **Deceleration**, and **Direction** parameters.
* **JogForward** - Jog the axis forward. The axis will stop when this command is reset. Uses the **JogVelocity**, **JogAcceleration**, and **JogDeceleration** parameters.
* **JogReverse** - Jog the axis backwards. The axis will stop when this command is reset. Uses the **JogVelocity**, **JogAcceleration**, and **JogDeceleration** parameters.
* **TrackSetPosition** - Track the input position. When set to 1, the axis will continually track the input position. Uses the **Position**, **Velocity**, **Acceleration**, and **Deceleration** parameters.
* **Stop** - Stops the axis. When set to 1, all other motion commands will be ignored and the axis will remain stopped. Uses the **StopDeceleration** parameter.
* **AcknowledgeError** - Acknowledge any errors or warnings on the axis.

#### Parameters
* **HomingPosition** - Actual position after homing the axis. [Units]
* **HomingMode** - Mode with which to home the axis. See the acp10mc library help for details.
* **Position** - Position for absolute moves and for tracking set position. [Units]
* **Distance** - Distance for additive moves. [Units]
* **Velocity** - Velocity for absolute, additive, and velocity moves, and for tracking set positions. [Units/s]
* **Acceleration**, **Deceleration** - Acceleration and deceleration for absolute, additive, and velocity moves, and for tracking set positions. [Units/s^2]
* **Direction** - Direction for absolute and velocity moves. See the acp10mc library help for details.
* **JogVelocity** - Velocity for jog moves. [Units/s]
* **JogAcceleration**, **JogDeceleration** - Acceleration and declaration for jog moves. [Units/s^2]
* **StopDeceleration** - Deceleration for stopping. [Units/s^2]

#### Configuration Settings
* **pAxisObject** - Address of the ACP10AXIS_typ variable listed in the NC mapping table. See the motion training manuals or the Automation Studio Online Help for details.
* **ErrorTextModuleName** - Name of the error text module for looking up error texts. See the motion training manuals or the Automation Studio Online Help for details.

### Outputs
The BasicAxis outputs contain status information (OUT.STAT).

* **ActualPosition** - Current axis position. [Units]
* **ActualVelocity** - Current axis velocity. [Units/s]
* **DriveStatus** - Extended drive status information. See the help for the acp10mc library MC_DRIVESTATUS_TYP datatype for details.
* **PLCOpenState** - Current PLCOpen state as booleans. See the motion training manuals or the Automation Studio Online Help for details.
* **Busy** - Operation is currently being processed.
* **Done** - Operation completed successfully. **Done** is reset when the input command is reset.
* **Warning** - A warning exists for the axis. **Warning** is reset with the **AcknowledgeError** command.
* **WarningCount** - Number of current warnings.
* **Error** - An error exists for the axis. **Error** is reset with the **AcknowledgeError** command.
* **ErrorCount** - Number of current errors.
* **ErrorID** - Current error ID number.
* **ErrorString** - Current error text information.

### Test Interface
The test interface gives you direct access to the axis commands and parameters, interrupting any commands from higher level programs. This allows for easy testing of the axis during machine commissioning or servicing. **The test commands and parameters are intended for use in an Automation Studio watch window and SHOULD NOT BE SET IN PROGRAMS**.

* **Enable** - If set to 1, TEST.CMD and TEST.PAR take priority over IN.CMD and IN.PAR. If set to 0, then TEST.CMD and TEST.PAR are ignored.
* **CMD** - Test commands. See above for a full list of commands.
* **PAR** - Test parameters. See above for a full list of parameters.
* **STAT** - A reduced set of status information that is always available whether test mode is enabled or not.

## Error ID Numbers
For a full list of motion errors and their descriptions, please see the Automation Studio Online Help.
