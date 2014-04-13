#include "CloudPoints.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

CloudPoints *
CloudPoints_new (void)
{
	CloudPoints *this;

	if ((this = CloudPoints_alloc()) == NULL)
		return NULL;

	CloudPoints_init(this);

	return this;
}

CloudPoints *
CloudPoints_alloc (void)
{
	return calloc(1, sizeof(CloudPoints));
}

void
CloudPoints_add_color (CloudPoints *this, float r, float g, float b, float opacity)
{
	if (r > 1.0)
		r = 1.0;
	if (g > 1.0)
		g = 1.0;
	if (b > 1.0)
		b = 1.0;
	if (opacity > 1.0)
		opacity = 1.0;

	int32_t r32, g32, b32, opacity32;
	memcpy(&r32, &r, sizeof(int32_t));
	memcpy(&g32, &g, sizeof(int32_t));
	memcpy(&b32, &b, sizeof(int32_t));
	memcpy(&opacity32, &opacity, sizeof(int32_t));

	bb_queue_add_raw(&this->colors_queue, r32);
	bb_queue_add_raw(&this->colors_queue, g32);
	bb_queue_add_raw(&this->colors_queue, b32);
	bb_queue_add_raw(&this->colors_queue, opacity32);
}

void
CloudPoints_add_vertice (CloudPoints *this, float x, float y, float z)
{
	int32_t x32, y32, z32;
	memcpy(&x32, &x, sizeof(int32_t));
	memcpy(&y32, &y, sizeof(int32_t));
	memcpy(&z32, &z, sizeof(int32_t));
	bb_queue_add_raw(&this->vertices_queue, x32);
	bb_queue_add_raw(&this->vertices_queue, y32);
	bb_queue_add_raw(&this->vertices_queue, z32);
}

void
CloudPoints_convert_queue (CloudPoints *this)
{
	if (this->vertices != NULL)
		free(this->vertices);
	if (this->colors != NULL)
		free(this->colors);

	int vSize = bb_queue_get_length(&this->vertices_queue);
	int cSize = bb_queue_get_length(&this->colors_queue);

	printf("CloudPoints : vertices array allocated. Size = %d \n", vSize);
	printf("CloudPoints : colors array allocated.   Size = %d.\n", cSize);

	this->vertices = malloc(sizeof(float) * vSize);
	this->colors   = malloc(sizeof(float) * cSize);
	this->cSize = cSize;
	this->vSize = vSize;
	this->verticeCount = vSize / 3;

	int loop = 0;
	while (bb_queue_get_length(&this->vertices_queue))
	{
		int32_t vertice32 = (int32_t) bb_queue_get_first(&this->vertices_queue);
		float vertice;
		memcpy(&vertice, &vertice32, sizeof(float));

		this->vertices[loop++] = vertice;
	}

	loop = 0;
	while (bb_queue_get_length(&this->colors_queue))
	{
		int32_t color32 = (int32_t) bb_queue_get_first(&this->colors_queue);
		float color;
		memcpy(&color, &color32, sizeof(float));

		this->colors[loop++] = color;
	}
}

void
CloudPoints_init (CloudPoints *this)
{
	bb_queue_init(&this->vertices_queue);
	bb_queue_init(&this->colors_queue);
}

void
CloudPoints_free (CloudPoints *CloudPoints)
{
	if (CloudPoints != NULL)
	{
		free (CloudPoints);
	}
}
