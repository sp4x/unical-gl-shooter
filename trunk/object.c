
#include <stdio.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#include "object.h"
#include "objectlist.h"
#include "texture.h"
#include "camera.h"
#include "util.h"
#include "scene.h"

#define COLLISION_GAP 0.1
//~ #define WALL_GAP (CELLSIZE-1)/2
#define LEN_GAP 0//CELLSIZE*0.7
#define DEPTH_GAP 0// CELLSIZE*0.5

void doNothing (object_t *this, object_t *obj) {}
void notUpdate (object_t *this) {}

int inGap(float c, object_t *obj, int coord) {
	if ( coord == 0)
		return c > obj->min_x && c < obj->max_x;
	return c > obj->min_z && c < obj->max_z;
}

/* check if this has a collision with obj 
 */
int hasCollision (object_t *this, object_t *obj) 
{
	if (this->type == TYPE_BULLET && obj->type == this->owner_type)
		return 0;

	if (obj->type == this->type )
		return 0;
		
	float x = this->pos_x;
	float y = this->pos_y;
	float z = this->pos_z;
	
	if ( this->type == TYPE_BULLET )
		return (x > obj->min_x && x < obj->max_x && 
			y > obj->min_y && y < obj->max_y && 
			z > obj->min_z && z < obj->max_z);
	
	if (obj->type == TYPE_FLOOR || obj->type == TYPE_TOP)
		return 0;
			
	enum {X, Y, Z};
	enum {LEFT, RIGHT, UP, DOWN};
	int xside = (x > obj->max_x ? RIGHT : LEFT);
	int zside = ( z > obj->max_z ? DOWN : UP);
	if (inGap(this->max_z,obj,Z) || inGap(this->min_z,obj,Z))
	{
		if (xside == LEFT && this->max_x + cam->mov_x >= obj->min_x)
			cam->mov_x = 0;
		if (xside == RIGHT && this->min_x + cam->mov_x <= obj->max_x)
			cam->mov_x = 0;
	}
	if (inGap(this->max_x,obj,X) || inGap(this->min_x,obj,X))
	{
		//~ printf("next %f, obj %f\n", this->max_z + cam->mov_z, obj->min_z);
		if (zside == UP && this->max_z + cam->mov_z >= obj->min_z)
			cam->mov_z = 0;
		if (zside == DOWN && this->min_z + cam->mov_z <= obj->max_z)
			cam->mov_z = 0;
	}
}

/****** Draw functions *******/

void drawWall (object_t *this) {
	float min_x = this->min_x;// + WALL_GAP;
	float max_x = this->max_x;// - WALL_GAP;
	float min_z = this->min_z;// + WALL_GAP;
	float max_z = this->max_z;// - WALL_GAP;
	
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
	GLUquadricObj *quadric = gluNewQuadric();
	glPushMatrix();
		glTranslatef (this->pos_x, 0, this->pos_z);
		glPushMatrix();
			glRotatef (-90, 1, 0, 0);
			gluCylinder (quadric, 0.5, 0.5, 2.5, 20, 20);
		glPopMatrix();
		glTranslatef (0, 2.5, 0);
		gluSphere (quadric, 0.6, 20, 20);
		glRotatef (this->rot_y, 0, 1, 0);
		glDisable (GL_CULL_FACE);
		gluCylinder (quadric, 0.1, 0.1, CELLSIZE/2, 20, 20);
		glEnable (GL_CULL_FACE);
	glPopMatrix();
	gluDeleteQuadric(quadric);
}

void drawCube (object_t *this) 
{
	glPushMatrix();
		glTranslatef(this->pos_x, this->pos_y, this->pos_z);
		glRotatef(this->rot_y, 0, 1, 0);
		glutSolidCube(2);
	glPopMatrix();
	this->rot_y+=0.5;
	if ( this->rot_y == 360 )
		this->rot_y = 0;
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

void cubeCollision (object_t *this, object_t *obj)
{
	if ( obj->type == TYPE_BULLET ) {
		cam->character->score++;
		this->energy = 0;
	}
}

/****** Update functions *******/

void characterUpdate(object_t *this) 
{
	this->pos_x += cam->mov_x*this->vel;
	this->pos_y += cam->mov_y*this->vel;
	this->pos_z += cam->mov_z*this->vel;
}

void bulletUpdate (object_t *this)
{
	double rot_x_rad = this->rot_x*DEG_TO_RAD;
	double rot_y_rad = this->rot_y*DEG_TO_RAD;
	
	this->pos_x += sin(rot_y_rad)*this->vel;
	this->pos_z += cos(rot_y_rad)*this->vel;
	this->pos_y += sin(rot_x_rad)*this->vel;
}

void turretUpdate (object_t *this)
{
	object_t *character = cam->character;
	float vector[] = { character->pos_x-this->pos_x, 0, character->pos_z-this->pos_z };
	float radius = sqrt( vector[0]*vector[0] + vector[2]*vector[2] );
	float angle = acos( vector[0]/radius );
	this->rot_y = (vector[2]>=0 ? 0 : 180);
	this->rot_y -= (angle*RAD_TO_DEG - 90);
	this->rot_y *= (vector[2]>=0 ? 1 : -1);
	
	this->curr_time = get_time();
	if (this->curr_time - this->last_time > 0.3)
	{
		this->last_time = this->curr_time;
		object_t *bullet = newBullet (this);
		object_list->append (bullet);
	}
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
	this->min_x = pos_x - 1;
	this->max_x = pos_x + 1;
	this->min_y = 0;
	this->max_y = 4;
	this->min_z = pos_z - 1;
	this->max_z = pos_z + 1;
	this->vel = 0.1;
	this->energy = 100;
	this->score = 0;
	
	// init timer
	this->curr_time = get_time();
	this->last_time = get_time();
	
	this->display = displayNothing;
	this->onCollision = characterCollision;
	this->update = characterUpdate;
	return this;
}

object_t *newWall (float start_x, float end_x, float start_z, float end_z) 
{
	object_t *this = malloc(sizeof(object_t));
	this->min_x = start_x;
	this->max_x = end_x;
	this->min_y = 0;
	this->max_y = WALL_HEIGHT;
	this->min_z = start_z;
	this->max_z = end_z;
	this->energy = 100;
	if (end_x-start_x < end_z-start_z) //horizontal wall
	{
		this->min_x+=LEN_GAP;
		this->max_x-=LEN_GAP;
		this->min_z+=DEPTH_GAP;
		this->max_z-=DEPTH_GAP;
	}
	else
	{
		this->min_x+=DEPTH_GAP;
		this->max_x-=DEPTH_GAP;
		this->min_z+=LEN_GAP;
		this->max_z-=LEN_GAP;
	}
	this->type = TYPE_WALL;

	this->update = notUpdate;
	this->display = drawWall;
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
	this->vel = 0.1;
	this->type = TYPE_BULLET;
	this->energy = 1;

	this->update = bulletUpdate;
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

	this->update = notUpdate;
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

	this->update = notUpdate;
	this->display = drawTop;
	this->onCollision = doNothing;
	return this;
}

object_t *newTurret (float min_x, float min_z) {
	object_t *this = newObject(min_x, 0, min_z);
	
	this->pos_y = 2.5;
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

	this->update = turretUpdate;
	this->display = drawTurret;
	this->onCollision = turretCollision;
	return this;
}

object_t *newCube(float min_x, float min_z) {
	object_t *this = newObject(min_x, 0, min_z);
	this->pos_y = 1;
	this->max_y = 2;//this->pos_y + 1;
	this->min_x = this->pos_x - 1;
	this->min_z = this->pos_z - 1;
	this->max_x = this->pos_x + 1;
	this->max_z = this->pos_z + 1;
	this->type = TYPE_CUBE;

	this->update = notUpdate;
	this->display = drawCube;
	this->onCollision = cubeCollision;
	return this;
}

object_t *newObject(float min_x, float min_y, float min_z) {
	object_t *this = malloc(sizeof(object_t));
	this->min_x = min_x;
	this->min_y = min_y;
	this->min_z = min_z;
	this->max_x = min_x+CELLSIZE;
	this->max_y = WALL_HEIGHT;
	this->max_z = min_z+CELLSIZE;
	this->energy = 100;
	this->rot_x = this->rot_y = 0;
	this->update = notUpdate;
	
	double half_cellsize = (double)CELLSIZE/2.0;
	this->pos_x = min_x+half_cellsize;
	this->pos_y = min_y;
	this->pos_z = min_z+half_cellsize;
	
	return this;
}
