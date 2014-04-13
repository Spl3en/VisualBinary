#include "CloudPoints.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

CloudPoints *
CloudPoints_new (int pointsCount)
{
	CloudPoints *this;

	if ((this = CloudPoints_alloc()) == NULL)
		return NULL;

	CloudPoints_init(this, pointsCount);

	return this;
}

void
CloudPoints_init (CloudPoints *this, int pointsCount)
{
	this->cIndex = 0;
	this->vIndex = 0;
	this->vSize  = pointsCount * 3;
	this->cSize  = pointsCount * 4;
	this->vertices = malloc(sizeof(float) * this->vSize);
	this->colors   = malloc(sizeof(float) * this->cSize);
	this->verticeCount = pointsCount;
}

CloudPoints * CloudPoints_alloc (void) {
	return calloc (1, sizeof(CloudPoints));
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

	this->colors[this->cIndex++] = r;
	this->colors[this->cIndex++] = g;
	this->colors[this->cIndex++] = b;
	this->colors[this->cIndex++] = opacity;
}

void
CloudPoints_add_vertice (CloudPoints *this, float x, float y, float z)
{
	this->vertices[this->vIndex++] = x;
	this->vertices[this->vIndex++] = y;
	this->vertices[this->vIndex++] = z;
}

void
CloudPoints_free (CloudPoints *CloudPoints)
{
	if (CloudPoints != NULL)
	{
		free (CloudPoints);
	}
}
