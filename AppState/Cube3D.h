// --- Author : Moreau Cyril - Spl3en
// --- Date   : 2013-08-11-17.31.46
#pragma once

// ---------- Includes ------------
#include "GraphicObjects/GraphicObjects.h"
#include "Randomizer/Randomizer.h"
#include "Analyzer/Analyzer.h"
#include "Sleep/Sleep.h"
#include "CloudPoints/CloudPoints.h"
#include "AppWindow/AppWindow.h"
#include "Vertex/Vertex.h"
#include "Utils/Utils.h"
#include "Ztring/Ztring.h"

#include <GL/glew.h>
#include <SFML/OpenGL.h>
#include <SFML/Graphics.h>

// ---------- Defines -------------
#define SPACE_BETWEEN_FRAMES 1

typedef enum {

	CUBE3D_SPACE = 0,
	CUBE3D_TIME,
	CUBE3D_FFT,
	CUBE3D_FFT_FRAMES,

	// Always at the end
	CUBE3D_STATE_SIZE

} CubeState;

typedef enum {

	CUBE3D_ROTATE_AUTO,
	CUBE3D_ROTATE_STOP

} CubeRotateState;

typedef enum {

	CUBE3D_CINEMATIC_PLAY,
	CUBE3D_CINEMATIC_STOP

} CubeCinematicState;


typedef struct
{
	float xrot, yrot;
	CubeRotateState state;

} 	CubeRotation;

typedef struct
{
	float xleft;
	float yleft;
	CubeCinematicState state;

}	CubeCinematic;

// ------ Class declaration -------
typedef struct _Cube3D
{
	sfRenderWindow* render;

	Analyzer *analyzer;

	// Display data
	CloudPoints *cloud[CUBE3D_STATE_SIZE];
	GLuint vboID[CUBE3D_STATE_SIZE][2];
	GLuint vaoID[CUBE3D_STATE_SIZE][1];
	GLuint shaderProgram;
	float projection_matrix[16];
	float model_matrix[16];

	// Bounds
	float start_limit;
	float end_limit;

	// Text
	sfFont *font;

	// States
	CubeState state;
	CubeRotation rotation;

	CubeCinematic cinematic;

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
cube3d_load_cloud (Cube3D *this, CubeState state);

void
cube3d_draw (Cube3D *this, float *view);

void
cube3d_update (Cube3D *this, float *view);

CloudPoints *
cube3d_compute_cloud (Cube3D *this, CubeState state);

void
cube3d_input (Cube3D *this);

void
cube3d_set_rot (Cube3D *this, float xrot, float yrot);

char *
cube3D_get_state_str (Cube3D *this);

void
cube3d_enable_state (Cube3D *this, CubeState state);

bool
cube3d_view_changed (Cube3D *this, float *view);

// --------- Destructors ----------

void
cube3d_free (Cube3D *cube3d);


