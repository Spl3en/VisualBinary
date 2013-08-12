#include "GraphicObjects.h"
#include <SFML/OpenGL.h>

void draw_square (float x, float y, float z, float width, float height)
{
	glBegin (GL_POLYGON);
		glVertex3f (x, y, z);
		glVertex3f (x, y + height, z);
		glVertex3f (x + width, y + height, z);
		glVertex3f (x + width, y, z);
	glEnd ();
}

void draw_point (float x, float y, float z)
{
	glBegin(GL_POINTS);
		glVertex3f(x, y, z);
	glEnd();
}

void draw_axes (float view[3])
{
	static float ORG[3] = {0,0,0};
	static float XP[3]  = {1,0,0},
				 YP[3]  = {0,1,0},
				 ZP[3]  = {0,0,1};

	glPushMatrix ();

	glTranslatef (0.0, 0.0, view[2]);
	glRotatef (view[1], 1,0,0);
	glRotatef (view[0], 0,1,0);

	glLineWidth (5.0);

	glBegin (GL_LINES);
	glColor3f (1,0,0); // X axis is red.
	glVertex3fv (ORG);
	glVertex3fv (XP );
	glColor3f (0,1,0); // Y axis is green.
	glVertex3fv (ORG);
	glVertex3fv (YP );
	glColor3f (0,0,1); // z axis is blue.
	glVertex3fv (ORG);
	glVertex3fv (ZP );
	glEnd();

	glPopMatrix ();
}

void draw_cube (float x, float y, float z, float sizex, float sizey, float sizez)
{
	glPushMatrix();

    glTranslatef(-x, -y, -z);

    glBegin(GL_QUADS);

    // BACK
    glVertex3f(-sizex, -sizey, -sizez);
    glVertex3f(-sizex, sizey, -sizez);
    glVertex3f(sizex, sizey, -sizez);
    glVertex3f(sizex, -sizey, -sizez);

    // FRONT
    glVertex3f(-sizex, -sizey, sizez);
    glVertex3f(sizex, -sizey, sizez);
    glVertex3f(sizex, sizey, sizez);
    glVertex3f(-sizex, sizey, sizez);

    // LEFT
    glVertex3f(-sizex, -sizey, sizez);
    glVertex3f(-sizex, sizey, sizez);
    glVertex3f(-sizex, sizey, -sizez);
    glVertex3f(-sizex, -sizey, -sizez);

    // RIGHT
    glVertex3f(sizex, -sizey, -sizez);
    glVertex3f(sizex, sizey, -sizez);
    glVertex3f(sizex, sizey, sizez);
    glVertex3f(sizex, -sizey, sizez);

    // TOP
    glVertex3f(-sizex, sizey, sizez);
    glVertex3f(sizex, sizey, sizez);
    glVertex3f(sizex, sizey, -sizez);
    glVertex3f(-sizex, sizey, -sizez);

    // BOTTOM
    glVertex3f(-sizex, -sizey, sizez);
    glVertex3f(-sizex, -sizey, -sizez);
    glVertex3f(sizex, -sizey, -sizez);
    glVertex3f(sizex, -sizey, sizez);

    glEnd();

	glPopMatrix();
}
