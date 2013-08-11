#include "AppWindow.h"
#include "../WrapSFML/wrap.h"
#include <stdlib.h>
#include <SFML/OpenGL.h>

// Private defines
#define SCREEN_W 800
#define SCREEN_H 800


AppWindow *
AppWindow_new (void)
{
	AppWindow *this;

	if ((this = AppWindow_alloc()) == NULL)
		return NULL;

	AppWindow_init(this);

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
AppWindow_init (AppWindow *this)
{
	/* RenderWindow */
	sfVideoMode video_mode = {
		.width  = SCREEN_W,
		.height = SCREEN_H,
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
		"VisualBinary",
		sfDefaultStyle,
		&settings
	);

	sfRenderWindow_setVerticalSyncEnabled(SFML(this), TRUE);

	// OpenGL
	void appWindow_init_OpenGL ()
	{
		glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
		glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
		glClearDepth(1.0f);									// Depth Buffer Setup
		glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
		glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

		glViewport(0, 0, SCREEN_W, SCREEN_H);				// Reset The Current Viewport

		glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
		glLoadIdentity();									// Reset The Projection Matrix

		// Calculate the aspect ratio of the window
		gluPerspective(50.0, 1.0, 1.0, 10.0);

		glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
		glLoadIdentity();									// Reset The Modelview Matrix
	}
	appWindow_init_OpenGL();


	// Initialize App Window state
	this->app_state = -1;
	this->last_app_state = -1;
	this->drawing_routines = bb_queue_new();
	this->view = calloc(sizeof(float), 3);
	this->view[X_AXIS] = 45.0;
	this->view[Y_AXIS] = 45.0;
	this->view[Z_AXIS] = -5.0;
}

int
AppWindow_add_draw_routine (AppWindow *this, DrawFunction *func)
{
	bb_queue_add(this->drawing_routines, func);
	return bb_queue_get_length(this->drawing_routines) - 1;
}

void
AppWindow_set_state (AppWindow *window, int state)
{
	window->app_state = state;
}

void
AppWindow_main (AppWindow *this)
{
	void AppWindow_draw ()
	{
		glLoadIdentity ();

		draw_axes (this->view);

		if (this->app_state != -1)
		{
			if (this->last_app_state != this->app_state)
			{
				this->draw = bb_queue_pick_nth (this->drawing_routines, this->app_state);
			}

			draw_function_call (this->draw);
		}

		this->last_app_state = this->app_state;
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
					glViewport(0, 0, event.size.width, event.size.height);
				}
				break;
			}
		}

		// Keyboard
		if (sfKeyboard_isKeyPressed(sfKeyUp))
		{
			this->view[Y_AXIS] += 5.0;
		}

		if (sfKeyboard_isKeyPressed(sfKeyDown))
		{
			this->view[Y_AXIS] -= 5.0;
		}

		if (sfKeyboard_isKeyPressed(sfKeyLeft))
		{
			this->view[X_AXIS] -= 5.0;
		}

		if (sfKeyboard_isKeyPressed(sfKeyRight))
		{
			this->view[X_AXIS] += 5.0;
		}

		if (sfKeyboard_isKeyPressed(sfKeyPageUp))
		{
			this->view[Z_AXIS] += 0.5;
		}

		if (sfKeyboard_isKeyPressed(sfKeyPageDown))
		{
			this->view[Z_AXIS] -= 0.5;
		}

		if (sfKeyboard_isKeyPressed(sfKeyR))
		{
			this->view[X_AXIS] = 0.0;
			this->view[Y_AXIS] = 0.0;
		}
	}

	void AppWindow_clear ()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear Screen And Depth Buffer
		sfRenderWindow_clear(SFML(this), sfBlack);
	}

	// Loop
	while (sfRenderWindow_isOpen(SFML(this)))
	{
		// Input
		AppWindow_input();

		// Clear
		AppWindow_clear();

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
