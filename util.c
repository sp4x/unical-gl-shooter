
#include "util.h"

/* GLOBAL TIMER */
struct timeval timer;

void game_over (void)
{
	gameover = 1;
	struct timeval tim;
	gettimeofday (&tim, NULL);
	double curr = tim.tv_sec + (tim.tv_usec / 1.0E6);
	double prev = curr;
	enter_ortho_mode();
	int w = glutGameModeGet (GLUT_GAME_MODE_WIDTH);
	int h = glutGameModeGet (GLUT_GAME_MODE_HEIGHT);
	draw_text2d (w/2-60, h/2+5, "GAME OVER!", GLUT_BITMAP_HELVETICA_18);
	glutSwapBuffers();
	while ((curr - prev) < 3)
	{	
		gettimeofday (&tim, NULL);
		curr = tim.tv_sec + (tim.tv_usec / 1.0E6);
	}
	glutLeaveGameMode();
	exit(0);
}

void level_complete (void)
{
	gameover = 1;
	struct timeval tim;
	gettimeofday (&tim, NULL);
	double curr = tim.tv_sec + (tim.tv_usec / 1.0E6);
	double prev = curr;
	enter_ortho_mode();
	int w = glutGameModeGet (GLUT_GAME_MODE_WIDTH);
	int h = glutGameModeGet (GLUT_GAME_MODE_HEIGHT);
	draw_text2d (w/2-60, h/2+5, "Level complete!", GLUT_BITMAP_HELVETICA_18);
	glutSwapBuffers();
	glutPostRedisplay();
	while ((curr - prev) < 3)
	{	
		gettimeofday (&tim, NULL);
		curr = tim.tv_sec + (tim.tv_usec / 1.0E6);
	}
	glutLeaveGameMode();
	exit(0);
}

void update_timer (void) 
{
	gettimeofday(&timer, NULL);
}

double get_time (void)
{
	return timer.tv_sec + (timer.tv_usec / 1.0E6);
}

char *objtype (object_t *o)
{
	char *s = "oo";
	switch (o->type)
	{
		case TYPE_BULLET: s = "bullet"; break;
		case TYPE_CHARACTER: s = "character"; break;
		case TYPE_CUBE: s = "cube"; break;
		case TYPE_FLOOR: s = "floor"; break;
		case TYPE_TOP: s = "top"; break;
		case TYPE_TURRET: s = "turret"; break;
		case TYPE_WALL: s = "wall"; break;
		default: s = "nothing?"; break;
	}
	return s;
}

float distance (float *point, object_t *obj)
{
	enum {X, Y, Z};
	float dx = obj->pos_x - point[X];
	float dz = obj->pos_z - point[Z];
	return sqrtf (dx*dx + dz*dz);
}
