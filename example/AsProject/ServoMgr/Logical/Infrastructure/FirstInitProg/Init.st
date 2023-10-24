(*******************************************************************
* COPYRIGHT --  
********************************************************************
* PROGRAM: FirstInitProg
* File: FirstInitProgInit.c
* Author: ScismD
* Created: January 07, 2014
********************************************************************
* Implementation OF PROGRAM FirstInitProg
*******************************************************************)

PROGRAM _INIT
	// Check for simulation
	// This is a DevLink using a Windows share which is not supported by ARsim
	// We use this instead of Diag function to support older CPU's
	simCheckDevLink.enable := TRUE;
	simCheckDevLink.pDevice := ADR('SimCheck');
	simCheckDevLink.pParam := ADR('/SIP=127.0.0.1 /PROTOCOL=cifs /SHARE=testfolder /USER=max /PASSWORD=passw');
	
	simCheckDevLink();
	
	IF (simCheckDevLink.status = ERR_NOTIMPLEMENTED) THEN
		gSimulation := TRUE;
	END_IF
	
	// Initialize User Loggers
	createLogInit('Errors', 1000000, arEVENTLOG_PERSISTENCE_VOLATILE);	
	createLogInit('Files', 1000000, arEVENTLOG_PERSISTENCE_VOLATILE);	
	createLogInit('Application', 1000000, arEVENTLOG_PERSISTENCE_VOLATILE);	
	
	// Initialize error collector
	ErrorCollectorFn_Init(gErrorCollector);

	// Link file devices
	//-------------------------------------------------------
	
	// This is NOT done in the CPU configuration because when using CPU simulation
	// you cannot have separate file devices for simulation mode,
	// and you cannot unlink devices that were entered in the CPU configuration

	// Set up file device names
	brsstrcpy(ADR(fileDevice[0]), ADR('CF_User'));
	brsstrcpy(ADR(fileDevice[1]), ADR('CNC_PrgDir'));
	brsstrcpy(ADR(fileDevice[2]), ADR('Recipes'));
	brsstrcpy(ADR(fileDevice[3]), ADR('permBackup'));
	
	// Set up file device paths
	IF (gSimulation) THEN
		brsstrcpy(ADR(directory[0]), ADR('C:\\ARSimUser\\LamResearch_MiniCell'));
		brsstrcpy(ADR(directory[1]), ADR('C:\\ARSimUser\\LamResearch_MiniCell\\CNC'));
		brsstrcpy(ADR(directory[2]), ADR('C:\\ARSimUser\\LamResearch_MiniCell\\Recipes'));
		brsstrcpy(ADR(directory[3]), ADR('C:\\ARSimUser\\LamResearch_MiniCell\\PermData'));
	ELSE
		brsstrcpy(ADR(directory[0]), ADR('F:\\'));
		brsstrcpy(ADR(directory[1]), ADR('F:\\CNC'));
		brsstrcpy(ADR(directory[2]), ADR('F:\\Recipes'));
		brsstrcpy(ADR(directory[3]), ADR('F:\\PermData'));
	END_IF
	
	// Link file devices
	FOR i:=0 TO MAI_FILEDEVICES BY 1 DO 
		
		brsstrcpy(ADR(parameters[i]), ADR('/DEVICE='));
		stringlcat(ADR(parameters[i]), ADR(directory[i]), SIZEOF(parameters[i]));
		
		fileDeviceDevLink[i].enable := TRUE;
		fileDeviceDevLink[i].pDevice := ADR(fileDevice[i]);
		fileDeviceDevLink[i].pParam := ADR(parameters[i]);
		
		fileDeviceDevLink[i]();
		
		IF (fileDeviceDevLink[i].status <> 0) THEN
			errcolAddError(ADR('File Devices'), ADR('Error linking file device'), fileDeviceDevLink[i].status, 0, gErrorCollector);
		END_IF
		
	END_FOR
	
	// Initialize Persisters
	//-------------------------------------------------------
	
	// Working variables
	
	// Machine configuration
	foundVars := varGetVariableList(ADR('Configuration'), ADR(gPersister[PERM_CONFIGURATION].IN.WorkingVariableList));
	
	// Home data
	brsstrcpy(ADR(gPersister[PERM_HOME_DATA].IN.WorkingVariableList[0]),ADR('gEndlessPosition'));
	
	// Axis pars
	// Verify that NUM_MOTOR is defined by ServoControl if error occurs here
	FOR i := 0 TO MAI_MOTOR BY 1 DO
		brsitoa(i, ADR(indexString));
		brsstrcpy(ADR(axisString), ADR('gMotorBasic'));
		appendArrayIndex(i, ADR(axisString));
		stringlcat(ADR(axisString),ADR('.IN.PAR'),SIZEOF(axisString));
		
		IF (i <= PERSIST_MAI_VARLIST) THEN
			brsstrcpy(ADR(gPersister[PERM_AXIS_PARS].IN.WorkingVariableList[i]), ADR(axisString));
		ELSE
			errcolAddError(ADR('Perm Data'), ADR('Too many axes for one persister.'), 0, 0, gErrorCollector);
			EXIT;
		END_IF
		
	END_FOR
	
	// Axis cfg
	// Verify that NUM_MOTOR is defined by ServoControl if error occurs here
	FOR i := 0 TO MAI_MOTOR BY 1 DO
		brsitoa(i, ADR(indexString));
		brsstrcpy(ADR(axisString), ADR('gMotorBasic'));
		appendArrayIndex(i, ADR(axisString));
		stringlcat(ADR(axisString),ADR('.IN.CFG'),SIZEOF(axisString));
		
		IF (i <= PERSIST_MAI_VARLIST) THEN
			brsstrcpy(ADR(gPersister[PERM_AXIS_CFG].IN.WorkingVariableList[i]), ADR(axisString));
		ELSE
			errcolAddError(ADR('Perm Data'), ADR('Too many axes for one persister.'), 0, 0, gErrorCollector);
			EXIT;
		END_IF
		
	END_FOR

	
	// Permanent variables
	gPersister[PERM_CONFIGURATION].IN.pPersistentVariable := ADR(gPermConfiguration);
	gPersister[PERM_CONFIGURATION].IN.sizeofPersistentVariable := SIZEOF(gPermConfiguration);

	gPersister[PERM_HOME_DATA].IN.pPersistentVariable := ADR(gPermHomeData);
	gPersister[PERM_HOME_DATA].IN.sizeofPersistentVariable := SIZEOF(gPermHomeData);

	gPersister[PERM_AXIS_PARS].IN.pPersistentVariable := ADR(gPermAxisPars);
	gPersister[PERM_AXIS_PARS].IN.sizeofPersistentVariable := SIZEOF(gPermAxisPars);

	gPersister[PERM_AXIS_CFG].IN.pPersistentVariable := ADR(gPermAxisCfgs);
	gPersister[PERM_AXIS_CFG].IN.sizeofPersistentVariable := SIZEOF(gPermAxisCfgs);
	
	// DataValid and PersistFn_Init()
	FOR i := 0 TO MAI_PERSISTERS BY 1 DO
		gPersister[i].IN.pDataValid := ADR(gDataValid[i]);
		PersistFn_Init(gPersister[i]);
		IF (gPersister[i].OUT.STAT.Error) THEN
			errcolAddError(ADR('Perm Data'), ADR(gPersister[i].OUT.STAT.ErrorString), gPersister[i].OUT.STAT.ErrorID, 0, gErrorCollector);
		END_IF
	END_FOR
	
	// Initialize persister CSV backups
	errcolAddSourceByName(ADR('Machine configuration backup'), ADR('gPermBackup[0]'), gErrorCollector);
	errcolAddSourceByName(ADR('Home data backup'), ADR('gPermBackup[1]'), gErrorCollector);
	errcolAddSourceByName(ADR('Axis pars backup'), ADR('gPermBackup[2]'), gErrorCollector);
	errcolAddSourceByName(ADR('Axis cfg backup'), ADR('gPermBackup[3]'), gErrorCollector);
	
	brsstrcpy(ADR(gPermBackup[PERM_CONFIGURATION].IN.PAR.FileName), ADR('configuration.csv'));
	brsstrcpy(ADR(gPermBackup[PERM_HOME_DATA].IN.PAR.FileName), ADR('homeData.csv'));
	brsstrcpy(ADR(gPermBackup[PERM_AXIS_PARS].IN.PAR.FileName), ADR('axisPars.csv'));
	brsstrcpy(ADR(gPermBackup[PERM_AXIS_CFG].IN.PAR.FileName), ADR('axisCfg.csv'));
	
	FOR i := 0 TO MAI_PERSISTERS BY 1 DO 
		
		brsstrcpy(ADR(gPermBackup[i].IN.PAR.FileDevice), ADR('permBackup'));
		brsmemcpy(ADR(gPermBackup[i].IN.PAR.VariableList), ADR(gPersister[i].IN.WorkingVariableList), SIZEOF(gPermBackup[i].IN.PAR.VariableList));
		
		brsstrcpy(ADR(gPermBackup[i].IN.CFG.LogFileName), ADR('.csvlogPermBackup'));
		brsitoa(i, ADR(indexString));
		brsstrcat(ADR(gPermBackup[i].IN.CFG.LogFileName), ADR(indexString));
		
		CSVFn_Init(gPermBackup[i]);
		
		IF (NOT gDataValid[i]) THEN
			CSVOpenFile_Init(gPermBackup[i]);
			IF (NOT gPermBackup[i].OUT.STAT.Error) THEN
				gDataValid[i] := TRUE;
				errcolAddError(ADR('Data restored'), ADR(gPermBackup[i].IN.PAR.FileName), 0, 0, gErrorCollector);
			END_IF
		END_IF
				
	END_FOR // loop through permbackup
	
	// Load configuration.csv every startup
	//CSVOpenFile_Init(&gPermBackup[PERM_CONFIGURATION]);
	
	gClearTrace := 1;
	
END_PROGRAM