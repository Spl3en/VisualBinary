/**
	Written by:	Spl3en (Moreau Cyril) 			<cmoreau.contact@gmail.com>

*/
#pragma once

// ---------- Includes ------------


// ---------- Defines -------------


// ---------- Debugging -------------
#ifdef __DBG_ACTIVATED__
#undef __DBG_ACTIVATED__
#endif
#define __DBG_ACTIVATED__ 		TRUE

#ifdef __DEBUG_OBJECT__
#undef __DEBUG_OBJECT__
#endif
#define __DEBUG_OBJECT__ "GraphicObjects"
#include "../dbg/dbg.h"


// ------ Class declaration -------



// --------- Constructors ---------


// ----------- Methods ------------
void draw_axes (float vector[3]);

void draw_square (float view[3], float x, float y, float z, float width, float height);
void draw_point  (float view[3], float x, float y, float z);


// --------- Destructors ----------



