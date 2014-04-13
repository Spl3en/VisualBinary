#include "Vertex.h"
#include <stdlib.h>
#include <stdio.h>

Vertex *
vertex_new (float x, float y, float z)
{
	Vertex *this;

	if ((this = vertex_alloc()) == NULL)
		return NULL;

	vertex_init(this, x, y, z);

	return this;
}

Vertex *
vertex_alloc (void)
{
	return calloc(1, sizeof(Vertex));
}

void
vertex_init (Vertex *this, float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float
vertex_distance (Vertex *v1, Vertex *v2)
{
	return sqrt (
		pow(v1->x - v2->x, 2)
	+	pow(v1->y - v2->y, 2)
	+	pow(v1->z - v2->z, 2)
	);
}

void
vertex_free (Vertex *vertex)
{
	if (vertex != NULL)
	{
		free (vertex);
	}
}

VertexList *
vertex_list_new (unsigned int size)
{
	VertexList *this;

	if ((this = vertex_list_alloc()) == NULL)
		return NULL;

	vertex_list_init(this, size);

	return this;
}

VertexList *
vertex_list_alloc (void)
{
	return calloc(1, sizeof(VertexList));
}

void
vertex_list_init (VertexList *this, unsigned int size)
{
	this->index = 0;
	this->size = size;
	this->data = calloc(sizeof(Vertex), size);
}

void
vertex_list_add_vertex (VertexList *this, Vertex *v)
{
	if (this->index > this->size)
	{
		printf("%s: VertexList overflow (Index=%u, Size=%u)", __FUNCTION__, this->index, this->size);
		return;
	}

	_vertex_list_add_vertex(this, v);
}

void
_vertex_list_add_vertex (VertexList *this, Vertex *v)
{
	this->data[this->index++] = *v;
}

void
vertex_list_free (VertexList *vertex_array)
{
	if (vertex_array != NULL)
	{
		free (vertex_array);
	}
}
