/**
 * Copyright (C) 2011 Vincenzo Pirrone, Salvatore Loria
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


#include "util.h"
#include <limits.h>

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

/* return true if 'v' contains 'val' */
int search (int val, int *v, int size)
{
	int i;
	for (i = 0; i < size; i++)
		if (v[i] == val)
			return 1;
	return 0;
}

/* return the positions of the 'n' min elems of 'v' */
int *min_pos (int *v, int vsize, int n)
{
	int *pos = malloc (n*sizeof(int));
	memset (pos, -1, n*sizeof(int));
	int i, j, min;
	for (i = 0; i < n; i++)
	{
		min = INT_MAX;
		for (j = 0; j < vsize; j++)
		{
			if (!search(j, pos, n) && v[j] < min) {
				pos[i] = j;
				min = v[j];
			}
		}
	}
	return pos;
}
