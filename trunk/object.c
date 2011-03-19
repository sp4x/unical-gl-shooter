
#include <stdio.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <math.h>

#include "object.h"
#include "texture.h"
#include "camera.h"
#include "util.h"
#include "scene.h"
#include "particles.h"
#include "hud.h"
#include "solar_system.h"

#define COLLISION_GAP 0.1

void nullCollision (object_t *this, object_t *obj) {}
void doNothing (object_t *this) {}

int inGap(float c, object_t *obj, int coord) {
	enum {X, Y, Z};
	if ( coord == X)
		return c > obj->min_x && c < obj->max_x;
	if ( coord == Y )
		return c > obj->min_y && c < obj->max_y;
	return c > obj->min_z && c < obj->max_z;
}

/* check if this has a collision with obj 
 */
int hasCollision (object_t *this, object_t *obj) 
{
	if (this->type == TYPE_BULLET && this->data == obj)
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
		if (zside == UP && this->max_z + cam->mov_z >= obj->min_z)
			cam->mov_z = 0;
		if (zside == DOWN && this->min_z + cam->mov_z <= obj->max_z)
			cam->mov_z = 0;
	}
}

/****** Draw functions *******/

void drawWall (object_t *this) {
	float min_x = this->min_x;
	float max_x = this->max_x;
	float min_z = this->min_z;
	float max_z = this->max_z;
	
	float alpha = this->transparent ? 0.5 : 1.0;
	glColor4f(0, 0.5, 1, alpha);
	loadTexture(TEXTURE_METAL_PLATE_FILL);
	   
	float sx = max_x/10, sz = max_z/10, t = WALL_HEIGHT/2;
	//~ float sx = 1, sz = 1, t = 1;
	glBegin(GL_QUADS);
	//north wall
	glNormal3f(0,0,-1);
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
	glColor3f(0.5,0.5,0.5);
	loadTexture(TEXTURE_WHITE);
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
	glColor3f(1,1,1);
	loadTexture(TEXTURE_BUMPPLAT);
	GLUquadricObj *quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
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
	glColor4f(0.5, 1, 0, 0.75);
	glPushMatrix();
		glTranslatef(this->pos_x, this->pos_y, this->pos_z);
		glRotatef(this->rot_y, 0, 1, 0);
		glutSolidCube(CELLSIZE);
	glPopMatrix();
}

void drawExplosion (object_t *this)
{
	explosion_t *explosion = (explosion_t *) this->data;
	explosion->display (explosion);
}

void drawBlood (object_t *this)
{
	float width = glutGet(GLUT_SCREEN_WIDTH); 
	float height = glutGet(GLUT_SCREEN_HEIGHT);
	object_t *character = cam->character;
	float rot_y = character->rot_y;
	float rot_x = character->rot_x;
	printf("%f\n", rot_x);
	float x = character->pos_x+sin(rot_y*DEG_TO_RAD)*2;
	float y = character->pos_y+sin(rot_x*DEG_TO_RAD)*2;
	float z = character->pos_z+cos(rot_y*DEG_TO_RAD)*2;
	rot_y-=180;
	glPushMatrix();
		glTranslatef(x,y,z);
		glRotatef(rot_x, 1, 0, 0);
		glRotatef(rot_y, 0, 1, 0);
		glColor4fv(this->data);
		glRectf(-100,-100,100,100);
	glPopMatrix();
}

/****** onCollision functions *******/

void turretCollision (object_t *this, object_t *obj)
{
	if (obj->type == TYPE_BULLET)
		this->energy -= 3;
	if (this->energy <= 0)
	{
		float pos[3] = {this->pos_x, this->pos_y, this->pos_z};
		float color[3] = {1,0,0};
		object_t *explosion = newExplosion (pos, 2000, 90, 600, 0.2, color, 0.005);
		scene->add (explosion);
	}
}

void bulletCollision (object_t *this, object_t *obj)
{
	this->energy = 0;
	if (obj->type != TYPE_CHARACTER) {
		float pos[3] = {this->pos_x, this->pos_y, this->pos_z};
		float color[3] = {1,1,1};
		object_t *explosion = newExplosion (pos, 0, 30, 30, 0.05, color, 0.01);
		scene->add (explosion);
	}
}

void characterCollision (object_t *this, object_t *obj)
{
	if (obj->type == TYPE_BULLET) {
		this->energy -= 1;
		show_blood();
	}
}

void cubeCollision (object_t *this, object_t *obj)
{
	if ( obj->type == TYPE_BULLET ) {
		cam->character->score++;
		this->energy = 0;
		float pos[3] = {this->pos_x, this->pos_y, this->pos_z};
		float color[3] = {0,0.5,0.9};
		object_t *explosion = newExplosion (pos, 2000, 50, 600, 0.2, color, 0.005);
		scene->add (explosion);
	}
}

/****** Update functions *******/

void characterUpdate (object_t *this) 
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
	if (this->curr_time - this->last_time > 0.1)
	{
		this->last_time = this->curr_time;
		object_t *bullet = newBullet (this);
		scene->add (bullet);
		float pos[3] = {bullet->pos_x, bullet->pos_y, bullet->pos_z};
		float color[3] = {1,1,1};
		object_t *explosion = newExplosion (pos, 100, 0, 70, 0.1, color, 0.01);
		scene->add (explosion);
	}
}

void explosionUpdate (object_t *this)
{
	explosion_t *explosion = (explosion_t *) this->data;
	explosion->update (explosion);
	if (explosion->lifetime <= 0) {
		this->energy = 0;
		delete_explosion (explosion);
		this->data = NULL;
	}
}

void cubeUpdate (object_t *this)
{
	this->rot_y+=0.5;
	if ( this->rot_y == 360 )
		this->rot_y = 0;
}

void bloodUpdate (object_t *this)
{
	float *color = this->data;
	color[3] -= 0.01;
	if (color[3] <= 0) {
		this->energy = 0;
		free(this->data);
	}
}

/****** Create functions *******/

object_t *newSolarSystem (int pos_x, int pos_y, int pos_z)
{
	object_t *this = newObject(0,0,0);
	this->pos_x = pos_x;
	this->pos_y = pos_y;
	this->pos_z = pos_z;
	this->update = updateSolarSystem;
	this->display = drawSolarSystem;
	return this;
}

object_t *newCharacter (int pos_x, int pos_y, int pos_z)
{
	object_t *this = newObject(0,0,0);
	this->type = TYPE_CHARACTER;
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
	this->collides = 1;
	
	// init timer
	this->curr_time = get_time();
	this->last_time = get_time();
	
	this->onCollision = characterCollision;
	this->update = characterUpdate;
	return this;
}

object_t *newExplosion (float *pos, int p, int d, int lifetime, float scale, float *color, double speed)
{
	object_t *this = newObject(0,0,0);
	this->data = new_explosion (pos, p, d, lifetime, scale, color, speed);
	this->update = explosionUpdate;
	this->display = drawExplosion;
	return this;
}

object_t *newWall (float start_x, float end_x, float start_z, float end_z) 
{
	object_t *this = newObject(start_x, 0, start_z);
	this->max_x = end_x;
	this->max_y = WALL_HEIGHT;
	this->max_z = end_z;
	this->type = TYPE_WALL;

	this->display = drawWall;
	return this;
}

object_t *newBullet (struct object_t *owner)
{
	object_t *this = newObject(0,0,0);
	
	//~ this->owner_type = owner->type;
	this->data = owner;
	this->pos_x = owner->pos_x;
	this->pos_y = owner->pos_y;
	this->pos_z = owner->pos_z;
	this->rot_x = owner->rot_x;
	this->rot_y = owner->rot_y;
	this->vel = 0.1;
	this->type = TYPE_BULLET;
	this->energy = 1;
	this->collides = 1;

	this->update = bulletUpdate;
	this->display = drawBullet;
	this->onCollision = bulletCollision;
	return this;
}

object_t *newFloor (float max_x, float max_y, float max_z)
{
	object_t *this = newObject(0, -CELLSIZE, 0);
	this->max_x = max_x;
	this->max_y = 0;
	this->max_z = max_z;
	this->type = TYPE_FLOOR;

	this->display = drawFloor;
	return this;
}

object_t *newTop (float max_x, float max_y, float max_z)
{
	object_t *this = newObject(0, max_y, 0);
	this->max_x = max_x;
	this->max_y = max_y+CELLSIZE;
	this->max_z = max_z;
	this->type = TYPE_TOP;
	this->energy = 1;

	this->display = drawTop;
	return this;
}

object_t *newTurret (float min_x, float min_z) {
	object_t *this = newObject(min_x, 0, min_z);
	
	this->pos_y = CELLSIZE*1.25;
	this->min_x = this->pos_x - 1;
	this->max_x = this->pos_x + 1;
	this->min_y = 0;
	this->max_y = this->pos_y + 1;
	this->min_z = this->pos_z - 1;
	this->max_z = this->pos_z + 1;
	this->type = TYPE_TURRET;
	this->energy = 100;
	this->collides = 1;
	
	// init timer
	this->curr_time = get_time();
	this->last_time = get_time();

	this->update = turretUpdate;
	this->display = drawTurret;
	this->onCollision = turretCollision;
	return this;
}

object_t *newCube(float min_x, float min_z) {
	double half_cellsize = (double)CELLSIZE/2.0;
	object_t *this = newObject(min_x, half_cellsize, min_z);
	//~ this->pos_y = this->min_y+half_cellsize;
	//~ this->min_x = this->pos_x - half_cellsize;
	//~ this->min_z = this->pos_z - half_cellsize;
	this->max_y = this->pos_y + half_cellsize;
	this->max_x = this->pos_x + half_cellsize;
	this->max_z = this->pos_z + half_cellsize;
	this->type = TYPE_CUBE;
	this->transparent = 1;

	this->display = drawCube;
	this->update = cubeUpdate;
	this->onCollision = cubeCollision;
	return this;
}

object_t *newBlood() {
	object_t *this = newObject(0,0,0);
	this->type = TYPE_BLOOD;
	this->transparent = 1;
	float color[] = {1, 0, 0, 1};
	this->data = malloc(sizeof(color));
	memcpy(this->data, color, sizeof(color));
	this->update = bloodUpdate;
	this->display = drawBlood;
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
	this->energy = 1;
	this->rot_x = this->rot_y = 0;
	this->transparent = 0;
	this->collides = 0;
	this->update = doNothing;
	this->display = doNothing;
	this->onCollision = nullCollision;
	
	double half_cellsize = (double)CELLSIZE/2.0;
	this->pos_x = min_x+half_cellsize;
	this->pos_y = min_y+half_cellsize;
	this->pos_z = min_z+half_cellsize;
	
	return this;
}