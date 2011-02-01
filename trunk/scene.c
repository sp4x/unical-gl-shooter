
#include <GL/gl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "scene.h"
#include "object.h"
#include "objectlist.h"
#include "texture.h"

#define WALL_GAP (CELLSIZE-1)/2
#define COLLISION_GAP 0.1

scene_t *scene;

char *buffer;
int lines, cols;

char getSceneCell(int i, int j) {
	int index = i*cols+j;
	return buffer[index];
}

void drawWall(float min_x, float max_x, float min_z, float max_z) {
	loadTexture(TEXTURE_BRICK);
	min_x+=WALL_GAP; max_x-=WALL_GAP; min_z+=WALL_GAP; max_z-=WALL_GAP;
	float sx = max_x/10, sz = max_z/10, t = HEIGHT/2;
	glBegin(GL_QUADS);
	//north wall
	glColor3f(1,1,1);
	glTexCoord2d( sx, 0);
	glVertex3f(max_x, 0, min_z);
	glTexCoord2d( 0, 0);
	glVertex3f(min_x, 0, min_z);
	glTexCoord2d( 0, t);
	glVertex3f(min_x, HEIGHT, min_z);
	glTexCoord2d( sx, t);
	glVertex3f(max_x, HEIGHT, min_z);
	//south wall
	//~ glColor3f(0,0,1);
	glTexCoord2d( 0, 0);
	glVertex3f(min_x, 0, max_z);
	glTexCoord2d( sx, 0);
	glVertex3f(max_x, 0, max_z);
	glTexCoord2d( sx, t);
	glVertex3f(max_x, HEIGHT, max_z);
	glTexCoord2d( 0, t);
	glVertex3f(min_x, HEIGHT, max_z);
	
	//east wall
	//~ glColor3f(0,1,0);
	glTexCoord2d( sz, 0);
	glVertex3f(max_x, 0, max_z);
	glTexCoord2d( 0, 0);
	glVertex3f(max_x, 0, min_z);
	glTexCoord2d( 0, t);
	glVertex3f(max_x, HEIGHT, min_z);
	glTexCoord2d( sz, t);
	glVertex3f(max_x, HEIGHT, max_z);
	//west wall
	//~ glColor3f(1,1,0);
	glTexCoord2d( 0, 0);
	glVertex3f(min_x, 0, min_z);
	glTexCoord2d( sz, 0);
	glVertex3f(min_x, 0, max_z);
	glTexCoord2d( sz, t);
	glVertex3f(min_x, HEIGHT, max_z);
	glTexCoord2d( 0, t);
	glVertex3f(min_x, HEIGHT, min_z);
	glEnd();
}

void drawBound(float min_x, float max_x, float min_z, float max_z) {
	glBegin(GL_LINE_LOOP);
		//north 
		glColor3f(0,1,0);
		glVertex3f(max_x, 0, min_z);
		glVertex3f(min_x, 0, min_z);
		glVertex3f(min_x, HEIGHT, min_z);
		glVertex3f(max_x, HEIGHT, min_z);
		//east 
		glVertex3f(max_x, 0, max_z);
		glVertex3f(max_x, 0, min_z);
		glVertex3f(max_x, HEIGHT, min_z);
		glVertex3f(max_x, HEIGHT, max_z);
		//south 
		glVertex3f(min_x, 0, max_z);
		glVertex3f(max_x, 0, max_z);
		glVertex3f(max_x, HEIGHT, max_z);
		glVertex3f(min_x, HEIGHT, max_z);
		//west 
		glVertex3f(min_x, 0, min_z);
		glVertex3f(min_x, 0, max_z);
		glVertex3f(min_x, HEIGHT, max_z);
		glVertex3f(min_x, HEIGHT, min_z);
	glEnd();
}

void drawBounds() {
	object_list_iterator *i = object_list->first;
	while (i != NULL) {
		object_t *obj = i->value;
		drawBound(obj->min_x, obj->max_x, obj->min_z, obj->max_z);
		i = i->next;
	}
}

void drawBullet (object_t *obj)
{
	float rot_x_rad = obj->rot_x / 180*3.141592654f;
	float rot_y_rad = obj->rot_y / 180*3.141592654f;
	
	obj->pos_x += sin(rot_y_rad)*obj->vel;
	obj->pos_z += cos(rot_y_rad)*obj->vel;
	obj->pos_y += sin(rot_x_rad)*obj->vel;

	glPushMatrix();
	glColor3f (1, 1, 0);
	glTranslatef (obj->pos_x, obj->pos_y, obj->pos_z);
	glutSolidSphere (0.01, 8, 8);
	glPopMatrix();
}

void drawObjects() {
	object_list_iterator *i = object_list->first;
	while (i != NULL) {
		object_t *obj = i->value;
		if (obj->type == TYPE_WALL) {
			drawWall(obj->min_x, obj->max_x, obj->min_z, obj->max_z);
			i = i->next;
		}
		else if (obj->type == TYPE_BULLET)
		{
			i = i->next;
			object_t *coll = scene->checkCollisions (obj->pos_x, obj->pos_y, obj->pos_z, NULL, NULL, NULL);
			if (coll == NULL) {
				drawBullet(obj);
			} else if (coll->type == TYPE_TURRET) {
				// do something
			} else {
				object_list->delete (obj);
			}
				
		}
	}
}

void drawTop() {
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_QUADS);
		glVertex3f(0, HEIGHT, 0);
		glVertex3f(cols*CELLSIZE, HEIGHT, 0);
		glVertex3f(cols*CELLSIZE, HEIGHT, lines*CELLSIZE);
		glVertex3f(0, HEIGHT, lines*CELLSIZE);
	glEnd();
}

void drawFloor() {
	glColor3f(1,1,1);
	loadTexture(TEXTURE_METAL2);
	int t = lines*CELLSIZE;
	int s = cols*CELLSIZE;
	float x = cols*CELLSIZE;
	float z = lines*CELLSIZE;
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

void drawAll() {
	drawObjects();
	drawFloor();
	drawTop();
	//drawBounds();
}


char *readData(char *file, int *lines, int *cols) {
	//buffer storing map size information
	char dataInfo[10];
	
	FILE* f = fopen(file, "r");	
	
	//get the first line
	fgets(dataInfo, sizeof(dataInfo), f);
	//parse data
	*lines = atof(strtok(dataInfo, " "));
	*cols = atof(strtok(NULL, " \r\n"));
	int bufsize = (*lines)*(*cols);
	//create the buffer
	char *buffer = malloc(bufsize);
	
	//get all data and putting it together
	char *buf_ptr = buffer;
	while ( fgets(buf_ptr, bufsize, f) !=NULL ) {
		buf_ptr+=(*cols);
	}
	
	fclose(f);
	
	return buffer;
}

object_t *newWall(float min_x, float max_x, float min_z, float max_z) {
	object_t *wall = malloc(sizeof(object_t));
	wall->min_x = min_x;
	wall->max_x = max_x;
	wall->min_y = 0;
	wall->max_y = HEIGHT;
	wall->min_z = min_z;
	wall->max_z = max_z;
	wall->type = TYPE_WALL;
	return wall;
}

void findWalls() {
	int i,j;
	object_t *wall = NULL;
	
	//horizontal walls
	for (i=0; i<lines; i++)
		for (j=0; j<cols; j++) {
			if ( !wall && getSceneCell(i,j) == WALL && j<cols-1 && getSceneCell(i,j+1) != FREE_SPACE ) {
				wall = newWall( j*CELLSIZE, 0, i*CELLSIZE, (i+1)*CELLSIZE);
			}
			else if ( wall ) {
				if (getSceneCell(i,j) != WALL || j == cols-1) {
					wall->max_x = (j+1)*CELLSIZE;
					object_list->append(wall);
					wall = NULL;
				}
			}
		}

	//vertical walls
	for (j=0; j<cols; j++)
		for (i=0; i<lines; i++) {
			if ( !wall && getSceneCell(i,j) == WALL && i<lines-1 && getSceneCell(i+1,j) == WALL ) {
				if ((j<cols-1 && getSceneCell(i,j+1) != WALL) || j == cols-1)
					if ((j>0 && getSceneCell(i,j-1) != WALL) || j == 0) {
						wall = newWall(j*CELLSIZE, (j+1)*CELLSIZE, (i-0.8)*CELLSIZE, 0);
					}
			} else if ( wall ) {
				if (getSceneCell(i,j) != WALL || i == lines - 1 ) {
					wall->max_z = (i+0.8)*CELLSIZE;
					object_list->append(wall);
					wall = NULL;
				}
			}
		}
}

object_t *checkCollisions(float x, float y, float z, float *available_x, float *available_y, float *available_z) {
	object_list_iterator *i = object_list->first;
	object_t *obj = NULL;
	while (i != NULL) {
		if ( hasCollision(i->value, x, y, z, available_x, available_y, available_z) )
			obj = i->value;
		i = i->next;
	}
	return obj;
}

void loadScene(char *file) {
	scene = malloc(sizeof(scene_t));
	buffer = readData(file, &lines, &cols);
	createObjectList();
	findWalls();
	scene->display = drawAll;
	scene->checkCollisions = checkCollisions;
	free(buffer);
}


void clean() {
	free(scene);
	object_list->clear();
	cleanTextures();
}









