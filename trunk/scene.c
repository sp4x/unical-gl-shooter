
#include <GL/gl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "scene.h"
#include "object.h"
#include "objectlist.h"
#include "texture.h"

scene_t *scene;

char *buffer;
int lines, cols;

char getSceneCell(int i, int j) {
	int index = i*cols+j;
	return buffer[index];
}

void drawScene() {
	object_list_iterator *i = object_list->first;
	while (i != NULL) {
		object_t *obj = i->value;
		if (obj->energy > 0) {
			obj->display(obj);
			i = i->next;
		} else {
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
	float max_x = cols*CELLSIZE;
	float max_z = lines*CELLSIZE;
	object_t *floor = newFloor (max_x, WALL_HEIGHT, max_z);
	object_list->append (floor);
	object_t *top = newTop (max_x, WALL_HEIGHT, max_z);
	object_list->append (top);
	
	scene->display = drawScene;
	scene->checkCollisions = checkCollisions;
	free(buffer);
}


void clean() {
	free(scene);
	object_list->clear();
	cleanTextures();
}









