#include "GraphicObjects.h"
#include <SFML/OpenGL.h>

void draw_square (float x, float y, float width, float height)
{
	glBegin (GL_POLYGON);
		glVertex3f (x, y, 0.0);
		glVertex3f (x, y + height, 0.0);
		glVertex3f (x + width, y + height, 0.0);
		glVertex3f (x + width, y, 0.0);
	glEnd ();
}

void draw_axes (float vector[3])
{
	float ORG[3] = {0,0,0};
	float XP[3]  = {1,0,0},
		  YP[3]  = {0,1,0},
		  ZP[3]  = {0,0,1};

	glPushMatrix ();

	glTranslatef (-2.0, -1.5, -5);
	glRotatef (vector[1], 1,0,0);
	glRotatef (vector[0], 0,1,0);
	glScalef (0.25, 0.25, 0.25);

	glLineWidth (2.0);

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
