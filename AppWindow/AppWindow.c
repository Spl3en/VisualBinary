#include "AppWindow.h"
#include <stdlib.h>
#include <SFML/OpenGL.h>


AppWindow *
AppWindow_new (char *window_name, int width, int height, bool fullscreen)
{
	AppWindow *this;

	if ((this = AppWindow_alloc()) == NULL)
		return NULL;

	AppWindow_init(this, window_name, width, height, fullscreen);

	return this;
}

AppWindow *
AppWindow_alloc (void)
{
	return calloc (1, sizeof(AppWindow));
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

	SFML(this) = sfRenderWindow_create(
		video_mode,
		window_name,
		(fullscreen) ? sfFullscreen : sfDefaultStyle,
		&settings
	);

	sfRenderWindow_setVerticalSyncEnabled(SFML(this), TRUE);

	// OpenGL initialization
	void appWindow_init_OpenGL ()
	{
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
		glViewport(0, 0, width, height);					// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix
		gluPerspective(50.0, (float) width / height, 0.1, 50.0);

		// Transparency
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}
	appWindow_init_OpenGL();


	// Initialize App Window state
	this->draw_app_state = -1;
	this->last_draw_app_state  = -1;
	this->drawing_routines = bb_queue_new();
	this->input_routines = bb_queue_new();
	this->update_routines = bb_queue_new();
	this->view = calloc(sizeof(float), 3);
	AppWindow_view_reset(this);
	this->view[0] = 45.0;
}

int
AppWindow_add_draw_routine (AppWindow *this, DrawFunction *func)
{
	bb_queue_add(this->drawing_routines, func);
	return bb_queue_get_length(this->drawing_routines) - 1;
}

void
AppWindow_add_input_routine (AppWindow *this, Function *func)
{
	bb_queue_add(this->input_routines, func);
}

void
AppWindow_add_update_routine (AppWindow *this, Function *func)
{
	bb_queue_add(this->update_routines, func);
}

void
AppWindow_set_state (AppWindow *window, int state)
{
	window->draw_app_state = state;
}

void
AppWindow_view_reset (AppWindow *this)
{
	this->view[X_AXIS] = 0.0;
	this->view[Y_AXIS] = 0.0;
	this->view[Z_AXIS] = -2.0;
}

void
AppWindow_main (AppWindow *this)
{
	void AppWindow_draw ()
	{
		glLoadIdentity ();

		// draw_axes (this->view);

		if (this->draw_app_state != -1)
		{
			if (this->last_draw_app_state != this->draw_app_state)
			{
				this->draw = bb_queue_pick_nth (this->drawing_routines, this->draw_app_state);
			}

			draw_function_call (this->draw);
		}

		this->last_draw_app_state  = this->draw_app_state;
	}

	void AppWindow_input ()
	{
		sfEvent event;

		while (sfRenderWindow_pollEvent(SFML(this), &event))
		{
			switch (event.type)
			{
				case sfEvtClosed:
				{
					// On exit
					sfRenderWindow_close(SFML(this));
				}
				break;

				case sfEvtResized:
				{
					// On ajuste le viewport lorsque la fenêtre est redimensionnée
					this->width  = event.size.width;
					this->height = event.size.height;
					glViewport(0, 0, this->width, this->height);
				}

				case sfEvtMouseWheelMoved:
					this->view[Z_AXIS] = this->view[Z_AXIS] + (event.mouseWheel.delta * 0.1);
				break;

				default:
				break;
			}
		}

		// Keyboard
		if (sfKeyboard_isKeyPressed(sfKeyEscape))
		{
			sfRenderWindow_close(SFML(this));
		}

		if (sfKeyboard_isKeyPressed(sfKeyUp))
		{
			this->view[Y_AXIS] += 1.0;
		}

		if (sfKeyboard_isKeyPressed(sfKeyDown))
		{
			this->view[Y_AXIS] -= 1.0;
		}

		if (sfKeyboard_isKeyPressed(sfKeyLeft))
		{
			this->view[X_AXIS] -= 1.0;
		}

		if (sfKeyboard_isKeyPressed(sfKeyRight))
		{
			this->view[X_AXIS] += 1.0;
		}

		if (sfKeyboard_isKeyPressed(sfKeyPageUp))
		{
			this->view[Z_AXIS] -= 0.01;
		}

		if (sfKeyboard_isKeyPressed(sfKeyPageDown))
		{
			this->view[Z_AXIS] += 0.01;
		}

		if (sfKeyboard_isKeyPressed(sfKeyR))
		{
			AppWindow_view_reset(this);
		}

		foreach_bbqueue_item (this->input_routines, Function *function)
		{
			function->call (function->arg);
		}
	}

	void AppWindow_clear ()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		sfRenderWindow_clear(SFML(this), sfBlack);
	}

	void AppWindow_update ()
	{
		foreach_bbqueue_item (this->update_routines, Function *function)
		{
			function->call (function->arg);
		}
	}

	// Loop
	while (sfRenderWindow_isOpen(SFML(this)))
	{
		// Input
		AppWindow_input();

		// Clear
		AppWindow_clear();

		// Update
		AppWindow_update();

		// Draw
		AppWindow_draw();

		// Display
		sfRenderWindow_display(SFML(this));
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
