
#include <stdio.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#include "object.h"
#include "objectlist.h"
#include "texture.h"
#include "camera.h"
#include "util.h"

#define COLLISION_GAP 0.1
#define WALL_GAP (CELLSIZE-1)/2

void doNothing (object_t *this, object_t *obj) {}

/* check if this has a collision with obj 
 */
int hasCollision (object_t *this, object_t *obj) 
{
	if (obj->type == this->owner_type || this->type == obj->owner_type ||
		obj->type == this->type )
		return 0;
		
	float x = this->pos_x;
	float y = this->pos_y;
	float z = this->pos_z;
	
	if (x > obj->min_x && x < obj->max_x && 
		y > obj->min_y && y < obj->max_y && 
		z > obj->min_z && z < obj->max_z) 
	{ 
		if (this->type != TYPE_CHARACTER)
			return 1;
			
		float coords[] = { x, y, z };
		float sides[] = { obj->min_x, obj->max_x, obj->min_y, obj->max_y,  obj->min_z, obj->max_z };
		float *available_coords[] = { &this->pos_x, &this->pos_y, &this->pos_z };
		int coord_index = 0, side_index = 0;
		
		for (coord_index = 0; coord_index < 3; coord_index++) 
		{
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
	glNormal3f(0,0,-1);
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
	glNormal3f(0,0,1);
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
	glNormal3f(1,0,0);
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
	glNormal3f(-1,0,0);
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
	glColor3f(1,1,1);
	loadTexture(TEXTURE_METAL2);
	int t = this->max_z;
	int s = this->max_x;
	float x = this->max_x;
	float z = this->max_z;
	glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glTexCoord2i(0, 0);
		glVertex3f(0, this->min_y, 0);
		glTexCoord2i(0, t);
		glVertex3f(x, this->min_y, 0);
		glTexCoord2i(s, t);
		glVertex3f(x, this->min_y, z);
		glTexCoord2i(s, 0);
		glVertex3f(0, this->min_y, z);
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
		glNormal3f(0,1,0);
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
	double rot_x_rad = this->rot_x / 180*M_PI;
	double rot_y_rad = this->rot_y / 180*M_PI;
	
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
	object_t *character = cam->character;
	float vector[] = { character->pos_x-this->pos_x, 0, character->pos_z-this->pos_z };
	float radius = sqrt( vector[0]*vector[0] + vector[2]*vector[2] );
	float angle = acos( vector[0]/radius );
	this->rot_y = ( vector[2]>=0 ? 0 : 180);
	this->rot_y -= (angle*180/M_PI - 90);
	this->rot_y *= ( vector[2]>=0 ? 1 : -1);
	
	this->curr_time = get_time();
	if (this->curr_time - this->last_time > 0.3)
	{
		this->last_time = this->curr_time;
		object_t *bullet = newBullet (this);
		object_list->append (bullet);
	}
	
	GLUquadricObj *quadric = gluNewQuadric();
	glPushMatrix();
		glTranslatef(this->pos_x, 0, this->pos_z);
		glPushMatrix();
			glRotatef(-90, 1, 0, 0);
			gluCylinder(quadric, 0.5, 0.5, 2.5, 20, 20);
		glPopMatrix();
		glTranslatef(0, 2.5, 0);
		gluSphere(quadric, 0.6, 20, 20);
		glRotatef( this->rot_y, 0, 1, 0);
		glDisable(GL_CULL_FACE);
		gluCylinder(quadric, 0.1, 0.1, CELLSIZE/2, 20, 20);
		glEnable(GL_CULL_FACE);
	glPopMatrix();
	gluDeleteQuadric(quadric);
}

/****** onCollision functions *******/

void turretCollision (object_t *this, object_t *obj)
{
	if (obj->type == TYPE_BULLET)
		this->energy -= 3;
}

void bulletCollision (object_t *this, object_t *obj)
{
	this->energy = 0;
}

void characterCollision (object_t *this, object_t *obj)
{
	if (obj->type == TYPE_BULLET)
		this->energy -= 1;
}

/****** Create functions *******/

void displayNothing (object_t *this) {}

object_t *newCharacter (int pos_x, int pos_y, int pos_z)
{
	object_t *this = malloc (sizeof(object_t));
	this->type = TYPE_CHARACTER;
	this->rot_x = 0;
	this->rot_y = 0;
	this->pos_x = pos_x;
	this->pos_y = pos_y;
	this->pos_z = pos_z;
	this->min_x = pos_x - 0.5;
	this->max_x = pos_x + 0.5;
	this->min_y = 0;
	this->max_y = 4;
	this->min_z = pos_z - 0.5;
	this->max_z = pos_z + 0.5;
	this->vel = 0.1;
	this->energy = 100;
	this->score = 0;
	
	// init timer
	this->curr_time = get_time();
	this->last_time = get_time();
	
	this->display = displayNothing;
	this->onCollision = characterCollision;
	return this;
}

object_t *newWall (float min_x, float max_x, float min_z, float max_z) 
{
	object_t *this = malloc(sizeof(object_t));
	this->min_x = min_x;
	this->max_x = max_x;
	this->min_y = 0;
	this->max_y = WALL_HEIGHT;
	this->min_z = min_z;
	this->max_z = max_z;
	this->type = TYPE_WALL;
	this->display = drawWall;
	this->energy = 1000;
	this->onCollision = doNothing;
	return this;
}

object_t *newBullet (struct object_t *owner)
{
	object_t *this = malloc (sizeof(object_t));
	
	this->owner_type = owner->type;
	this->pos_x = owner->pos_x;
	this->pos_y = owner->pos_y;
	this->pos_z = owner->pos_z;
	this->rot_x = owner->rot_x;
	this->rot_y = owner->rot_y;
	this->vel = 0.02;
	this->type = TYPE_BULLET;
	this->energy = 1;
	
	this->display = drawBullet;
	this->onCollision = bulletCollision;
	return this;
}

object_t *newFloor (float max_x, float max_y, float max_z)
{
	object_t *this = malloc (sizeof(object_t));
	this->max_x = max_x;
	this->max_y = 0;
	this->max_z = max_z;
	this->min_x = 0;
	this->min_y = -3;
	this->min_z = 0;
	this->type = TYPE_FLOOR;
	this->energy = 1;
	
	this->display = drawFloor;
	this->onCollision = doNothing;
	return this;
}

object_t *newTop (float max_x, float max_y, float max_z)
{
	object_t *this = malloc (sizeof(object_t));
	this->max_x = max_x;
	this->max_y = max_y+3;
	this->max_z = max_z;
	this->min_x = 0;
	this->min_y = max_y;
	this->min_z = 0;
	this->type = TYPE_TOP;
	this->energy = 1;
	
	this->display = drawTop;
	this->onCollision = doNothing;
	return this;
}

object_t *newTurret (float min_x, float max_x, float min_z, float max_z) {
	object_t *this = malloc(sizeof(object_t));
	double half_cellsize = (double)CELLSIZE/2.0;
	this->pos_x = min_x+half_cellsize;
	this->pos_y = 2.5;
	this->pos_z = min_z+half_cellsize;
	
	this->min_x = this->pos_x - 1;
	this->max_x = this->pos_x + 1;
	this->min_y = 0;
	this->max_y = this->pos_y + 1;
	this->min_z = this->pos_z - 1;
	this->max_z = this->pos_z + 1;
	this->type = TYPE_TURRET;
	this->energy = 100;
	
	// init timer
	this->curr_time = get_time();
	this->last_time = get_time();
	
	this->display = drawTurret;
	this->onCollision = turretCollision;
	return this;
}

