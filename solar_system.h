#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include "object.h"
#include "util.h"

float local_rotate = 0;
float global_rotate[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void drawCircle (float radius)
{
	glBegin (GL_LINE_LOOP);
		int i;
		for (i=0; i < 360; i++)
		{
			float angle = i*DEG_TO_RAD;
			glVertex3f (cos(angle)*radius , 0, sin(angle)*radius);
		}
	glEnd();
}

void drawSolarSystem (object_t *this)
{
	glPushMatrix();

	glTranslatef (this->pos_x, this->pos_y, this->pos_z);
	glRotatef (45, 1, 0, 0);
	
	// mercurio
	glColor3f (0.5, 0.5, 0.5);
	drawCircle (1.3);
	glPushMatrix();
		glColor3f(0.0, 0.6, 1.0);
		glRotatef (global_rotate[0], 0.0, 1.0, 0.0);
		glTranslatef (1.3, 0.0, 0.0);
		glRotatef (local_rotate, 0.0, 1.0, 0.0);
		glutSolidSphere (0.1, 20, 16);
	glPopMatrix();

	// venere
	glColor3f (0.5, 0.5, 0.5);
	drawCircle (1.8);
	glPushMatrix();
		glColor3f (1.0, 0.3, 0.0);
		glRotatef (global_rotate[1], 0.0, 1.0, 0.0);
		glTranslatef (1.8, 0.0, 0.0);
		glRotatef (local_rotate, 0.0, 1.0, 0.0);
		glutSolidSphere (0.13, 20, 16);
	glPopMatrix();

	// terra
	glColor3f (0.5, 0.5, 0.5);
	drawCircle (2.1);
	glPushMatrix();
		glColor3f (0.0, 0.5, 0.3);
		glRotatef (global_rotate[2], 0.0, 1.0, 0.0);
		glTranslatef (2.1, 0.0, 0.0);
		glRotatef (local_rotate, 0.0, 1.0, 0.0);
		glutSolidSphere (0.16, 20, 16);
		// luna
		glColor3f (1.0, 1.0, 1.0);
		glRotatef (global_rotate[2], 0.0, 1.0, 0.0);
		glTranslatef (0.2, 0.0, 0.0);
		glutSolidSphere (0.05, 20, 16);
	glPopMatrix();

	// marte
	glColor3f (0.5, 0.5, 0.5);
	drawCircle (2.6);
	glPushMatrix();
		glColor3f (1.0, 0.0, 0.0);
		glRotatef (global_rotate[3], 0.0, 1.0, 0.0);
		glTranslatef (2.6, 0.0, 0.0);
		glRotatef (local_rotate, 0.0, 1.0, 0.0);
		glutSolidSphere (0.2, 20, 16);
	glPopMatrix();

	// giove
	glColor3f (0.5, 0.5, 0.5);
	drawCircle (4.0);
	glPushMatrix();
		glColor3f (0.3, 0.3, 0.3);
		glRotatef (global_rotate[4], 0.0, 1.0, 0.0);
		glTranslatef (4.0, 0.0, 0.0);
		glRotatef (local_rotate, 0.0, 1.0, 0.0);
		glutSolidSphere (0.5, 20, 16);
	glPopMatrix();

	// saturno
	glColor3f (0.5, 0.5, 0.5);
	drawCircle (6.0);
	glPushMatrix();
		glColor3f (0.6, 0.4, 0.3);
		glRotatef (global_rotate[5], 0.0, 1.0, 0.0);
		glTranslatef (6.0, 0.0, 0.0);
		glRotatef (60, 1.0, 0.0, 0.0);
		glutSolidTorus (0.05, 0.55, 8, 12);
		glRotatef (local_rotate, 0.0, 1.0, 0.0);
		glutSolidSphere (0.4, 20, 16);
	glPopMatrix();

	// urano
	glColor3f (0.5, 0.5, 0.5);
	drawCircle (7.0);
	glPushMatrix();
		glColor3f (0.0, 0.2, 1.0);
		glRotatef (global_rotate[6], 0.0, 1.0, 0.0);
		glTranslatef (7.0, 0.0, 0.0);
		glRotatef (local_rotate, 0.0, 1.0, 0.0);
		glutSolidSphere (0.3, 20, 16);
	glPopMatrix();

	// nettuno
	glColor3f (0.5, 0.5, 0.5);
	drawCircle (8.0);
	glPushMatrix();
		glColor3f (0.0, 0.0, 1.0);
		glRotatef (global_rotate[7], 0.0, 1.0, 0.0);
		glTranslatef (8.0, 0.0, 0.0);
		glRotatef (local_rotate, 0.0, 1.0, 0.0);
		glutSolidSphere (0.35, 20, 16);
	glPopMatrix();

	// sole
	glColor3f(1.0, 0.3, 0.0);
	glutSolidSphere (0.8, 32, 28);

	glPopMatrix();
}

void updateSolarSystem (object_t *this)
{
	local_rotate = (local_rotate - 1);
	if (local_rotate < 0)
		local_rotate = 360;

	int i;
	for (i=0; i<8; i++)
	{
		global_rotate[i] = global_rotate[i] + 0.1/(i+1);
		if (global_rotate[i] > 360)
			global_rotate[i] = 0;
	}
}
