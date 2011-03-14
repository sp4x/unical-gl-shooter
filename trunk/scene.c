
#include <stdio.h>
#include <stdlib.h>

#include "scene.h"
#include "object.h"
#include "objectlist.h"
#include "texture.h"
#include "hud.h"
#include "util.h"

#define COLLISION_SLOTS 5

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

void drawBounds (void)
{
	object_list_iterator *i = object_list->first;
	glDisable (GL_LIGHTING);
	while (i != NULL)
	{
		object_t *obj = i->value;
		if (obj->type != TYPE_BULLET) 
		{
			float min_x = obj->min_x;
			float min_y = obj->min_y;
			float min_z = obj->min_z;
			float max_x = obj->max_x;
			float max_y = obj->max_y;
			float max_z = obj->max_z;
			glBegin (GL_LINE_LOOP);
				glVertex3f (min_x, min_y, min_z);
				glVertex3f (min_x, min_y, max_z);
				glVertex3f (max_x, min_y, max_z);
				glVertex3f (max_x, min_y, min_z);
			glEnd();
			glBegin (GL_LINE_LOOP);
				glVertex3f (min_x, max_y, min_z);
				glVertex3f (min_x, max_y, max_z);
				glVertex3f (max_x, max_y, max_z);
				glVertex3f (max_x, max_y, min_z);
			glEnd();	
			glBegin (GL_LINE_LOOP);
				glVertex3f (min_x, min_y, min_z);
				glVertex3f (min_x, min_y, max_z);
				glVertex3f (min_x, max_y, max_z);
				glVertex3f (min_x, max_y, min_z);
			glEnd();	
			glBegin (GL_LINE_LOOP);
				glVertex3f (max_x, min_y, min_z);
				glVertex3f (max_x, min_y, max_z);
				glVertex3f (max_x, max_y, max_z);
				glVertex3f (max_x, max_y, min_z);
			glEnd();	
			glBegin (GL_LINE_LOOP);
				glVertex3f (min_x, min_y, min_z);
				glVertex3f (max_x, min_y, min_z);
				glVertex3f (max_x, max_y, min_z);
				glVertex3f (min_x, max_y, min_z);
			glEnd();
			glBegin (GL_LINE_LOOP);
				glVertex3f (min_x, min_y, max_z);
				glVertex3f (max_x, min_y, max_z);
				glVertex3f (max_x, max_y, max_z);
				glVertex3f (min_x, max_y, max_z);
			glEnd();
		}
		i = i->next;
	}
	glEnable (GL_LIGHTING);
}

void drawScene() {
	placeLights();
	drawBounds();
	if (showbounds)
		drawBounds();
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

void findObjects() {
	int i,j;
	object_t *wall = NULL;
	
	//horizontal walls
	for (i=0; i<lines; i++)
		for (j=0; j<cols; j++) {
			if ( !wall ) {
				if ( getSceneCell(i,j) == TURRET) {
					object_t *turret = newTurret( j*CELLSIZE, i*CELLSIZE );
					object_list->append(turret);
				}
				else if ( getSceneCell(i,j) == CUBE ) {
					object_t *cube = newCube( j*CELLSIZE, i*CELLSIZE );
					object_list->append(cube);
				}
				else if ( getSceneCell(i,j) == WALL && j<cols-1 && getSceneCell(i,j+1) != FREE_SPACE ) {
					wall = newWall( j*CELLSIZE, 0, i*CELLSIZE, (i+1)*CELLSIZE);
				}
			}
			else if (getSceneCell(i,j) != WALL || j == cols-1) {
					wall->max_x = (j)*CELLSIZE;
					object_list->append(wall);
					wall = NULL;
			}
		}

	//vertical walls
	for (j=0; j<cols; j++)
		for (i=0; i<lines; i++) {
			if ( !wall && getSceneCell(i,j) == WALL && i<lines-1 && getSceneCell(i+1,j) == WALL ) {
				if ((j<cols-1 && getSceneCell(i,j+1) != WALL) || j == cols-1)
					if ((j>0 && getSceneCell(i,j-1) != WALL) || j == 0) {
						wall = newWall(j*CELLSIZE, (j+1)*CELLSIZE, (i)*CELLSIZE, 0);
					}
			} else if ( wall ) {
				if (getSceneCell(i,j) != WALL || i == lines - 1 ) {
					wall->max_z = (i)*CELLSIZE;
					object_list->append(wall);
					wall = NULL;
				}
			}
		}
}

int checkCollisions (object_t *collider, object_t **with) 
{
	object_t *obj = NULL;
	int j=0;
	
	object_list_iterator *i = object_list->first;
	while (i != NULL) 
	{
		if (i->value->type != TYPE_BULLET)
			if (hasCollision (collider, i->value)) 
				with[j++] = i->value;
		i = i->next;
	}
	return j;
}

void updateFunc() 
{
	object_list_iterator *i = object_list->first;
	while (i != NULL) 
	{
		object_t *collider = i->value;
		if (collider->type == TYPE_BULLET || collider->type == TYPE_CHARACTER)
		{
			object_t *with[COLLISION_SLOTS];
			int with_size = checkCollisions (collider, with);
			int j;
			for (j=0; j<with_size; j++)
			{
				object_t *obj = with[j];
				//~ char *s, *s1;
				//~ switch (obj->type)
				//~ {
					//~ case TYPE_FLOOR: s = "floor"; break;
					//~ case TYPE_TOP: s = "top"; break;
					//~ case TYPE_WALL: s = "wall"; break;
					//~ case TYPE_TURRET: s = "turret"; break;
					//~ case TYPE_CHARACTER: s = "character"; break;
					//~ case TYPE_BULLET: s = "bullet"; break;
					//~ default: s = "nothing"; break;
				//~ }
				//~ switch (collider->type)
				//~ {
					//~ case TYPE_FLOOR: s1 = "floor"; break;
					//~ case TYPE_TOP: s1 = "top"; break;
					//~ case TYPE_WALL: s1 = "wall"; break;
					//~ case TYPE_TURRET: s1 = "turret"; break;
					//~ case TYPE_CHARACTER: s1 = "character"; break;
					//~ case TYPE_BULLET: s1 = "bullet"; break;
					//~ default: s1 = "nothing"; break;
				//~ }
				//~ printf("%s collides with %s\n", s1, s);
				obj->onCollision (obj, collider);
				collider->onCollision (collider, obj);
			}
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
	findObjects();
	float max_x = cols*CELLSIZE;
	float max_z = lines*CELLSIZE;
	object_t *floor = newFloor (max_x, WALL_HEIGHT, max_z);
	object_list->append (floor);
	object_t *top = newTop (max_x, WALL_HEIGHT, max_z);
	object_list->append (top);
	
	scene->display = drawScene;
	scene->update = updateFunc;
	//~ scene->checkCollisions = checkCollisions;
	free(buffer);
	
	addLighting();
}


void clean() {
	free(scene);
	object_list->clear();
	cleanTextures();
}











