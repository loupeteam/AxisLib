(*
 * File: Types.typ
 * Copyright (c) 2023 Loupe
 * https://loupe.team
 * 
 * This file is part of AxisErrLib, licensed under the MIT License.
 *)
TYPE
	AXERR_RETRIEVE_ST_enum : 
		(
		AXERR_RETRIEVE_ST_IDLE,
		AXERR_RETRIEVE_ST_GETTEXT
		);
	AXERR_RESET_ST_enum : 
		(
		AXERR_RESET_ST_IDLE,
		AXERR_RESET_ST_ENDAT,
		AXERR_RESET_ST_RESET
		);
	AXERR_ERR_enum : 
		(
		AXERR_ERR_INVALIDINPUT := 53000,
		AXERR_ERR_INVALIDAXIS
		);
	axisAddErrorsToCollector_int_typ : 	STRUCT 
		retrieveState : DINT;
		resetState : DINT;
		errorStringArray : ARRAY[0..3]OF STRING[80];
		errorString : STRING[ERRCOL_STRLEN_ERRORSTRING];
		pAcknowledge : UDINT;
		readAxisError : MC_BR_ReadAxisError;
		getErrorText : MC_BR_GetErrorText;
		writeParIDText : MC_BR_WriteParIDText;
		reset : MC_Reset;
		addErrorStatus : ERRCOL_ERR_enum;
	END_STRUCT;
END_TYPE
