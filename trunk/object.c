
#include <stdio.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <math.h>

#include "object.h"
#include "texture.h"

#define COLLISION_GAP 0.1
#define WALL_GAP (CELLSIZE-1)/2


int hasCollision(object_t *obj, float x, float y, float z, float *available_x, float *available_y, float *available_z) {
	if (x > obj->min_x && x < obj->max_x 
		&& y > obj->min_y && y < obj->max_y
		&& z > obj->min_z && z < obj->max_z) { //(x,y,z) in object bounds
		
		
		if (available_x == NULL || available_y == NULL || available_z == NULL)
			return 1;
			
		float coords[] = { x, y, z };
		float sides[] = { obj->min_x, obj->max_x, obj->min_y, obj->max_y,  obj->min_z, obj->max_z };
		float *available_coords[] = { available_x, available_y, available_z };
		int coord_index = 0, side_index = 0;
		for (coord_index = 0; coord_index < 3; coord_index++) {
			float min_side_gap = coords[coord_index] - sides[side_index]; //c - min_side
			float max_side_gap = sides[side_index+1] - coords[coord_index]; //max_side -c
			
			if ( min_side_gap > 0 && min_side_gap <= COLLISION_GAP) {
				*( available_coords[coord_index] ) = sides[side_index];
			}
			else if ( max_side_gap > 0 && max_side_gap <= COLLISION_GAP) {
				*( available_coords[coord_index] ) = sides[side_index+1];
			}
			
			side_index += 2;
		}
		return 1;
	}
	return 0;
}

void drawWall(object_t *this) {
	float min_x = this->min_x + WALL_GAP;
	float max_x = this->max_x - WALL_GAP;
	float min_z = this->min_z + WALL_GAP;
	float max_z = this->max_z - WALL_GAP;
	
	loadTexture(TEXTURE_BRICK);
	   
	float sx = max_x/10, sz = max_z/10, t = WALL_HEIGHT/2;
	glBegin(GL_QUADS);
	//north wall
	glColor3f(1,1,1);
	glTexCoord2d( sx, 0);
	glVertex3f(max_x, 0, min_z);
	glTexCoord2d( 0, 0);
	glVertex3f(min_x, 0, min_z);
	glTexCoord2d( 0, t);
	glVertex3f(min_x, WALL_HEIGHT, min_z);
	glTexCoord2d( sx, t);
	glVertex3f(max_x, WALL_HEIGHT, min_z);
	//south wall
	//~ glColor3f(0,0,1);
	glTexCoord2d( 0, 0);
	glVertex3f(min_x, 0, max_z);
	glTexCoord2d( sx, 0);
	glVertex3f(max_x, 0, max_z);
	glTexCoord2d( sx, t);
	glVertex3f(max_x, WALL_HEIGHT, max_z);
	glTexCoord2d( 0, t);
	glVertex3f(min_x, WALL_HEIGHT, max_z);
	
	//east wall
	//~ glColor3f(0,1,0);
	glTexCoord2d( sz, 0);
	glVertex3f(max_x, 0, max_z);
	glTexCoord2d( 0, 0);
	glVertex3f(max_x, 0, min_z);
	glTexCoord2d( 0, t);
	glVertex3f(max_x, WALL_HEIGHT, min_z);
	glTexCoord2d( sz, t);
	glVertex3f(max_x, WALL_HEIGHT, max_z);
	//west wall
	//~ glColor3f(1,1,0);
	glTexCoord2d( 0, 0);
	glVertex3f(min_x, 0, min_z);
	glTexCoord2d( sz, 0);
	glVertex3f(min_x, 0, max_z);
	glTexCoord2d( sz, t);
	glVertex3f(min_x, WALL_HEIGHT, max_z);
	glTexCoord2d( 0, t);
	glVertex3f(min_x, WALL_HEIGHT, min_z);
	glEnd();
}

object_t *newWall(float min_x, float max_x, float min_z, float max_z) {
	object_t *wall = malloc(sizeof(object_t));
	wall->min_x = min_x;
	wall->max_x = max_x;
	wall->min_y = 0;
	wall->max_y = WALL_HEIGHT;
	wall->min_z = min_z;
	wall->max_z = max_z;
	wall->type = TYPE_WALL;
	wall->display = drawWall;
	return wall;
}

void drawBullet (object_t *obj)
{
	float rot_x_rad = obj->rot_x / 180*3.141592654f;
	float rot_y_rad = obj->rot_y / 180*3.141592654f;
	
	obj->pos_x += sin(rot_y_rad)*obj->vel;
	obj->pos_z += cos(rot_y_rad)*obj->vel;
	obj->pos_y += sin(rot_x_rad)*obj->vel;

	glPushMatrix();
	glDisable (GL_TEXTURE_2D);
	glColor3f (1, 1, 0);
	glTranslatef (obj->pos_x, obj->pos_y, obj->pos_z);
	glutSolidSphere (0.01, 8, 8);
	glEnable (GL_TEXTURE_2D);
	glPopMatrix();
}



