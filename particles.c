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


#include "particles.h"

void delete_explosion (explosion_t *exp)
{
	free (exp->particles);
	free (exp->debris);
	free (exp);
	exp = NULL;
}

void update_explosion (explosion_t *this)
{
	if (this->lifetime > 0)
	{
		int i;
		for (i = 0; i < this->nparticles; i++)
		{
			this->particles[i].position[0] += this->particles[i].speed[0] * this->speed;
			this->particles[i].position[1] += this->particles[i].speed[1] * this->speed;
			this->particles[i].position[2] += this->particles[i].speed[2] * this->speed;

			/* smoothly modify color during update... */
			//~ this->particles[i].color[0] -= 1.0 / 500.0;
			//~ if (this->particles[i].color[0] < 0.0)
			//~ {
				//~ this->particles[i].color[0] = 0.0;
			//~ }
//~ 
			//~ this->particles[i].color[1] -= 1.0 / 100.0;
			//~ if (this->particles[i].color[1] < 0.0)
			//~ {
				//~ this->particles[i].color[1] = 0.0;
			//~ }
//~ 
			//~ this->particles[i].color[2] -= 1.0 / 50.0;
			//~ if (this->particles[i].color[2] < 0.0)
			//~ {
				//~ this->particles[i].color[2] = 0.0;
			//~ }
		}

		for (i = 0; i < this->ndebris; i++)
		{
			this->debris[i].position[0] += this->debris[i].speed[0] * this->speed;
			this->debris[i].position[1] += this->debris[i].speed[1] * this->speed;
			this->debris[i].position[2] += this->debris[i].speed[2] * this->speed;

			this->debris[i].orientation[0] += this->debris[i].orientationSpeed[0] * 10;
			this->debris[i].orientation[1] += this->debris[i].orientationSpeed[1] * 10;
			this->debris[i].orientation[2] += this->debris[i].orientationSpeed[2] * 10;
		}
		this->lifetime -= 1;
	}
}

void draw_explosion (explosion_t *this)
{
	glPushAttrib (GL_ENABLE_BIT);
	glDisable (GL_LIGHTING);
	glDisable (GL_TEXTURE_2D);
	glDisable (GL_COLOR_MATERIAL);
	glDisable (GL_CULL_FACE);
	glEnable (GL_DEPTH_TEST);

	if (this->lifetime > 0)
    {
        glPushMatrix ();
			glBegin (GL_POINTS);
			int i;
			for (i = 0; i < this->nparticles; i++)
			{
				glColor3fv (this->particles[i].color);
				glVertex3fv (this->particles[i].position);
			}
			glEnd ();
        glPopMatrix ();

        glNormal3f (0.0, 0.0, 1.0);
        for (i = 0; i < this->ndebris; i++)
        {
            glColor3fv (this->debris[i].color);
            glPushMatrix ();
				glTranslatef (this->debris[i].position[0], this->debris[i].position[1], this->debris[i].position[2]);
				glRotatef (this->debris[i].orientation[0], 1.0, 0.0, 0.0);
				glRotatef (this->debris[i].orientation[1], 0.0, 1.0, 0.0);
				glRotatef (this->debris[i].orientation[2], 0.0, 0.0, 1.0);

				glScalef (this->debris[i].scale[0], this->debris[i].scale[1], this->debris[i].scale[2]);

				glBegin (GL_TRIANGLES);
					glVertex3f (0.0, 0.5, 0.0);
					glVertex3f (-0.25, 0.0, 0.0);
					glVertex3f (0.25, 0.0, 0.0);
				glEnd ();
            glPopMatrix ();
        }
    }
	glPopAttrib();
}

void random_speed (float dest[3])
{
    float x, y, z, len;

    x = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
    y = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;
    z = (2.0 * ((GLfloat) rand ()) / ((GLfloat) RAND_MAX)) - 1.0;

	// normalize speeds
	len = sqrt (x * x + y * y + z * z);
	if (len)
	{
		x = x / len;
		y = y / len;
		z = z / len;
	}

    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
}

/** create a new explosion with p particles and d debris and a lifetime
 */
explosion_t *new_explosion (int p, int d, double lifetime, float scale, float *color, double speed)
{
	explosion_t *this = malloc (sizeof (explosion_t));

	this->lifetime = lifetime;
	this->nparticles = p;
	this->ndebris = d;
	this->speed = speed;
	
	this->update = update_explosion;
	this->display = draw_explosion;

	this->particles = malloc (p * sizeof (particle_t));
	this->debris = malloc (d * sizeof (debris_t));

	int i;
    for (i = 0; i < p; i++)
    {
        this->particles[i].position[0] = 0;
        this->particles[i].position[1] = 0;
        this->particles[i].position[2] = 0;
		
        this->particles[i].color[0] = color[0];
		this->particles[i].color[1] = color[1];
        this->particles[i].color[2] = color[2];

        random_speed (this->particles[i].speed);
    }

    for (i = 0; i < d; i++)
    {
        this->debris[i].position[0] = 0;
        this->debris[i].position[1] = 0;
        this->debris[i].position[2] = 0;
        
        this->debris[i].orientation[0] = 0.0;
        this->debris[i].orientation[1] = 0.0;
        this->debris[i].orientation[2] = 0.0;

        this->debris[i].color[0] = color[0];
        this->debris[i].color[1] = color[1];
        this->debris[i].color[2] = color[2];

        this->debris[i].scale[0] = scale;
        this->debris[i].scale[1] = scale;
        this->debris[i].scale[2] = scale;

        random_speed (this->debris[i].speed);
        random_speed (this->debris[i].orientationSpeed);
    }
	return this;
}
