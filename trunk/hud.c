#include "hud.h"
#include "util.h"

/* Enters othographics projection mode to draw text or images to the
 * front of the screen */
void enter_ortho_mode()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, width, height, 0.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable (GL_LIGHTING);
}

/* Gets back to perspective projection mode after entering orthographic mode */
void exit_ortho_mode()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnable (GL_LIGHTING);
}

/* Writes red text on screen in the specified position */
void draw_text2d (int x, int y, char *string)
{
	int len = 0, i = 0;
	glRasterPos2i(x, y);
	len = (int)strlen(string);
	for(i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
}

/* Draws the heads up display on screen */
void draw_hud()
{
	enter_ortho_mode();

	glColor3f (1, 1, 1);
	glLineWidth (2);
	glBegin (GL_LINES);
		glVertex2f (width/2-10, height/2);
		glVertex2f (width/2+10, height/2);

		glVertex2f (width/2, height/2-10);
		glVertex2f (width/2, height/2+10);
	glEnd();
	char s[50];
	sprintf (s, "Energy: %d", cam->character->energy);
	draw_text2d(20, 30, s);
	sprintf (s, "Score: %d", cam->character->score);
	draw_text2d(20, 50, s);
	
	if (gameover)
	{
		draw_text2d(width/2-60, height/2, "GAME OVER!");
	}
	
	exit_ortho_mode();	
}