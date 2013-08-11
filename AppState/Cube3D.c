#include "Cube3D.h"
#include <stdlib.h>
#include <stdio.h>

Cube3D *
cube3d_new (void)
{
	Cube3D *this;

	if ((this = cube3d_alloc()) == NULL)
		return NULL;

	cube3d_init(this);

	return this;
}

Cube3D *
cube3d_alloc (void)
{
	return calloc(1, sizeof(Cube3D));
}

void
cube3d_init (Cube3D *this)
{

}

void
cube3d_draw (Cube3D *this, float *view)
{
	draw_square(view, 0.0, 0.0, 1.0, 1.0);
}

void
cube3d_free (Cube3D *cube3d)
{
	if (cube3d != NULL)
	{
		free (cube3d);
	}
}
