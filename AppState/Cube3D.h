// --- Author : Moreau Cyril - Spl3en
// --- Date   : 2013-08-11-17.31.46
#pragma once

// ---------- Includes ------------
#include "../GraphicObjects/GraphicObjects.h"
#include "../Randomizer/Randomizer.h"
#include "../Frame/Frame.h"

// ---------- Defines -------------


// ------ Class declaration -------
typedef struct _Cube3D
{
	Frame frames[256];

}	Cube3D;



// --------- Constructors ---------

Cube3D *
cube3d_new (void);

Cube3D *
cube3d_alloc (void);

// ----------- Methods ------------

void
cube3d_init (Cube3D *cube3d);

void
cube3d_draw (Cube3D *this, float *view);

// --------- Destructors ----------

void
cube3d_free (Cube3D *cube3d);


