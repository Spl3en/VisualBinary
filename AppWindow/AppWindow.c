#include "AppWindow.h"
#include <stdlib.h>
#include <SFML/OpenGL.h>

#define __DEBUG_OBJECT__ "AppWindow"
#include "dbg/dbg.h"


AppWindow *
AppWindow_new (char *window_name, int width, int height, bool fullscreen)
{
	AppWindow *this;

	if ((this = AppWindow_alloc ()) == NULL)
		return NULL;

	AppWindow_init (this, window_name, width, height, fullscreen);

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),glGetString(GL_SHADING_LANGUAGE_VERSION));
    glewExperimental = GL_TRUE;
	if ((glewInit () != GLEW_OK)
	|| (glGenVertexArrays == NULL))
	{
		printf ("GLEW hasn't been initialized correctly or isn't fully supported.\n");
		AppWindow_free (this);
		return NULL;
	}

	return this;
}

AppWindow *
AppWindow_alloc (void)
{
	return calloc (1, sizeof (AppWindow));
}

float *
AppWindow_get_view (AppWindow *this)
{
	return this->view;
}

void
AppWindow_init (AppWindow *this, char *window_name, int width, int height, bool fullscreen)
{
	this->width = width;
	this->height = height;

	/* RenderWindow */
	sfVideoMode video_mode = {
		.width  = width,
		.height = height,
		.bitsPerPixel = 32
	};

	sfContextSettings settings = {
		.depthBits = 32,
		.stencilBits = 8,
		.antialiasingLevel = 0,
		.majorVersion = 2,
		.minorVersion = 1,
	};

	SFML(this) = sfRenderWindow_create (
		video_mode,
		window_name,
		 (fullscreen) ? sfFullscreen : sfDefaultStyle,
		&settings
	);

	sfRenderWindow_setVerticalSyncEnabled (SFML (this) , TRUE);

	// OpenGL initialization
	{
		glShadeModel (GL_SMOOTH);							// Enable Smooth Shading
		glClearColor (0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth (1.0f);									// Depth Buffer Setup

		glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		glViewport (0, 0, width, height);					// Reset The Current Viewport

		glMatrixMode (GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity ();									// Reset The Projection Matrix
		gluPerspective (50.0, (float) width / height, 0.1, 50.0);

		// Smooth points
		glEnable (GL_POINT_SMOOTH);

		// Transparency
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode (GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity ();									// Reset The Modelview Matrix
	}

	// Initialize App Window state
	this->draw_app_state = APP_STATE_UNDEFINED;
	this->last_draw_app_state  = APP_STATE_UNDEFINED;
	this->drawing_routines = bb_queue_new ();
	this->input_routines = bb_queue_new ();
	this->update_routines = bb_queue_new ();
	AppWindow_view_reset (this);

	// Initialize view at X=30°
	AppWindow_set_view (this, 30.0, this->view[Y_AXIS], this->view[Z_AXIS]);

	// Get a profiler
	this->profiler = ProfilerFactory_getProfiler ("SFML");
}

void
AppWindow_set_view (AppWindow *this, float x, float y, float z)
{
	this->view[X_AXIS] = x;
	this->view[Y_AXIS] = y;
	this->view[Z_AXIS] = z;
	memcpy (this->targetView, this->view, sizeof (this->view));
}

int
AppWindow_add_draw_routine (AppWindow *this, DrawFunction *func)
{
	bb_queue_add (this->drawing_routines, func);
	return bb_queue_get_length (this->drawing_routines) - 1;
}

void
AppWindow_add_input_routine (AppWindow *this, Function *func)
{
	bb_queue_add (this->input_routines, func);
}

void
AppWindow_add_update_routine (AppWindow *this, DrawFunction *func)
{
	bb_queue_add (this->update_routines, func);
}

void
AppWindow_set_state (AppWindow *window, int state)
{
	window->draw_app_state = state;
}

void
AppWindow_view_reset (AppWindow *this)
{
	AppWindow_set_view (this, 0.0, 0.0, -2.0);
}

void
AppWindow_main (AppWindow *this)
{
	void AppWindow_draw ()
	{
		glLoadIdentity ();

		// draw_axes (this->view);

		// Draw profiler
		if (Profiler_getTime (this->profiler) >= 1.0f) {
			Profiler_update (this->profiler);
			Profiler_restart (this->profiler);
		}

		if (this->draw_app_state != APP_STATE_UNDEFINED)
		{
			if (this->last_draw_app_state != this->draw_app_state)
			{
				this->draw = bb_queue_pick_nth (this->drawing_routines, this->draw_app_state);
			}

			draw_function_call (this->draw);
		}

		this->last_draw_app_state  = this->draw_app_state;

		sfRenderWindow_pushGLStates (SFML (this));
		sfRenderWindow_drawText (SFML (this), this->profiler->text, NULL);
		sfRenderWindow_popGLStates (SFML (this));
	}

	void AppWindow_input ()
	{
		sfEvent event;

		while (sfRenderWindow_pollEvent (SFML (this) , &event))
		{
			switch (event.type)
			{
				case sfEvtClosed:
				{
					// On exit
					sfRenderWindow_close (SFML (this));
				}
				break;

				case sfEvtResized:
				{
					// On ajuste le viewport lorsque la fenêtre est redimensionnée
					this->width  = event.size.width;
					this->height = event.size.height;
					glViewport (0, 0, this->width, this->height);
					glMatrixMode (GL_PROJECTION);
					glLoadIdentity ();
					gluPerspective (50.0, (float) this->width / this->height, 0.1, 50.0);
					glMatrixMode (GL_MODELVIEW);
				}

				case sfEvtMouseWheelMoved:
					this->targetView[Z_AXIS] += event.mouseWheel.delta * 0.1;
				break;

				default:
				break;
			}
		}

		// Keyboard
		if (sfKeyboard_isKeyPressed (sfKeyEscape))
		{
			sfRenderWindow_close (SFML (this));
		}

		if (sfKeyboard_isKeyPressed (sfKeyUp))
		{
			this->targetView[Y_AXIS] += 1.0;
		}

		if (sfKeyboard_isKeyPressed (sfKeyDown))
		{
			this->targetView[Y_AXIS] -= 1.0;
		}

		if (sfKeyboard_isKeyPressed (sfKeyLeft))
		{
			this->targetView[X_AXIS] -= 1.0;
		}

		if (sfKeyboard_isKeyPressed (sfKeyRight))
		{
			this->targetView[X_AXIS] += 1.0;
		}

		if (sfKeyboard_isKeyPressed (sfKeyPageUp))
		{
			this->targetView[Z_AXIS] += 0.02;
		}

		if (sfKeyboard_isKeyPressed (sfKeyPageDown))
		{
			this->targetView[Z_AXIS] -= 0.02;
		}

		if (sfKeyboard_isKeyPressed (sfKeyR))
		{
			AppWindow_view_reset (this);
		}

		foreach_bbqueue_item (this->input_routines, Function *function)
		{
			function_call (function);
		}
	}

	void AppWindow_clear ()
	{
		// Clear Screen And Depth Buffer
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void AppWindow_update ()
	{
		// Update view
		for (PosAxis axis = X_AXIS; axis < N_AXIS; axis++) {
			if (this->view[axis] != this->targetView[axis]) {
				float increment = (axis != Z_AXIS) ? 1.0 : 0.03;
				// Threshold
				if (fabs (this->view[axis] - this->targetView[axis]) > increment) {
					if (this->view[axis] < this->targetView[axis]) {
						this->view[axis] += increment;
					} else {
						this->view[axis] -= increment;
					}
				} else {
					this->view[axis] = this->targetView[axis];
				}
			}
		}

		// Draw items
		foreach_bbqueue_item (this->update_routines, DrawFunction *function)
		{
			draw_function_call (function);
		}
	}

	// Main loop
	while (sfRenderWindow_isOpen (SFML (this)) )
	{
		Profiler_tick (this->profiler);

		// Input
		AppWindow_input ();

		// Clear
		AppWindow_clear ();

		// Update
		AppWindow_update ();

		// Draw
		AppWindow_draw ();

		// Display
		sfRenderWindow_display (SFML (this));
	}
}

void
AppWindow_free (AppWindow *this)
{
	if (this != NULL)
	{
		free (this);
	}
}
