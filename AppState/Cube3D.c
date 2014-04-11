#include "Cube3D.h"
#include <stdlib.h>
#include <stdio.h>
#include <SFML/OpenGL.h>

Cube3D *
cube3d_new (sfRenderWindow* render, Analyzer *analyzer)
{
	Cube3D *this;

	if ((this = cube3d_alloc()) == NULL)
		return NULL;

	cube3d_init(this, render, analyzer);
	//cube3d_init_vertex_array(this);
	cube3d_init_list(this);
	//cube3d_init_pbo(this);

	return this;
}

Cube3D *
cube3d_alloc (void)
{
	return calloc(1, sizeof(Cube3D));
}

void
cube3d_init (Cube3D *this, sfRenderWindow* render, Analyzer *analyzer)
{
	this->analyzer    = analyzer;
	this->start_limit = 0.0;
	this->end_limit   = (float) (NB_FRAMES - 1);
	this->render      = render;
	this->font        = sfFont_createFromFile("verdana.ttf");
	this->state       = CUBE3D_SPACE;
	this->rotation    = (CubeRotation) {.xrot = 0.1, .yrot = 0.0, .state = CUBE3D_ROTATE_AUTO};
	this->cinematic   = (CubeCinematic) {.xleft = 360.0, .yleft = 360.0, .state = CUBE3D_CINEMATIC_STOP};

	for (CubeState state = 0; state < CUBE3D_STATE_SIZE; state++)
	{
		this->vertex_list[state] = vertex_list_new(NB_FRAMES);
	}
}

void
cube3d_set_rot (Cube3D *this, float xrot, float yrot)
{
	this->rotation.xrot = xrot;
	this->rotation.yrot = yrot;
}

void
cube3d_init_vertex_array (Cube3D *this)
{
	int x, y, z;
	unsigned int value;

	for (CubeState state = 0; state < CUBE3D_STATE_SIZE; state++)
	{
		VertexList *vertex_list = this->vertex_list[state];

		for (z = this->start_limit; z < this->end_limit; z++)
		{
			for (y = 0; y < NB_FRAMES; y++)
			{
				for (x = 0; x < NB_FRAMES; x++)
				{
					switch (state)
					{
						case CUBE3D_TIME:
							value = frame_get(&this->analyzer->frames_time[z], x, y);
						break;

						case CUBE3D_SPACE:
							value = frame_get(&this->analyzer->frames_space[z], x, y);
						break;

						case CUBE3D_FFT:
							value = frame_get(&this->analyzer->frames_fft[z], x, y);
						break;

						default:
						break;
					}

					if (value > 0)
					{
						Vertex *vertex = vertex_new((float) x / NB_FRAMES - 0.5, 1.0 - (float) y / NB_FRAMES - 0.5, (float) z / NB_FRAMES - 0.5);
						vertex_list_add_vertex(vertex_list, vertex);
					}
				}
			}
		}
	}
}

void
cube3d_init_pbo (Cube3D *this)
{
	glGenBuffersARB(CUBE3D_STATE_SIZE, this->pbo_geometry);

	for (CubeState state = 0; state < CUBE3D_STATE_SIZE; state++)
	{
		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, this->pbo_geometry[state]);

		switch (state)
		{
			case CUBE3D_SPACE:
				// glBufferDataARB(GL_ARRAY_BUFFER_ARB, dataSize, vertices, GL_STATIC_DRAW_ARB);
			break;

			case CUBE3D_TIME:
			break;

			default:
			break;
		}
	}
}

void
cube3d_init_list (Cube3D *this)
{
	for (CubeState state = 0; state < CUBE3D_STATE_SIZE; state++)
	{
		this->index[state] = glGenLists(1);
		glNewList(this->index[state], GL_COMPILE);
			cube3d_direct_draw(this, (float[3]) {0.0, 0.0, -5.0}, state);
		glEndList();
	}
}


void
cube3d_update (Cube3D *this)
{
	static int onChange[4] = {};
	if (this->cinematic.state == CUBE3D_CINEMATIC_PLAY)
	{
		cube3d_set_rot(this, 1.0, 0.0);

		if (this->cinematic.xleft > 120.0) {
			this->cinematic.xleft -= this->rotation.xrot;
			cube3d_enable_state(this, CUBE3D_SPACE);
		}
		else if (this->cinematic.xleft > -120.0) {
			if (onChange[0] == 0)
				onChange[0] = 1;
			else if (onChange[0] == 1)
			{
				onChange[0] = 2;
				Sleep(1000);
			}
			this->cinematic.xleft -= this->rotation.xrot;
			cube3d_enable_state(this, CUBE3D_TIME);
		}
		else if (this->cinematic.xleft > -480.0) {
			this->cinematic.xleft -= this->rotation.xrot;
			cube3d_enable_state(this, CUBE3D_FFT);
		}
		else {
			cube3d_set_rot(this, 0.0, 0.0);
			this->cinematic.xleft = 360.0;
			this->cinematic.yleft = 360.0;
			this->cinematic.state = CUBE3D_CINEMATIC_STOP;
		}
	}
}

void
cube3d_enable_state (Cube3D *this, CubeState state)
{
	this->state = state;

	switch (state)
	{
		case CUBE3D_SPACE:
		case CUBE3D_TIME:
			glDisable(GL_DEPTH_TEST);
		break;

		case CUBE3D_FFT:
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
		break;

		case CUBE3D_STATE_SIZE:
		break;
	}
}

void
cube3d_input (Cube3D *this)
{
	float speed_select = 10.0;

	// Start
	if (sfKeyboard_isKeyPressed(sfKeyAdd))
	{
		this->start_limit += speed_select;
		if (this->start_limit > 255.0)
			this->start_limit = 255.0;
		cube3d_generate_cube(this, this->state);
	}

	if (sfKeyboard_isKeyPressed(sfKeySubtract))
	{
		this->start_limit -= speed_select;
		if (this->start_limit < 0.0)
			this->start_limit = 0.0;
		cube3d_generate_cube(this, this->state);
	}

	// End
	if (sfKeyboard_isKeyPressed(sfKeyDivide))
	{
		this->end_limit -= speed_select;
		if (this->end_limit < 0.0)
			this->end_limit = 0.0;
		cube3d_generate_cube(this, this->state);
	}

	if (sfKeyboard_isKeyPressed(sfKeyMultiply))
	{
		this->end_limit += speed_select;
		if (this->end_limit > 255.0)
			this->end_limit = 255.0;
		cube3d_generate_cube(this, this->state);
	}

	// Space
	if (sfKeyboard_isKeyPressed(sfKeyS))
	{
		cube3d_enable_state(this, CUBE3D_SPACE);
	}

	if (sfKeyboard_isKeyPressed(sfKeyT))
	{
		cube3d_enable_state(this, CUBE3D_TIME);
	}

	if (sfKeyboard_isKeyPressed(sfKeyF))
	{
		cube3d_enable_state(this, CUBE3D_FFT);
	}

	if (sfKeyboard_isKeyPressed(sfKeyC))
	{
		// Enable Rotation
		this->rotation.state = CUBE3D_ROTATE_AUTO;
	}

	if (sfKeyboard_isKeyPressed(sfKeyX))
	{
		// Disable Rotation
		this->rotation.state = CUBE3D_ROTATE_STOP;
	}

	if (sfKeyboard_isKeyPressed(sfKeyP))
	{
		// Disable Rotation
		this->cinematic.state = CUBE3D_CINEMATIC_PLAY;
	}
}

void
cube3d_generate_cube (Cube3D *this, CubeState state)
{
	if (this->index[state] != 0)
		glDeleteLists(this->index[state], 1);

	this->index[state] = glGenLists(1);

	glNewList(this->index[state], GL_COMPILE);
		cube3d_direct_draw (this, (float[3]) {0.0, 0.0, -5.0}, state);
	glEndList();
}

void
cube3d_direct_draw (Cube3D *this, float view[3], CubeState state)
{
	int x, y, z;
	int value = 0;
	int maxvalue = 0;
	float opacity = 0.5;
	float r, g, b;
	int real, imag;

	// Get maxvalue of the cube
	switch (state)
	{
		case CUBE3D_TIME:
			maxvalue = this->analyzer->maxvalue_time;
		break;

		case CUBE3D_SPACE:
			maxvalue = this->analyzer->maxvalue_space;
		break;

		case CUBE3D_FFT:
			maxvalue = this->analyzer->maxvalue_fft;
		break;

		default:
		break;
	}

	// Draw points
	glPointSize(2.0);
	glBegin(GL_POINTS);
	for (z = this->start_limit; z < this->end_limit; z++)
	{
		for (y = 0; y < NB_FRAMES; y++)
		{
			for (x = 0; x < NB_FRAMES; x++)
			{
				switch (state)
				{
					case CUBE3D_TIME:
						value = frame_get(&this->analyzer->frames_time[z], x, y);
					break;

					case CUBE3D_SPACE:
						value = frame_get(&this->analyzer->frames_space[z], x, y);
					break;

					case CUBE3D_FFT:
						frame_get_complex(&this->analyzer->frames_fft[z], x, y, &real, &imag);
						real = abs(real);
						imag = abs(imag);
					break;

					default:
					break;
				}

				switch (state)
				{
					case CUBE3D_TIME:
					case CUBE3D_SPACE:
						if (value > 0)
						{
							if (value == 1)
								glColor4f(0.0, 1.0, 0.0, 0.05);
							else
							{
								r = ((float) value / (maxvalue+1)) * 8.0;
								g = ((float) value / (maxvalue+1)) * 32.0;
								b = ((float) value / (maxvalue+1)) * 16.0;

								glColor4f(r, g, b, opacity);
							}

							draw_point((float) x / NB_FRAMES - 0.5, 1.0 - (float) y / NB_FRAMES - 0.5, (float) z / NB_FRAMES - 0.5);

						}
					break;

					case CUBE3D_FFT:
					{
						if (real > 0 || imag > 0)
						{
							float threshold = 1 + (maxvalue / 3.0);
							if (threshold > 1.0)
								threshold = 1.0;

							r = (real > threshold) ? 1.0 : (real / threshold);
							b = (imag > threshold) ? 1.0 : (imag / threshold);
							g = (((real / 2.0) + (imag / 2.0)) > threshold) ?
								1.0 : ((real / 2.0) + (imag / 2.0)) / threshold;

							if (r > 0.0 || g > 0.0 || b > 0.0)
							{
								#define SPACE_BETWEEN_FRAMES 2
								glColor4f(r, g, b, opacity);
								draw_point (
										(float) x / NB_FRAMES - 0.5,
										1.0 - (float) y / NB_FRAMES - 0.5,
										(float) (SPACE_BETWEEN_FRAMES * z) / NB_FRAMES - (SPACE_BETWEEN_FRAMES * 0.5)
								);
								#undef SPACE_BETWEEN_FRAMES
							}
						}
					}
					break;

					default:
					break;
				}

			}
		}
	}
	glEnd();
}

void
cube3d_draw (Cube3D *this, float *view)
{
	glPushMatrix ();

	switch (this->rotation.state)
	{
		case CUBE3D_ROTATE_AUTO:
			view[0] += this->rotation.xrot;
			view[1] += this->rotation.yrot;
		break;

		case CUBE3D_ROTATE_STOP:
		break;
	}

	glTranslatef (0.0, 0.0, view[2]);
	glRotatef (view[1], 1,0,0);
	glRotatef (view[0], 0,1,0);

	glCallList(this->index[this->state]);

	glPopMatrix ();

	char buffer[1024];
	sfText *text = sfText_create();
	sfText_setFont(text, this->font);
	sfText_setCharacterSize(text, 20);
	// sprintf(buffer, "Start = 0x%d - End = 0x%d", (int) (this->start_limit * (float) this->analyzer->filesize / NB_FRAMES), (int) (this->end_limit * (float) this->analyzer->filesize / NB_FRAMES));
	sprintf(buffer, "View : %s", cube3D_get_state_str(this));
	sfText_setString(text, buffer);

	sfRenderWindow_pushGLStates(this->render);
	sfRenderWindow_drawText(this->render, text, NULL);
	sfRenderWindow_popGLStates(this->render);

	sfText_destroy(text);
}

char *
cube3D_get_state_str (Cube3D *this)
{
	char *state_str [CUBE3D_STATE_SIZE] = {
		[CUBE3D_SPACE] = "CUBE3D_SPACE",
		[CUBE3D_TIME]  = "CUBE3D_TIME",
		[CUBE3D_FFT]   = "CUBE3D_FFT"
	};

	return state_str[this->state];
}

void
cube3d_free (Cube3D *cube3d)
{
	if (cube3d != NULL)
	{
		free (cube3d);
	}
}
