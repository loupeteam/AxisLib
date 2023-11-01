/* Automation Studio generated header file */
/* Do not edit ! */

#ifndef _BUR_1698853848_2_
#define _BUR_1698853848_2_

#include <bur/plctypes.h>

/* Constants */
#ifdef _REPLACE_CONST
 #define MAI_FILEDEVICES 3U
 #define NUM_FILEDEVICES 4U
#else
 _LOCAL_CONST unsigned char MAI_FILEDEVICES;
 _LOCAL_CONST unsigned char NUM_FILEDEVICES;
#endif


/* Variables */
_BUR_LOCAL plcstring axisString[321];
_BUR_LOCAL plcstring indexString[321];
_BUR_LOCAL signed long i;
_BUR_LOCAL plcstring parameters[4][321];
_BUR_LOCAL plcstring directory[4][321];
_BUR_LOCAL plcstring fileDevice[4][21];
_BUR_LOCAL struct DevLink fileDeviceDevLink[4];
_BUR_LOCAL struct DevLink simCheckDevLink;





__asm__(".section \".plc\"");

/* Used IEC files */
__asm__(".ascii \"iecfile \\\"Logical/Infrastructure/FirstInitProg/FirstInitProg.var\\\" scope \\\"local\\\"\\n\"");
__asm__(".ascii \"iecfile \\\"Logical/Libraries/FileIO/FileIO.fun\\\" scope \\\"global\\\"\\n\"");

/* Exported library functions and function blocks */

__asm__(".previous");


#endif /* _BUR_1698853848_2_ */

