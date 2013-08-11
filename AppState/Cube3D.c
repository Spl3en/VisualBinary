#include "Cube3D.h"
#include <stdlib.h>
#include <stdio.h>
#include <SFML/OpenGL.h>

Cube3D *
cube3d_new (Frame *frames)
{
	Cube3D *this;

	if ((this = cube3d_alloc()) == NULL)
		return NULL;

	cube3d_init(this, frames);

	return this;
}

Cube3D *
cube3d_alloc (void)
{
	return calloc(1, sizeof(Cube3D));
}

void
cube3d_init (Cube3D *this, Frame *frames)
{
	this->frames = frames;
}

void
cube3d_draw (Cube3D *this, float *view)
{
	int x, y, z;
	int value;
	glPushMatrix ();

	glTranslatef (0.0, 0.0, view[2]);
	glRotatef (view[1], 1,0,0);
	glRotatef (view[0], 0,1,0);

	for (z = 0; z < 256; z++)
	{
		for (y = 0; y < 256; y++)
		{
			for (x = 0; x < 256; x++)
			{
				value = frame_get(&this->frames[z], x, y);

				if (value > 0)
				{
					glColor3f(z / 256.0, (z / 128.0) - 0.5, 1.0 - z / 256.0);
					draw_point (view, x / 256.0, y / 256.0, z / 256.0);
				}
			}
		}
	}

	glPopMatrix ();
}

void
cube3d_free (Cube3D *cube3d)
{
	if (cube3d != NULL)
	{
		free (cube3d);
	}
}
