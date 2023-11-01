
FUNCTION_BLOCK AxisAddErrorsToCollector (*Add all current axis errors to an error collector*) (*$GROUP=User,$CAT=User,$GROUPICON=User.png,$CATICON=User.png*)
	VAR_INPUT
		Axis : UDINT;
		Enable : BOOL;
		Reset : BOOL;
		ResetEnDat : BOOL;
		DataObjectName : STRING[12];
		ErrorSourceName : STRING[ERRCOL_STRLEN_SOURCENAME];
		pErrorCollector : UDINT;
	END_VAR
	VAR_OUTPUT
		AxisError : BOOL;
		Error : BOOL;
		ErrorID : UINT;
		ErrorString : STRING[ERRCOL_STRLEN_ERRORSTRING];
	END_VAR
	VAR
		internal : axisAddErrorsToCollector_int_typ;
	END_VAR
END_FUNCTION_BLOCK
