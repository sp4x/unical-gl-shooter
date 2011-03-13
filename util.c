
#include "util.h"

struct timeval timer;

void game_over (void)
{
	gameover = 1;
	struct timeval tim;
	gettimeofday (&tim, NULL);
	double curr = tim.tv_sec + (tim.tv_usec / 1.0E6);
	double prev = curr;
	while ((curr - prev) < 3)
	{	
		gettimeofday (&tim, NULL);
		curr = tim.tv_sec + (tim.tv_usec / 1.0E6);
		enter_ortho_mode();
		int w = glutGameModeGet (GLUT_GAME_MODE_WIDTH);
		int h = glutGameModeGet (GLUT_GAME_MODE_HEIGHT);
		draw_text2d (w/2-5, h/2, "GAME OVER!");
		exit_ortho_mode();
		glutSwapBuffers();
	}
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
