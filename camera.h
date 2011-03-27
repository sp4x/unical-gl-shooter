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

#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"

typedef struct camera_t
{
	// the player object
	object_t *character;
	// movements limits
	int min_y, max_y;
	// movement's components
	float mov_x, mov_y, mov_z;

	// move functions
	void (*move_forward) (void);
	void (*move_backward) (void);
	void (*strafe_left) (void);
	void (*strafe_right) (void);
	void (*rotate_left) (void);
	void (*rotate_right) (void);
	// update function
	void (*update) (void);
	
} camera_t;

extern camera_t *cam;

void init_camera (void);
void clean_camera (void);

#endif
