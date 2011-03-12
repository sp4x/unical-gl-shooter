#include <GL/glut.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scene.h"
#include "camera.h"
#include "input.h"
#include "hud.h"
#include "frames.h"


char *resolution = "1280x800:32@60";

void init (void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable (GL_CULL_FACE);
	glCullFace (GL_BACK);
	glEnable (GL_DEPTH_TEST);
	glEnable (GL_LIGHTING);
	glEnable (GL_COLOR_MATERIAL);
	glEnable (GL_TEXTURE_2D);
}

void reshape (int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (60.0, (GLfloat)w/(GLfloat)h, 1, 500);
    glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	frameStart();

	input_update();
	
	glPushMatrix();
		cam->update();
		scene->update();
		scene->display();
	glPopMatrix();

	draw_hud(); 
	
	frameEnd(GLUT_BITMAP_HELVETICA_10, 1.0, 1.0, 1.0, 0.90, 0.95);

	glutSwapBuffers();
}

int main (int argc, char **argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutGameModeString (resolution); // fullscreen
	glutEnterGameMode();
	
	init();
	glutReshapeFunc (reshape);
	glutDisplayFunc (display);
	glutPassiveMotionFunc (mouse_motion);
	glutMotionFunc (mouse_motion);
	glutMouseFunc (mouse_func);
	glutKeyboardFunc (key_down);
	glutKeyboardUpFunc (key_up);
	glutSpecialFunc (spec_key_down);
	glutSpecialUpFunc (spec_key_up);
	glutIdleFunc (display);
	
	glutSetCursor(GLUT_CURSOR_NONE);
	
	loadScene("prova");
	init_camera();
	atexit(clean);
	atexit(clean_camera);
	
	glutMainLoop();
	
    return 0;
}

