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

#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "object.h"

#define RAD_TO_DEG 180.0/M_PI
#define DEG_TO_RAD M_PI/180.0

/* true when player dies */
GLboolean gameover;

/* current screen resolution */
char resolution[30];
int width, height;

/* true if you want to show objects bounds */
GLboolean showbounds;

/* global timer */
extern struct timeval timer;

/* game over... */
void game_over (void);
void level_complete (void);

/* update the global timer */
void update_timer (void);

/* returns the current time (in seconds) */
double get_time (void);

/* return a string identifying the type of the object o */
char *objtype (object_t *o);

/* distance of a point from an object */
float distance (float *point, object_t *obj);

/* return the positions of the 'n' min elems of 'v' */
int *min_pos (int *v, int vsize, int n);
