#pragma once

#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
	#define Sleep(x) usleep(1000 * x)
#endif
