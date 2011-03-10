
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

#include "object.h"
#include "texture.h"
#include "camera.h"

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

/****** Draw functions *******/

void drawWall (object_t *this) {
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

void drawTop (object_t *this) {
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_QUADS);
		glVertex3f(0, this->max_y, 0);
		glVertex3f(this->max_x, this->max_y, 0);
		glVertex3f(this->max_x, this->max_y, this->max_z);
		glVertex3f(0, this->max_y, this->max_z);
	glEnd();
}

void drawFloor (object_t *this) {
	glColor3f(1,1,1);
	loadTexture(TEXTURE_METAL2);
	int t = this->max_z;
	int s = this->max_x;
	float x = this->max_x;
	float z = this->max_z;
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex3f(0, 0, 0);
		glTexCoord2i(0, t);
		glVertex3f(0, 0, z);
		glTexCoord2i(s, t);
		glVertex3f(x, 0, z);
		glTexCoord2i(s, 0);
		glVertex3f(x, 0, 0);
	glEnd();
}

void drawBullet (object_t *this)
{
	float rot_x_rad = this->rot_x / 180*3.141592654f;
	float rot_y_rad = this->rot_y / 180*3.141592654f;
	
	this->pos_x += sin(rot_y_rad)*this->vel;
	this->pos_z += cos(rot_y_rad)*this->vel;
	this->pos_y += sin(rot_x_rad)*this->vel;

	glPushMatrix();
		glDisable (GL_TEXTURE_2D);
		glColor3f (1, 1, 0);
		glTranslatef (this->pos_x, this->pos_y, this->pos_z);
		glutSolidSphere (0.01, 8, 8);
		glEnable (GL_TEXTURE_2D);
	glPopMatrix();
}


void drawTurret (object_t *this)
{
	glPushMatrix();
	glTranslatef(this->pos_x, this->pos_y+CELLSIZE/2, this->pos_z);
	object_t *character = cam->character;
	float vector[] = { character->pos_x-this->pos_x, 0, character->pos_z-this->pos_z };
	float radius = sqrt( vector[0]*vector[0] + vector[2]*vector[2] );
	float angle = acos( vector[0]/radius );
	this->rot_y = angle*180/M_PI - 90;
	if ( vector[2] < 0 )
		this->rot_y = 180-this->rot_y;
	glRotatef( -this->rot_y, 0, 1, 0 );
	glColor3f(1,0,0);
	glutSolidCone(CELLSIZE/4, CELLSIZE/2, 30, 30);
	glPopMatrix();
}

/****** collision functions *******/

void turretCollision (object_t *this)
{
	this->energy -= 10;
}

/****** Create functions *******/

object_t *newWall (float min_x, float max_x, float min_z, float max_z) {
	object_t *wall = malloc(sizeof(object_t));
	wall->min_x = min_x;
	wall->max_x = max_x;
	wall->min_y = 0;
	wall->max_y = WALL_HEIGHT;
	wall->min_z = min_z;
	wall->max_z = max_z;
	wall->type = TYPE_WALL;
	wall->display = drawWall;
	wall->energy = 1000;
	wall->onCollision = doNothing;
	return wall;
}

object_t *newBullet (float pos_x, float pos_y, float pos_z, float rot_x, float rot_y)
{
	object_t *bullet = malloc (sizeof(object_t));
	bullet->pos_x = pos_x;
	bullet->pos_y = pos_y;
	bullet->pos_z = pos_z;
	bullet->rot_x = rot_x;
	bullet->rot_y = rot_y;
	bullet->vel = 0.5;
	bullet->type = TYPE_BULLET;
	bullet->energy = 1;
	
	bullet->display = drawBullet;
	bullet->onCollision = doNothing;
	return bullet;
}

object_t *newFloor (float max_x, float max_y, float max_z)
{
	object_t *floor = malloc (sizeof(object_t));
	floor->max_x = max_x;
	floor->max_y = 0;
	floor->max_z = max_z;
	floor->min_x = 0;
	floor->min_y = 0;
	floor->min_z = 0;
	floor->type = TYPE_FLOOR;
	floor->energy = 1;
	
	floor->display = drawFloor;
	floor->onCollision = doNothing;
	return floor;
}

object_t *newTop (float max_x, float max_y, float max_z)
{
	object_t *top = malloc (sizeof(object_t));
	top->max_x = max_x;
	top->max_y = max_y;
	top->max_z = max_z;
	top->min_x = 0;
	top->min_y = max_y;
	top->min_z = 0;
	top->type = TYPE_TOP;
	top->energy = 1;
	
	top->display = drawTop;
	top->onCollision = doNothing;
	return top;
}

object_t *newTurret (float min_x, float max_x, float min_z, float max_z) {
	object_t *turret = malloc(sizeof(object_t));
	turret->min_x = min_x;
	turret->max_x = max_x;
	turret->min_y = 0;
	turret->max_y = WALL_HEIGHT/2;
	turret->min_z = min_z;
	turret->max_z = max_z;
	turret->type = TYPE_TURRET;
	turret->display = drawTurret;
	turret->energy = 100;
	turret->pos_x = min_x+CELLSIZE/2;
	turret->pos_y = 0;
	turret->pos_z = min_z+CELLSIZE/2;
	turret->onCollision = turretCollision;
	return turret;
}

void doNothing() {}



