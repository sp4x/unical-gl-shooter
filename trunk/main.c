#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "scene.h"
#include "camera.h"
#include "input.h"
#include "hud.h"
#include "frames.h"
#include "util.h"

double curr, last;

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
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	update_timer();
	curr = last = get_time();
}

void reshape (int w, int h)
{
    glViewport (0, 0, w, h);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    gluPerspective (60.0, (GLfloat)w/(GLfloat)h, 0.1, 500);
    glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}

void display()
{
	frameStart();
	update_timer();
	glPushMatrix();
	curr = get_time();
	/* update input each 16 ms (~60 time for second) */
	if ((curr - last) >= 0.0167)
    {
		last = curr;
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		input_update();
		scene->update();
		cam->update();
		scene->display();

		draw_hud(); 
		frameEnd(GLUT_BITMAP_HELVETICA_10, 1.0, 1.0, 1.0, 0.90, 0.95);
		
		glutSwapBuffers();
	}

	glPopMatrix();
}

int main (int argc, char **argv)
{
	glutInit (&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	
	showbounds = GL_FALSE;
	
	width = glutGet(GLUT_SCREEN_WIDTH); 
	height = glutGet(GLUT_SCREEN_HEIGHT); 
	
	printf ("screen resolution: %dx%d\n", width, height);
	
	sprintf (resolution, "%dx%d:32@60", width, height);
	printf ("game resolution: %s\n", resolution);
	gameover = 0;
	
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

