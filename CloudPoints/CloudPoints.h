// --- Author : Moreau Cyril - Spl3en
// --- Date   : 2014-04-12-23.14.46
#pragma once

// ---------- Includes ------------
#include "../BbQueue/BbQueue.h"

// ---------- Defines -------------


// ------ Class declaration -------
typedef struct _CloudPoints
{
	float *vertices;
	float *colors;
	int cSize;
	int vSize;
	int vIndex;
	int cIndex;

	int verticeCount;

}	CloudPoints;



// --------- Constructors ---------

CloudPoints *
CloudPoints_new (int pointsCount);

CloudPoints *
CloudPoints_alloc (void);

// ----------- Methods ------------

void
CloudPoints_init (CloudPoints *CloudPoints, int pointsCount);

void
CloudPoints_add_color (CloudPoints *this, float r, float g, float b, float opacity);

void
CloudPoints_add_vertice (CloudPoints *this, float x, float y, float z);

void
CloudPoints_convert_queue (CloudPoints *this);

// --------- Destructors ----------

void
CloudPoints_free (CloudPoints *CloudPoints);


