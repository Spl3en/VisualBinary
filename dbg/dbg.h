/**
	Written by:	Spl3en (Moreau Cyril) 			<cmoreau.contact@gmail.com>

*/
#pragma once

// ---------- Includes ------------
#include "../Ztring/Ztring.h"

// ---------- Defines -------------
#if __DBG_ACTIVATED__ == TRUE
	#ifdef __DEBUG_OBJECT__
		#define dbg(format, ...) 	\
			_dbg(__DEBUG_OBJECT__ ":" TOSTRING(__LINE__) "| " format "\n", ##__VA_ARGS__)
	#else
		#define dbg(format, ...) 	\
			_dbg("%s:" TOSTRING(__LINE__) "| " format "\n", ((strrchr(__FILE__, '\\')) != NULL) ? &(strrchr(__FILE__, '\\'))[1] : __FILE__, ##__VA_ARGS__)
	#endif
#else
	#define dbg(format, ...)
#endif

void
_dbg (char *format, ...);


// ----------- Methods ------------
