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


#ifndef SCENE_H
#define SCENE_H

#include "object.h"

#define FREE_SPACE '0'
#define WALL '1'
#define TURRET 't'
#define CUBE 'c'
#define WINDOW 'w'
#define LIGHT 'l'

#define CELLSIZE 2

/** struct that represents the scene 
 */
typedef struct scene_t {
	/** draw the scene */
	void (*display) (void);
	/** update the scene */
	void (*update) (void);
	/** add an object to the scene */
	void (*add) (object_t *obj);
	/** remove an object from the scene */
	void (*remove) (object_t *obj);
	
} scene_t;

/** externalized instance of scene */
extern scene_t *scene;

/** load the scene from file */
void loadScene (char *file);
/** clean the scene and free memory */
void cleanScene (void);

#endif
