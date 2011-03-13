
#include <stdio.h>
#include <stdlib.h>

#include "scene.h"
#include "object.h"
#include "objectlist.h"
#include "texture.h"
#include "hud.h"
#include "util.h"

scene_t *scene;

char *buffer;
int lines, cols;

char getSceneCell(int i, int j) {
	int index = i*cols+j;
	return buffer[index];
}

void placeLights() {
	GLfloat position[] = {cols/2*CELLSIZE, WALL_HEIGHT*2, lines/2*CELLSIZE, 1};
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	//~ GLfloat spot_direction[] = {0,-1,0};
	//~ glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
}

void drawScene() {
	placeLights();
	object_list_iterator *i = object_list->first;
	while (i != NULL) {
		object_t *obj = i->value;
		if (obj->energy > 0) {
			obj->display(obj);
			i = i->next;
		} else {
			if (obj->type == TYPE_CHARACTER)
				game_over();
			i = i->next;
			object_list->delete (obj);
		}
	}
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

void findWalls() {
	int i,j;
	object_t *wall = NULL;
	
	//horizontal walls
	for (i=0; i<lines; i++)
		for (j=0; j<cols; j++) {
			if ( !wall && getSceneCell(i,j) == TURRET) {
				object_t *turret = newTurret( j*CELLSIZE, (j+1)*CELLSIZE, i*CELLSIZE, (i+1)*CELLSIZE);
				object_list->append(turret);
			}
			else if ( !wall && getSceneCell(i,j) == WALL && j<cols-1 && getSceneCell(i,j+1) != FREE_SPACE ) {
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

object_t *checkCollisions(object_t *collider, int modify ) {
	float *available_x, *available_y, *available_z;
	if (!modify) {
		available_x = available_y = available_z = NULL;
	}
	else {
		available_x = &collider->pos_x;
		available_y = &collider->pos_y;
		available_z = &collider->pos_z;
	}
	object_list_iterator *i = object_list->first;
	object_t *obj = NULL;
	while (i != NULL) {
		if ( hasCollision(i->value, collider->pos_x, collider->pos_y, collider->pos_z, available_x, available_y, available_z) )
			obj = i->value;
		i = i->next;
	}
	return obj;
}

void updateFunc() {
	object_list_iterator *i = object_list->first;
	object_t *obj = NULL;
	while (i != NULL) {
		object_t *collider = i->value;
		if (collider->type == TYPE_BULLET) {
			obj = checkCollisions (collider, 0);
			if (obj != NULL && collider->owner != obj) {
				obj->onCollision (obj, collider);
				collider->onCollision (collider, obj);
			}
		}
		else if (collider->type == TYPE_CHARACTER) {
			obj = checkCollisions (collider, 1 );
		}
		i = i->next;
	}
}

void addLighting() {
	//~ GLfloat ambient[] = {0.1,0.1,0.1,0};
	//~ glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	
	GLfloat diffuse[] = {0.7,0.7,0.7,0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	
	glEnable(GL_LIGHT0);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void loadScene(char *file) {
	scene = malloc(sizeof(scene_t));
	buffer = readData(file, &lines, &cols);
	createObjectList();
	findWalls();
	float max_x = cols*CELLSIZE;
	float max_z = lines*CELLSIZE;
	object_t *floor = newFloor (max_x, WALL_HEIGHT, max_z);
	object_list->append (floor);
	object_t *top = newTop (max_x, WALL_HEIGHT, max_z);
	object_list->append (top);
	
	scene->display = drawScene;
	scene->update = updateFunc;
	scene->checkCollisions = checkCollisions;
	free(buffer);
	
	addLighting();
}


void clean() {
	free(scene);
	object_list->clear();
	cleanTextures();
}











