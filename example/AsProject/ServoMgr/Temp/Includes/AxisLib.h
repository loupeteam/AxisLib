/* Automation Studio generated header file */
/* Do not edit ! */
/* AxisLib 3.06.4 */

#ifndef _AXISLIB_
#define _AXISLIB_
#ifdef __cplusplus
extern "C" 
{
#endif
#ifndef _AxisLib_VERSION
#define _AxisLib_VERSION 3.06.4
#endif

#include <bur/plctypes.h>

#ifndef _BUR_PUBLIC
#define _BUR_PUBLIC
#endif
#ifdef _SG3
		#include "Acp10_MC.h"
		#include "Acp10man.h"
#endif
#ifdef _SG4
		#include "Acp10_MC.h"
		#include "Acp10man.h"
#endif
#ifdef _SGC
		#include "Acp10_MC.h"
		#include "Acp10man.h"
#endif


/* Constants */
#ifdef _REPLACE_CONST
 #define AXLIB_STRLEN_NAME 120U
 #define AXLIB_STRLEN_ERROR 329U
#else
 _GLOBAL_CONST unsigned short AXLIB_STRLEN_NAME;
 _GLOBAL_CONST unsigned short AXLIB_STRLEN_ERROR;
#endif




/* Datatypes and datatypes of function blocks */
typedef enum AXISLIB_PLCOPEN_ST_enum
{	AXISLIB_PLCOPEN_ST_INVALID = 0,
	AXISLIB_PLCOPEN_ST_ERRORSTOP,
	AXISLIB_PLCOPEN_ST_DISABLED,
	AXISLIB_PLCOPEN_ST_STANDSTILL,
	AXISLIB_PLCOPEN_ST_STOPPING,
	AXISLIB_PLCOPEN_ST_HOMING,
	AXISLIB_PLCOPEN_ST_DISCRETEMOTN,
	AXISLIB_PLCOPEN_ST_SYNCMOTN,
	AXISLIB_PLCOPEN_ST_CONTMOTN
} AXISLIB_PLCOPEN_ST_enum;

typedef enum AXISLIB_REFST_enum
{	AXISLIB_REFST_START,
	AXISLIB_REFST_START_CHECK,
	AXISLIB_REFST_START_INIT,
	AXISLIB_REFST_START_HOME,
	AXISLIB_REFST_IDLE,
	AXISLIB_REFST_REF_INIT,
	AXISLIB_REFST_REF_HOME,
	AXISLIB_REFST_CLEAR_INIT
} AXISLIB_REFST_enum;

typedef enum AXISLIB_RPST_enum
{	AXISLIB_RPST_IDLE,
	AXISLIB_RPST_INIT,
	AXISLIB_RPST_CHECK,
	AXISLIB_RPST_HOME,
	AXISLIB_RPST_DONE
} AXISLIB_RPST_enum;

typedef struct AxisStatus_Int_typ
{	struct MC_BR_ReadDriveStatus ReadDriveStatus;
	struct MC_ReadActualPosition ReadActualPosition;
	struct MC_BR_ReadCyclicPosition ReadCyclicPosition;
	struct MC_ReadActualVelocity ReadActualVelocity;
	struct MC_ReadStatus ReadStatus;
} AxisStatus_Int_typ;

typedef struct AxisLib_PLCOpenState_typ
{	enum AXISLIB_PLCOPEN_ST_enum State;
	plcbit Errorstop;
	plcbit Disabled;
	plcbit StandStill;
	plcbit Stopping;
	plcbit Homing;
	plcbit DiscreteMotion;
	plcbit SynchronizedMotion;
	plcbit ContinuousMotion;
} AxisLib_PLCOpenState_typ;

typedef struct AxisReference_Int_typ
{	enum AXISLIB_REFST_enum state;
	struct MC_DRIVESTATUS_TYP driveStatus;
	struct MC_ENDLESS_POSITION endlessPosition;
	plcbit Reference;
	plcbit ClearReference;
	plcbit needToClearReference;
	plcbit oldNetworkInit;
	struct ACP10HOMPA_typ initHomeParam;
	struct MC_BR_ReadDriveStatus readDriveStatus;
	struct MC_ReadStatus readStatus;
	struct MC_BR_CheckEndlessPosition checkEndlessPos;
	struct MC_BR_InitEndlessPosition initEndlessPos;
	struct MC_Home home;
} AxisReference_Int_typ;

typedef struct AxisBasic_IN_CMD_typ
{	plcbit Power;
	plcbit Reference;
	plcbit MoveAbsolute;
	plcbit MoveAdditive;
	plcbit MoveVelocity;
	plcbit MoveCyclicPosition;
	plcbit JogForward;
	plcbit JogReverse;
	plcbit Halt;
	plcbit Stop;
	plcbit ClearReference;
	plcbit AcknowledgeError;
	plcbit Reset;
	plcbit WaitToInitializeReference;
} AxisBasic_IN_CMD_typ;

typedef struct AxisBasic_IN_PAR_typ
{	float Position;
	float Distance;
	struct MC_CYCLIC_POSITION CyclicPosition;
	float Velocity;
	float Acceleration;
	float Deceleration;
	unsigned char Direction;
	float JogVelocity;
	float JogAcceleration;
	float JogDeceleration;
} AxisBasic_IN_PAR_typ;

typedef struct AxisBasic_IN_CFG_typ
{	plcstring Name[121];
	plcbit Active;
	float HomingPosition;
	unsigned char HomingMode;
	float DefaultPosition;
	float StopDeceleration;
	unsigned short ReadCyclicPositionParID;
	unsigned long Factor;
	unsigned long Period;
} AxisBasic_IN_CFG_typ;

typedef struct AxisBasic_IN_typ
{	struct AxisBasic_IN_CMD_typ CMD;
	struct AxisBasic_IN_PAR_typ PAR;
	struct AxisBasic_IN_CFG_typ CFG;
} AxisBasic_IN_typ;

typedef struct AxisBasic_IO_typ
{	plcbit diHomeSwitch;
	plcbit diNegHWSwitch;
	plcbit diPosHWSwitch;
	plcbit diTrigger1;
	plcbit diTrigger2;
} AxisBasic_IO_typ;

typedef struct AxisBasic_OUT_typ
{	plcbit Active;
	float ActualPosition;
	double ActualPositionPrecise;
	struct MC_CYCLIC_POSITION ActualCyclicPosition;
	float ActualVelocity;
	struct MC_DRIVESTATUS_TYP DriveStatus;
	struct AxisLib_PLCOpenState_typ PLCOpenState;
	plcbit Referenced;
	plcbit EndlessPositionInitialized;
	plcbit DataValid;
	plcbit Done;
	plcbit Busy;
	plcbit Warning;
	unsigned short WarningCount;
	plcbit Error;
	unsigned short ErrorCount;
	unsigned short ErrorID;
	plcstring ErrorString[330];
} AxisBasic_OUT_typ;

typedef struct AxisBasic_TEST_STAT_typ
{	plcbit Busy;
	plcbit Done;
	plcbit Warning;
	unsigned short WarningCount;
	plcbit Error;
	unsigned short ErrorCount;
	unsigned short ErrorID;
	float ActualPosition;
	float ActualVelocity;
	plcbit NetworkInit;
	plcbit ControllerStatus;
	plcbit HomingOk;
} AxisBasic_TEST_STAT_typ;

typedef struct AxisBasic_TEST_typ
{	plcbit Enable;
	struct AxisBasic_IN_CMD_typ CMD;
	struct AxisBasic_IN_PAR_typ PAR;
	struct AxisBasic_TEST_STAT_typ STAT;
} AxisBasic_TEST_typ;

typedef struct AxisStatus
{
	/* VAR_INPUT (analog) */
	unsigned long Axis;
	unsigned short ReadCyclicPositionParID;
	/* VAR_OUTPUT (analog) */
	float ActualPosition;
	struct MC_CYCLIC_POSITION ActualCyclicPosition;
	float ActualVelocity;
	struct MC_DRIVESTATUS_TYP DriveStatus;
	struct AxisLib_PLCOpenState_typ PLCOpenState;
	unsigned short ErrorID;
	/* VAR (analog) */
	struct AxisStatus_Int_typ internal;
	/* VAR_INPUT (digital) */
	plcbit Enable;
	/* VAR_OUTPUT (digital) */
	plcbit Valid;
	plcbit Busy;
	plcbit Error;
} AxisStatus_typ;

typedef struct AxisReference
{
	/* VAR_INPUT (analog) */
	unsigned long Axis;
	unsigned long EndlessPositionDataAddress;
	float DefaultPosition;
	float Position;
	unsigned char HomingMode;
	/* VAR_OUTPUT (analog) */
	unsigned short ErrorID;
	/* VAR (analog) */
	struct AxisReference_Int_typ internal;
	/* VAR_INPUT (digital) */
	plcbit WaitToInitialize;
	plcbit Reference;
	plcbit ClearReference;
	/* VAR_OUTPUT (digital) */
	plcbit Done;
	plcbit Busy;
	plcbit CommandAborted;
	plcbit Error;
	plcbit Referenced;
	plcbit EndlessPositionInitialized;
	plcbit DataValid;
} AxisReference_typ;

typedef struct AxisBasic_Int_FUB_typ
{	struct AxisStatus Status;
	struct MC_Power Power;
	struct MC_BR_SetHardwareInputs SetHardwareInputs;
	struct AxisReference Reference;
	struct MC_Home Home;
	struct MC_MoveAbsolute MoveAbsolute;
	struct MC_MoveAdditive MoveAdditive;
	struct MC_MoveVelocity MoveVelocity;
	struct MC_BR_JogVelocity Jog;
	struct MC_BR_MoveCyclicPosition MoveCyclicPosition;
	struct MC_Halt Halt;
	struct MC_Stop Stop;
	struct MC_BR_ReadAxisError ReadAxisError;
	struct MC_Reset Reset;
	struct MC_BR_InitModPos InitModPos;
} AxisBasic_Int_FUB_typ;

typedef struct AxisBasic_Internal_typ
{	struct AxisBasic_Int_FUB_typ FUB;
	plcstring ErrorString[4][80];
	plcbit ResetOK;
	plcbit useFastFunction;
} AxisBasic_Internal_typ;

typedef struct AxisBasic_typ
{	struct ACP10AXIS_typ* pAxisObject;
	unsigned long pEndlessPosition;
	plcstring ErrorTextModuleName[13];
	struct AxisBasic_IN_typ IN;
	struct AxisBasic_IO_typ IO;
	struct AxisBasic_OUT_typ OUT;
	struct AxisBasic_TEST_typ TEST;
	struct AxisBasic_Internal_typ Internal;
} AxisBasic_typ;

typedef struct AxisApplicationLimits_typ
{	float pos_sw_end;
	float neg_sw_end;
} AxisApplicationLimits_typ;

typedef struct AxisExpandLimit_Internal_typ
{	struct MC_BR_InitAxisSubjectPar _MC_BR_InitAxisSubjectPar;
	struct AxisApplicationLimits_typ ApplicationLimits;
	struct AxisApplicationLimits_typ ExpandedLimits;
	plcbit _AcknowledgeError;
	unsigned char State;
} AxisExpandLimit_Internal_typ;

typedef struct AxisRestorePosition_Int_typ
{	enum AXISLIB_RPST_enum State;
	struct MC_BR_InitEndlessPosition InitEndlessPos;
	struct MC_BR_CheckEndlessPosition CheckEndlessPos;
	struct MC_Home Home;
} AxisRestorePosition_Int_typ;

typedef struct AxisRestorePosition
{
	/* VAR_INPUT (analog) */
	unsigned long Axis;
	unsigned long DataAddress;
	/* VAR_OUTPUT (analog) */
	unsigned short ErrorID;
	/* VAR (analog) */
	struct AxisRestorePosition_Int_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit Execute;
	/* VAR_OUTPUT (digital) */
	plcbit Done;
	plcbit Busy;
	plcbit Error;
	plcbit EndlessPositionInitialized;
	plcbit DataValid;
} AxisRestorePosition_typ;

typedef struct AxisExpandLimit
{
	/* VAR_INPUT (analog) */
	unsigned long Axis;
	float Tolerance;
	/* VAR_OUTPUT (analog) */
	unsigned short ErrorID;
	/* VAR (analog) */
	struct AxisExpandLimit_Internal_typ Internal;
	/* VAR_INPUT (digital) */
	plcbit Enable;
	plcbit AcknowledgeError;
	/* VAR_OUTPUT (digital) */
	plcbit Active;
	plcbit Busy;
	plcbit Error;
} AxisExpandLimit_typ;



/* Prototyping of functions and function blocks */
_BUR_PUBLIC void AxisStatus(struct AxisStatus* inst);
_BUR_PUBLIC void AxisReference(struct AxisReference* inst);
_BUR_PUBLIC void AxisRestorePosition(struct AxisRestorePosition* inst);
_BUR_PUBLIC void AxisExpandLimit(struct AxisExpandLimit* inst);
_BUR_PUBLIC plcbit AxisBasicFn_Cyclic(struct AxisBasic_typ* t);
_BUR_PUBLIC plcbit AxisBasicFn_Fast(struct AxisBasic_typ* t);
_BUR_PUBLIC plcbit axisInternalFastFn(struct AxisBasic_typ* t);


__asm__(".section \".plc\"");

/* Additional IEC dependencies */
__asm__(".ascii \"iecdep \\\"Logical/Libraries/Acp10_MC/acp10_mc.var\\\" scope \\\"global\\\"\\n\"");

__asm__(".previous");

#ifdef __cplusplus
};
#endif
#endif /* _AXISLIB_ */

