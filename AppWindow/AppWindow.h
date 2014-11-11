/**
	Written by:	Spl3en (Moreau Cyril) 			<cmoreau.contact@gmail.com>

*/
#pragma once

// ---------- Includes ------------
#include <math.h>
#include <GL/glew.h>
#include <SFML/Graphics.h>
#include "BbQueue/BbQueue.h"
#include "GraphicObjects/GraphicObjects.h"
#include "Function/Function.h"
#include "WrapSFML/wrap.h"
#include "Utils/Utils.h"
#include "Profiler/ProfilerFactory.h"

// ---------- Defines -------------
typedef enum {
	X_AXIS,
	Y_AXIS,
	Z_AXIS,
	N_AXIS
} PosAxis;
#define APP_STATE_UNDEFINED -1

// ------ Class declaration -------
typedef struct _AppWindow
{
	// AppWindow hérite de RenderWindow
	sfRenderWindow* obj;
	int width, height;

	// Data
	float view[3];
	float targetView[3];

	// Draw
	BbQueue *drawing_routines;
	int draw_app_state;
	int last_draw_app_state;
	DrawFunction *draw;

	// Input
	BbQueue *input_routines;

	// Update
	BbQueue *update_routines;

	// Profiler
	Profiler *profiler;

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
AppWindow_set_view (AppWindow *this, float x, float y, float z);

void
AppWindow_view_reset (AppWindow *this);

// --------- Destructors ----------

void
AppWindow_free (AppWindow *window);


