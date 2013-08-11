/**
	Written by:	Spl3en (Moreau Cyril) 			<cmoreau.contact@gmail.com>

*/
#pragma once

// ---------- Includes ------------
#include <SFML/Graphics.h>

#include "../GraphicObjects/GraphicObjects.h"
#include "../Function/Function.h"

// ---------- Defines -------------
#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

// ---------- Debugging -------------
#ifdef __DBG_ACTIVATED__
#undef __DBG_ACTIVATED__
#endif
#define __DBG_ACTIVATED__ 		TRUE

#ifdef __DEBUG_OBJECT__
#undef __DEBUG_OBJECT__
#endif
#define __DEBUG_OBJECT__ "AppWindow"
#include "../dbg/dbg.h"


// ------ Class declaration -------
typedef struct _AppWindow
{
	// AppWindow hérite de RenderWindow
	sfRenderWindow* obj;

	// Data
	float *view;

	// Application states
	BbQueue *drawing_routines;
	int app_state;
	int last_app_state;
	DrawFunction *draw;

}	AppWindow;



// --------- Constructors ---------

AppWindow *
AppWindow_new (void);

AppWindow *
AppWindow_alloc (void);

// ----------- Methods ------------

void
AppWindow_init (AppWindow *window);

void
AppWindow_main (AppWindow *window);

int
AppWindow_add_draw_routine (AppWindow *this, DrawFunction *func);

void
AppWindow_set_state (AppWindow *window, int state);

float *
AppWindow_get_view (AppWindow *this);

// --------- Destructors ----------

void
AppWindow_free (AppWindow *window);


