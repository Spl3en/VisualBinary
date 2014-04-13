/**
	Written by:	Spl3en (Moreau Cyril) 			<cmoreau.contact@gmail.com>

*/
#pragma once

// ---------- Includes ------------
#include <GL/glew.h>
#include <SFML/Graphics.h>
#include "../GraphicObjects/GraphicObjects.h"
#include "../Function/Function.h"
#include "../WrapSFML/wrap.h"

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
	int width, height;

	// Data
	float *view;

	// Draw
	BbQueue *drawing_routines;
	int draw_app_state;
	int last_draw_app_state;
	DrawFunction *draw;

	// Input
	BbQueue *input_routines;

	// Update
	BbQueue *update_routines;

}	AppWindow;



// --------- Constructors ---------

AppWindow *
AppWindow_new (char *window_name, int width, int height, bool fullscreen);

AppWindow *
AppWindow_alloc (void);

// ----------- Methods ------------

void
AppWindow_init (AppWindow *this, char *window_name, int width, int height, bool fullscreen);

void
AppWindow_main (AppWindow *window);

int
AppWindow_add_draw_routine (AppWindow *this, DrawFunction *func);

void
AppWindow_add_input_routine (AppWindow *this, Function *func);

void
AppWindow_add_update_routine (AppWindow *this, DrawFunction *func);

void
AppWindow_set_state (AppWindow *window, int state);

float *
AppWindow_get_view (AppWindow *this);

void
AppWindow_view_reset (AppWindow *this);

// --------- Destructors ----------

void
AppWindow_free (AppWindow *window);


