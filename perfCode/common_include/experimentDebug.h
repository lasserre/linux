// Debug Defines

#pragma once

/* --------------- Constant Macros --------------- */
#define DEBUG_VERBOSE 3
#define DEBUG_INFO 2
#define DEBUG_EXP_OUTPUT 1
#define DEBUG_OFF 0
#define EXECUTE_IF_DEBUG(debug_level, command) \
	if (DEBUG_LEVEL >= debug_level)	command;
#define LOG_DEBUG(debug_level, string) \
	if (DEBUG_LEVEL >= debug_level) cout << __FILE__ << ":" << __FUNCTION__ \
		<< "():" << __LINE__ << ":\t" << string; 		
/*------------------------------------------------ */

/* ------------ USER-SPECIFIED MACROS HERE!! ------------- */
#ifndef DEBUG_LEVEL
	#define DEBUG_LEVEL DEBUG_OFF
#endif
/* ------------------------------------------------------- */