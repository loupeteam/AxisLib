/* Automation Studio generated header file */
/* Do not edit ! */
/* AxisErrLib 0.00.5 */

#ifndef _AXISERRLIB_
#define _AXISERRLIB_
#ifdef __cplusplus
extern "C" 
{
#endif
#ifndef _AxisErrLib_VERSION
#define _AxisErrLib_VERSION 0.00.5
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG4
		#include "Acp10par.h"
		#include "Acp10_MC.h"
		#include "stringext.h"
		#include "errorlib.h"
#endif
#ifdef _SG3
		#include "Acp10par.h"
		#include "Acp10_MC.h"
		#include "stringext.h"
		#include "errorlib.h"
#endif
#ifdef _SGC
		#include "Acp10par.h"
		#include "Acp10_MC.h"
		#include "stringext.h"
		#include "errorlib.h"
#endif

/* Datatypes and datatypes of function blocks */
typedef enum AXERR_RETRIEVE_ST_enum
{	AXERR_RETRIEVE_ST_IDLE,
	AXERR_RETRIEVE_ST_GETTEXT
} AXERR_RETRIEVE_ST_enum;

typedef enum AXERR_RESET_ST_enum
{	AXERR_RESET_ST_IDLE,
	AXERR_RESET_ST_ENDAT,
	AXERR_RESET_ST_RESET
} AXERR_RESET_ST_enum;

typedef enum AXERR_ERR_enum
{	AXERR_ERR_INVALIDINPUT = 53000,
	AXERR_ERR_INVALIDAXIS
} AXERR_ERR_enum;

typedef struct axisAddErrorsToCollector_int_typ
{	signed long retrieveState;
	signed long resetState;
	plcstring errorStringArray[4][81];
	plcstring errorString[330];
	unsigned long pAcknowledge;
	struct MC_BR_ReadAxisError readAxisError;
	struct MC_BR_GetErrorText getErrorText;
	struct MC_BR_WriteParIDText writeParIDText;
	struct MC_Reset reset;
	ERRCOL_ERR_enum addErrorStatus;
} axisAddErrorsToCollector_int_typ;

typedef struct AxisAddErrorsToCollector
{
	/* VAR_INPUT (analog) */
	unsigned long Axis;
	plcstring DataObjectName[13];
	plcstring ErrorSourceName[121];
	unsigned long pErrorCollector;
	/* VAR_OUTPUT (analog) */
	unsigned short ErrorID;
	plcstring ErrorString[330];
	/* VAR (analog) */
	struct axisAddErrorsToCollector_int_typ internal;
	/* VAR_INPUT (digital) */
	plcbit Enable;
	plcbit Reset;
	plcbit ResetEnDat;
	/* VAR_OUTPUT (digital) */
	plcbit AxisError;
	plcbit Error;
} AxisAddErrorsToCollector_typ;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC void AxisAddErrorsToCollector(struct AxisAddErrorsToCollector* inst);


__asm__(".section \".plc\"");

/* Additional IEC dependencies */
__asm__(".ascii \"iecdep \\\"Logical/Libraries/Loupe/errorlib/ErrorLib.var\\\" scope \\\"global\\\"\\n\"");

__asm__(".previous");

#ifdef __cplusplus
};
#endif
#endif /* _AXISERRLIB_ */

