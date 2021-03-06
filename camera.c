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

#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "util.h"
#include "camera.h"
#include "scene.h"

#define ROT_Y_RAD cam->character->rot_y*DEG_TO_RAD
#define ROT_X_RAD cam->character->rot_x*DEG_TO_RAD

// externalized instance
camera_t *cam;


void move_forward (void)
{
	cam->mov_x += sin(ROT_Y_RAD)*cos(ROT_X_RAD);
	//~ cam->mov_y += sin(ROT_X_RAD);
	cam->mov_z += cos(ROT_Y_RAD)*cos(ROT_X_RAD);
}

void move_backward (void)
{
	cam->mov_x -= sin(ROT_Y_RAD)*cos(ROT_X_RAD);
	//~ cam->mov_y -= sin(ROT_X_RAD);
	cam->mov_z -= cos(ROT_Y_RAD)*cos(ROT_X_RAD);
}

void strafe_left (void)
{
	cam->mov_x += cos(ROT_Y_RAD);
	cam->mov_z -= sin(ROT_Y_RAD);
}

void strafe_right (void)
{
	cam->mov_x -= cos(ROT_Y_RAD);
	cam->mov_z += sin(ROT_Y_RAD);
}

void rotate_left (void)
{
	cam->character->rot_y += 0.3;
}

void rotate_right (void)
{
	cam->character->rot_y -= 0.3;
}

void update_bounds (void)
{
	cam->character->min_x = cam->character->pos_x - 1;
	cam->character->max_x = cam->character->pos_x + 1;
	cam->character->min_y = 0;
	cam->character->max_y = 4;
	cam->character->min_z = cam->character->pos_z - 1;
	cam->character->max_z = cam->character->pos_z + 1;
}

void reset_moves (void)
{
	cam->mov_x = 0;
	cam->mov_y = 0;
	cam->mov_z = 0;
}

void check_limits (void)
{
	if (cam->character->pos_y < cam->min_y)
		cam->character->pos_y = cam->min_y;
	if (cam->character->pos_y > cam->max_y)
		cam->character->pos_y = cam->max_y;
}

void update (void)
{
	update_bounds();

	reset_moves();

	check_limits();

	// make movements by opposite traslate and rotate the scene...
	glRotatef (-cam->character->rot_x, 1.0, 0.0, 0.0);
	glRotatef (-cam->character->rot_y, 0.0, 1.0, 0.0);
	glRotatef (180.0, 0.0, 1.0, 0.0); 
	glTranslatef (-cam->character->pos_x, -cam->character->pos_y, -cam->character->pos_z);
}

void init_camera (void)
{
	cam = malloc (sizeof (camera_t));

	cam->move_forward = move_forward;
	cam->move_backward = move_backward;
	cam->strafe_left = strafe_left;
	cam->strafe_right = strafe_right;
	cam->rotate_left = rotate_left;
	cam->rotate_right = rotate_right;
	cam->update = update;
	
	cam->min_y = 3;
	cam->max_y = 3;
	
	reset_moves();
}

void clean_camera (void)
{
	free (cam);
}

