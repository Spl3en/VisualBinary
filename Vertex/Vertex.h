// --- Author : Moreau Cyril - Spl3en
// --- Date   : 2013-08-15-15.31.03
#pragma once

// ---------- Includes ------------
#include <math.h>

// ---------- Defines -------------


// ------ Class declaration -------
typedef struct _Vertex
{
	float x, y, z;
	int value;

}	Vertex;

typedef struct _VertexList
{
	Vertex *data;
	unsigned int size;
	unsigned int index;

} VertexList;



// --------- Constructors ---------

Vertex *
vertex_new (float x, float y, float z);

Vertex *
vertex_alloc (void);

VertexList *
vertex_list_new (unsigned int size);

VertexList *
vertex_list_alloc (void);

// ----------- Methods ------------

void
vertex_init (Vertex *vertex, float x, float y, float z);

void
vertex_list_init (VertexList *this, unsigned int size);

void
_vertex_list_add_vertex (VertexList *this, Vertex *v);

void
vertex_list_add_vertex (VertexList *this, Vertex *v);

float
vertex_distance (Vertex *v1, Vertex *v2);

// --------- Destructors ----------

void
vertex_free (Vertex *vertex);


