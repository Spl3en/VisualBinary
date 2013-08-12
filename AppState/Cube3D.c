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
	cube3d_init_list(this);

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
	this->analyzer = analyzer;
	this->start_limit = 0.0;
	this->end_limit = 256.0;
	this->render = render;
	this->font = sfFont_createFromFile("verdana.ttf");
	this->state = CUBE3D_TIME;
	this->rotation = (CubeRotation) {.xrot = 0.1, .yrot = 0.0, .state = CUBE3D_ROTATE_AUTO};
}

void
cube3d_init_list (Cube3D *this)
{
	for (int i = 0; i < CUBE3D_STATE_SIZE - 1; i++)
	{
		this->index[i] = glGenLists(1);
		glNewList(this->index[i], GL_COMPILE);
		cube3d_direct_draw(this, (float[]) {0.0, 0.0, 0.0}, i);
		glEndList();
	}
}

void
cube3d_input (Cube3D *this)
{
	float speed = 2.5;

	// Start
	int last_start_limit = (int) this->start_limit;

	if (sfKeyboard_isKeyPressed(sfKeyAdd))
		this->start_limit += speed;

	if (sfKeyboard_isKeyPressed(sfKeySubtract))
		this->start_limit -= speed;

	if (this->start_limit < 0.0)
		this->start_limit = 0.0;

	if (this->start_limit > 255.0)
		this->start_limit = 255.0;

	if (last_start_limit != (int) this->start_limit)
	{
		this->index[this->state] = glGenLists(1);
		glNewList(this->index[this->state], GL_COMPILE);
		cube3d_direct_draw(this, (float[]) {0.0, 0.0, -5.0}, this->state);
		glEndList();
	}

	// End
	int last_end_limit = (int) this->end_limit;

	if (sfKeyboard_isKeyPressed(sfKeyMultiply))
		this->end_limit += speed;

	if (sfKeyboard_isKeyPressed(sfKeyDivide))
		this->end_limit -= speed;

	if (this->end_limit < 0.0)
		this->end_limit = 0.0;

	if (this->end_limit > 255.0)
		this->end_limit = 255.0;

	if (last_end_limit != (int) this->end_limit)
	{
		cube3d_generate_cube(this, this->state);
	}

	// Space
	if (sfKeyboard_isKeyPressed(sfKeyS))
	{
		// Enable Space Cube
		this->state = CUBE3D_SPACE;
		cube3d_generate_cube(this, this->state);
	}

	if (sfKeyboard_isKeyPressed(sfKeyT))
	{
		// Enable Time Cube
		this->state = CUBE3D_TIME;
		cube3d_generate_cube(this, this->state);
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
}

void
cube3d_generate_cube (Cube3D *this, CubeState state)
{
	this->index[state] = glGenLists(1);
	glNewList(this->index[state], GL_COMPILE);
	cube3d_direct_draw(this, (float[]) {0.0, 0.0, -5.0}, state);
	glEndList();
}

void
cube3d_direct_draw (Cube3D *this, float *view, CubeState state)
{
	int x, y, z;
	int value;
	int maxvalue;

	// Get maxvalue of the cube
	switch (state)
	{
		case CUBE3D_TIME:
			maxvalue = this->analyzer->maxvalue_time;
		break;

		case CUBE3D_SPACE:
			maxvalue = this->analyzer->maxvalue_space;
		break;

		default:
		break;
	}

	// Draw points
	glPointSize(3.0);
	for (z = this->start_limit; z < this->end_limit; z++)
	{
		for (y = 0; y < 256; y++)
		{
			for (x = 0; x < 256; x++)
			{
				switch (state)
				{
					case CUBE3D_TIME:
						value = frame_get(&this->analyzer->frames_time[z], x, y);
					break;

					case CUBE3D_SPACE:
						value = frame_get(&this->analyzer->frames_space[z], x, y);
					break;

					default:
					break;
				}

				if (value > 0)
				{
					value += 1;

					float r = ((float) value / maxvalue) * 8.0;
					float g = ((float) value / maxvalue) * 32.0;
					float b = ((float) value / maxvalue) * 16.0;

					glColor3f(r, g, b);
					draw_point(x / 256.0 - 0.5, 1.0 - y / 256.0 - 0.5, z / 256. - 0.5);
				}
			}
		}
	}
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
	sprintf(buffer, "Start = 0x%d - End = 0x%d", (int) (this->start_limit * this->analyzer->filesize / 256.0), (int) (this->end_limit * this->analyzer->filesize / 256.0));
	sfText_setString(text, buffer);

	sfRenderWindow_pushGLStates(this->render);
	sfRenderWindow_drawText(this->render, text, NULL);
	sfRenderWindow_popGLStates(this->render);

	sfText_destroy(text);
}

void
cube3d_free (Cube3D *cube3d)
{
	if (cube3d != NULL)
	{
		free (cube3d);
	}
}
