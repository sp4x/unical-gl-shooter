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

typedef struct
{
	float position[3];				/* particle's position */
    float speed[3];					/* speed on each axes */
    float color[3];					/* particle's color (RGB) */
	
} particle_t;

typedef struct
{
	float   position[3];			/* debris position */
    float   speed[3];				/* speed on each axes */
    float   orientation[3];        	/* Rotation angles around x, y, and z axes */
    float   orientationSpeed[3];	/* rotation angles around itself */
    float   color[3];				/* debris color (RGB) */
    float   scale[3];				/* scale factor on each component */

} debris_t;

typedef struct explosion_t
{
	int nparticles, ndebris;		/* number of particles and debris */
	particle_t *particles; 			/* particles array */
	debris_t *debris;				/* debris array */
	double lifetime;				/* explosion life time */
	double speed;					/* explosion speed */

	/* update particles and debris */
	void (*update) (struct explosion_t *this);
	/* draw the explosion */			
	void (*display) (struct explosion_t *this);			
	
} explosion_t;

/** create a new explosion with p particles and d debris and a lifetime
 */
explosion_t *new_explosion (int p, int d, double lifetime, float scale, float *color, double speed);

void delete_explosion (explosion_t *exp);
