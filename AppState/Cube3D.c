#include "Cube3D.h"
#include <stdlib.h>
#include <stdio.h>
#include <SFML/OpenGL.h>

Cube3D *
cube3d_new (sfRenderWindow* render, Analyzer *analyzer)
{
	Cube3D *this;

	if ((this = cube3d_alloc ()) == NULL)
		return NULL;

	cube3d_init (this, render, analyzer);

	return this;
}

Cube3D *
cube3d_alloc (void)
{
	return calloc (1, sizeof (Cube3D));
}

void
cube3d_init (Cube3D *this, sfRenderWindow* render, Analyzer *analyzer)
{
	CubeState default_state = CUBE3D_SPACE;

	this->analyzer    = analyzer;
	this->start_limit = 0.0;
	this->end_limit   = (float) (NB_FRAMES);
	this->render      = render;
	this->font        = sfFont_createFromFile ("verdana.ttf");
	this->state       = default_state;
	this->rotation    = (CubeRotation) {.xrot = 0.1, .yrot = 0.0, .state = CUBE3D_ROTATE_AUTO};
	this->cinematic   = (CubeCinematic) {.xleft = 360.0, .yleft = 360.0, .state = CUBE3D_CINEMATIC_STOP};

	memset (this->vboID, APP_STATE_UNDEFINED, sizeof (this->vboID));
	memset (this->vaoID, APP_STATE_UNDEFINED, sizeof (this->vaoID));
	glGetFloatv (GL_PROJECTION_MATRIX, this->projection_matrix);

	cube3d_load_cloud (this, default_state);
}

void
cube3d_set_rot (Cube3D *this, float xrot, float yrot)
{
	this->rotation.xrot = xrot;
	this->rotation.yrot = yrot;
}

void
cube3d_load_cloud (Cube3D *this, CubeState state)
{
	this->cloud[state] = cube3d_compute_cloud (this, state);

	float *colors   = this->cloud[state]->colors;
	float *vertices = this->cloud[state]->vertices;

	int vSize = this->cloud[state]->vSize;
	int cSize = this->cloud[state]->cSize;

	// Create VAO
	glGenVertexArrays (1, this->vaoID[state]);
	glBindVertexArray (this->vaoID[state][0]);
	glGenBuffers (2, this->vboID[state]);

	// Vertices
	glBindBuffer (GL_ARRAY_BUFFER, this->vboID[state][0]);
	glBufferData (GL_ARRAY_BUFFER, vSize * sizeof (float) , vertices, GL_STATIC_DRAW);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray (0);

	// Colors
	glBindBuffer (GL_ARRAY_BUFFER, this->vboID[state][1]);
	glBufferData (GL_ARRAY_BUFFER, cSize * sizeof (float) , colors, GL_STATIC_DRAW);
    glVertexAttribPointer (1, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray (1);

	// Read shaders
	// Get path
	char *vert_path = strdup ("./shaders/cloudPoints.vert");
	char *frag_path = strdup ("./shaders/cloudPoints.frag");
	#ifdef WIN32
		char exePath [MAX_PATH];
		GetModuleFileName (NULL, exePath, sizeof(exePath));

		char * lastSlash = strrchr (exePath, '\\');
		char * exeName = (lastSlash != NULL) ? &lastSlash[1] : exePath;
		exeName [0] = '\0';
		vert_path = str_dup_printf ("%s/%s", exePath, vert_path);
		frag_path = str_dup_printf ("%s/%s", exePath, frag_path);
	#endif // WIN32
	const GLchar *vertexSource   = file_get_contents (vert_path);
    const GLchar *fragmentSource = file_get_contents (frag_path);

	// Create and compile the vertex shader
	GLint compiled;
    GLuint vertexShader = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vertexShader, 1, &vertexSource, NULL);
    glCompileShader (vertexShader);
    glGetShaderiv (vertexShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
		int length;
		glGetShaderiv (vertexShader, GL_INFO_LOG_LENGTH, &length);
		char *vertexInfoLog = (char *) malloc (length);
		glGetShaderInfoLog (vertexShader, length, &length, vertexInfoLog);
		printf ("Error : vertexInfoLog = %s\n", vertexInfoLog);
		free (vertexInfoLog);
		return;
    }

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader (fragmentShader);
    glGetShaderiv (fragmentShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
		int length;
		glGetShaderiv (fragmentShader, GL_INFO_LOG_LENGTH, &length);
		char *fragmentInfoLog = (char *) malloc (length);
		glGetShaderInfoLog (fragmentShader, length, &length, fragmentInfoLog);
		printf ("Error : fragmentInfoLog = %s\n", fragmentInfoLog);
		free (fragmentInfoLog);
		return;
    }

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram ();
    glAttachShader (shaderProgram, vertexShader);
    glAttachShader (shaderProgram, fragmentShader);
    glBindAttribLocation (shaderProgram, 0, "in_Position");
    glBindAttribLocation (shaderProgram, 1, "in_Color");
    glLinkProgram (shaderProgram);

    glGetProgramiv (shaderProgram, GL_LINK_STATUS, (int *) &compiled);
    if (!compiled)
    {
		int length;
		glGetProgramiv (shaderProgram, GL_INFO_LOG_LENGTH, &length);
		char *shaderProgramInfoLog = (char *) malloc (length);
		glGetProgramInfoLog (shaderProgram, length, &length, shaderProgramInfoLog);
		printf ("Error : shaderProgramInfoLog = %s\n", shaderProgramInfoLog);
		free (shaderProgramInfoLog);
		return;
    }

	glEnable (GL_VERTEX_PROGRAM_POINT_SIZE);
    glUseProgram (shaderProgram);
    this->shaderProgram = shaderProgram;

    glBindVertexArray (0);
}

void
cube3d_update (Cube3D *this, float *view)
{
	// Update rotation
	switch (this->rotation.state)
	{
		case CUBE3D_ROTATE_AUTO:
			view[0] += this->rotation.xrot;
			view[1] += this->rotation.yrot;
		break;

		case CUBE3D_ROTATE_STOP:
		break;
	}

	static int onChange = 0;
	if (this->cinematic.state == CUBE3D_CINEMATIC_PLAY)
	{
		cube3d_set_rot (this, 1.0, 0.0);

		if (this->cinematic.xleft > 120.0) {
			this->cinematic.xleft -= this->rotation.xrot;
			cube3d_enable_state (this, CUBE3D_SPACE);
		}
		else if (this->cinematic.xleft > -120.0) {
			if (onChange == 0)
				onChange = 1;
			else if (onChange == 1)
			{
				onChange = 2;
				Sleep (1000);
			}
			this->cinematic.xleft -= this->rotation.xrot;
			cube3d_enable_state (this, CUBE3D_TIME);
		}
		else if (this->cinematic.xleft > -480.0) {
			this->cinematic.xleft -= this->rotation.xrot;
			cube3d_enable_state (this, CUBE3D_FFT);
		}
		else {
			cube3d_set_rot (this, 0.0, 0.0);
			this->cinematic.xleft = 360.0;
			this->cinematic.yleft = 360.0;
			this->cinematic.state = CUBE3D_CINEMATIC_STOP;
		}
	}
}

void
cube3d_enable_state (Cube3D *this, CubeState state)
{
	this->state = state;

	switch (state)
	{
		case CUBE3D_SPACE:
		case CUBE3D_TIME:
			glDisable (GL_DEPTH_TEST);
		break;

		case CUBE3D_FFT:
			glEnable (GL_DEPTH_TEST);
			glDepthFunc (GL_LEQUAL);
		break;

		case CUBE3D_FFT_FRAMES:
			glEnable (GL_DEPTH_TEST);
			glDepthFunc (GL_LEQUAL);
		break;

		case CUBE3D_STATE_SIZE:
		break;
	}
}

void
cube3d_input (Cube3D *this)
{
	float speed_select = 10.0;

	if (sfKeyboard_isKeyPressed (sfKeyAdd))
	{
		this->start_limit += speed_select;
		if (this->start_limit > 255.0)
			this->start_limit = 255.0;
	}

	if (sfKeyboard_isKeyPressed (sfKeySubtract))
	{
		this->start_limit -= speed_select;
		if (this->start_limit < 0.0)
			this->start_limit = 0.0;
		// cube3d_generate_cube (this, this->state);
	}

	if (sfKeyboard_isKeyPressed (sfKeyDivide))
	{
		this->end_limit -= speed_select;
		if (this->end_limit < 0.0)
			this->end_limit = 0.0;
		// cube3d_generate_cube (this, this->state);
	}

	if (sfKeyboard_isKeyPressed (sfKeyMultiply))
	{
		this->end_limit += speed_select;
		if (this->end_limit > 255.0)
			this->end_limit = 255.0;
		// cube3d_generate_cube (this, this->state);
	}

	// Space
	if (sfKeyboard_isKeyPressed (sfKeyS))
	{
		cube3d_enable_state (this, CUBE3D_SPACE);
		if (this->vaoID[this->state][0] == APP_STATE_UNDEFINED)
			cube3d_load_cloud (this, this->state);
	}

	if (sfKeyboard_isKeyPressed (sfKeyT))
	{
		cube3d_enable_state (this, CUBE3D_TIME);
		if (this->vaoID[this->state][0] == APP_STATE_UNDEFINED)
			cube3d_load_cloud (this, this->state);
	}

	if (sfKeyboard_isKeyPressed (sfKeyF))
	{
		cube3d_enable_state (this, CUBE3D_FFT);
		if (this->vaoID[this->state][0] == APP_STATE_UNDEFINED)
			cube3d_load_cloud (this, this->state);
	}

	if (sfKeyboard_isKeyPressed (sfKeyC))
	{
		// Enable Rotation
		this->rotation.state = CUBE3D_ROTATE_AUTO;
	}

	if (sfKeyboard_isKeyPressed (sfKeyX))
	{
		// Disable Rotation
		this->rotation.state = CUBE3D_ROTATE_STOP;
	}

	if (sfKeyboard_isKeyPressed (sfKeyP))
	{
		// Disable Rotation
		this->cinematic.state = CUBE3D_CINEMATIC_PLAY;
	}
}

CloudPoints *
cube3d_compute_cloud (Cube3D *this, CubeState state)
{
	int x, y, z;
	int value = 0;
	int maxvalue = 0;
	float opacity = 1.0;
	float visibility = 0.0;
	float r, g, b;
	double real, imag;

	// Get maxvalue of the cube
	switch (state)
	{
		case CUBE3D_TIME:       maxvalue = this->analyzer->maxvalue_time;  break;
		case CUBE3D_SPACE:      maxvalue = this->analyzer->maxvalue_space; break;
		case CUBE3D_FFT:        maxvalue = this->analyzer->maxvalue_fft;   break;
		case CUBE3D_FFT_FRAMES: maxvalue = this->analyzer->maxvalue_fft;   break;
		case CUBE3D_STATE_SIZE: break;
	}

	// Count points
	unsigned int pointsCount = 0;
	for (z = this->start_limit; z < this->end_limit; z++)
	{
		for (y = 0; y < NB_FRAMES; y++)
		{
			for (x = 0; x < NB_FRAMES; x++)
			{
				switch (state)
				{
					case CUBE3D_TIME:
						value = frame_get (&this->analyzer->frames_time[z], x, y);
					break;

					case CUBE3D_SPACE:
						value = frame_get (&this->analyzer->frames_space[z], x, y);
					break;

					case CUBE3D_FFT:
						frame_get_complex (&this->analyzer->frames_fft[z], x, y, &real, &imag);
						real = fabs (real) * 100;
						imag = fabs (imag) * 100;
					break;

					case CUBE3D_FFT_FRAMES:
						frame_get_complex (&this->analyzer->frames_fft[z], x, y, &real, &imag);
						real = fabs (real) * 100;
						imag = fabs (imag) * 100;
					break;

					case CUBE3D_STATE_SIZE:
					break;
				}

				switch (state)
				{
					case CUBE3D_TIME:
					case CUBE3D_SPACE:
						if (value > 0)
							pointsCount++;
					break;

					case CUBE3D_FFT:
						if (real > 1 || imag > 1)
							pointsCount++;
					break;

					case CUBE3D_FFT_FRAMES:
						if (real > 0 || imag > 0)
							pointsCount++;
					break;

					case CUBE3D_STATE_SIZE:break;
				}
			}
		}
	}

	printf ("Creating cloud points with %d vertices ...\n", pointsCount);
	CloudPoints *cloud = CloudPoints_new (pointsCount);

	// Process points
	for (z = this->start_limit; z < this->end_limit; z++)
	{
		for (y = 0; y < NB_FRAMES; y++)
		{
			for (x = 0; x < NB_FRAMES; x++)
			{
				switch (state)
				{
					case CUBE3D_TIME:
						value = frame_get (&this->analyzer->frames_time[z], x, y);
					break;

					case CUBE3D_SPACE:
						value = frame_get (&this->analyzer->frames_space[z], x, y);
					break;

					case CUBE3D_FFT:
						frame_get_complex (&this->analyzer->frames_fft[z], x, y, &real, &imag);
						real = fabs (real) * 100;
						imag = fabs (imag) * 100;
					break;

					case CUBE3D_FFT_FRAMES:
						frame_get_complex (&this->analyzer->frames_fft[z], x, y, &real, &imag);
						real = fabs (real) * 100;
						imag = fabs (imag) * 100;
					break;

					case CUBE3D_STATE_SIZE:break;
				}

				switch (state)
				{
					case CUBE3D_TIME:
					case CUBE3D_SPACE:
						visibility = ((float) (value) / (maxvalue+1));
						if (visibility > 0.00)
						{
							r = visibility * value * x;
							g = visibility * value * y;
							b = visibility * value * z;

							CloudPoints_add_color (cloud, r, g, b, 0.1);

							CloudPoints_add_vertice (cloud,
								 (float) x / NB_FRAMES - 0.5,
								1.0 - (float) y / NB_FRAMES - 0.5,
								 (float) z / NB_FRAMES - 0.5
							);
						}
					break;

					case CUBE3D_FFT:
						if (real > 1 || imag > 1)
						{
							r = (real > 1) ? 1 : real;
							b = (imag > 1) ? 1 : imag;
							g = (( (real / 2) + (imag / 2)) > 1) ?
								1 : ((real / 2) + (imag / 2));

							CloudPoints_add_color (cloud, r, g, b, opacity);

							CloudPoints_add_vertice (cloud,
									 (float) x / NB_FRAMES - 0.5,
									1.0 - (float) y / NB_FRAMES - 0.5,
									 (float) (SPACE_BETWEEN_FRAMES * z) / NB_FRAMES - (SPACE_BETWEEN_FRAMES * 0.5)
							);
						}
					break;

					case CUBE3D_FFT_FRAMES:
					break;

					case CUBE3D_STATE_SIZE: break;
				}

			}
		}
	}

	switch (state)
	{
		case CUBE3D_TIME:       free (this->analyzer->frames_time);  this->analyzer->frames_time  = NULL; break;
		case CUBE3D_SPACE:      free (this->analyzer->frames_space); this->analyzer->frames_space = NULL; break;
		case CUBE3D_FFT:        free (this->analyzer->frames_fft);   this->analyzer->frames_fft   = NULL; break;
		case CUBE3D_FFT_FRAMES: /* shared with FFT */ break;

		case CUBE3D_STATE_SIZE:break;
	}

	return cloud;
}

bool
cube3d_view_changed (Cube3D *this, float *view)
{
	static float last_view[3] = {0.0, 0.0, 0.0};
	static CubeState last_state = CUBE3D_SPACE;

	if (last_view[0] != view[0]
	||  last_view[1] != view[1]
	||  last_view[2] != view[2]
	||  last_state   != this->state)
	{
		last_view[0] = view[0];
		last_view[1] = view[1];
		last_view[2] = view[2];
		last_state   = this->state;

		return true;
	}

	return false;
}

void
cube3d_draw (Cube3D *this, float *view)
{
	CubeState state = this->state;

	glTranslatef (0.0, 0.0, view[2]);
	glRotatef (view[1], 1,0,0);
	glRotatef (view[0], 0,1,0);

	//! ------ Start of the program
	glUseProgram (this->shaderProgram);

	if (cube3d_view_changed (this, view)) {
		glGetFloatv (GL_MODELVIEW_MATRIX, this->model_matrix);
		glUniformMatrix4fv (glGetUniformLocation (this->shaderProgram, "MVMatrix") , 1, GL_FALSE, this->model_matrix);
		glUniformMatrix4fv (glGetUniformLocation (this->shaderProgram, "MPMatrix") , 1, GL_FALSE, this->projection_matrix);
	}

	glBindVertexArray (this->vaoID[state][0]);
	glDrawArrays (GL_POINTS, 0, this->cloud[state]->verticeCount);
	glBindVertexArray (0);

	//! ------- End of the program
	glUseProgram (0);

}

char *
cube3D_get_state_str (Cube3D *this)
{
	char *state_str [CUBE3D_STATE_SIZE] = {
		[CUBE3D_SPACE] = "CUBE3D_SPACE",
		[CUBE3D_TIME]  = "CUBE3D_TIME",
		[CUBE3D_FFT]   = "CUBE3D_FFT"
	};

	if (this->state > sizeof_array (state_str)) {
		printf ("Error : Unknown state.\n");
	}

	return state_str[this->state];
}

void
cube3d_free (Cube3D *cube3d)
{
	if (cube3d != NULL)
	{
		free (cube3d);
	}
}
