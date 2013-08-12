// --- Author : Moreau Cyril - Spl3en
// --- Date   : 2013-08-11-17.31.46
#pragma once

// ---------- Includes ------------
#include "../GraphicObjects/GraphicObjects.h"
#include "../Randomizer/Randomizer.h"
#include "../Analyzer/Analyzer.h"
#include <SFML/OpenGL.h>
#include <SFML/Graphics.h>

// ---------- Defines -------------
typedef enum {

	CUBE3D_SPACE = 0,
	CUBE3D_TIME,




	// Always at the end
	CUBE3D_STATE_SIZE

} CubeState;

typedef enum {

	CUBE3D_ROTATE_AUTO,
	CUBE3D_ROTATE_STOP

} CubeRotateState;

typedef struct
{
	float xrot, yrot;
	CubeRotateState state;

} 	CubeRotation;

// ------ Class declaration -------
typedef struct _Cube3D
{
	sfRenderWindow* render;

	Analyzer *analyzer;

	// Display
	GLuint index[CUBE3D_STATE_SIZE];

	// Bounds
	float start_limit;
	float end_limit;

	// Text
	sfFont *font;

	// States
	CubeState state;
	CubeRotation rotation;

}	Cube3D;



// --------- Constructors ---------

Cube3D *
cube3d_new (sfRenderWindow* render, Analyzer *analyzer);

Cube3D *
cube3d_alloc (void);

// ----------- Methods ------------

void
cube3d_init (Cube3D *this, sfRenderWindow* render, Analyzer *analyzer);

void
cube3d_draw (Cube3D *this, float *view);

void
cube3d_direct_draw (Cube3D *this, float *view, CubeState state);

void
cube3d_init_list (Cube3D *this);

void
cube3d_input (Cube3D *this);

void
cube3d_generate_cube (Cube3D *this, CubeState state);

// --------- Destructors ----------

void
cube3d_free (Cube3D *cube3d);


