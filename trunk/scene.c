
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
object_list_t *render_queue_opaque;
object_list_t *render_queue_transparent;
object_list_t *render_queue;

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
	object_list_iterator *i = render_queue->iterator;
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
	if (showbounds)
		drawBounds();
	object_list_iterator *i = render_queue->iterator;
	while (i != NULL) {
		object_t *obj = i->value;
		if (obj->energy > 0) {
			obj->display(obj);
			i = i->next;
		} else {
			if (obj->type == TYPE_CHARACTER)
				game_over();
			i = i->next;
			listDelete(render_queue, obj);
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
					scene->add(turret, QUEUE_OPAQUE);
				}
				else if ( getSceneCell(i,j) == CUBE ) {
					object_t *cube = newCube( j*CELLSIZE, i*CELLSIZE );
					scene->add(cube, QUEUE_OPAQUE);
				}
				else if ( getSceneCell(i,j) == WALL && j<cols-1 && getSceneCell(i,j+1) != FREE_SPACE ) {
					wall = newWall( j*CELLSIZE, 0, i*CELLSIZE, (i+1)*CELLSIZE);
				}
			}
			else if (getSceneCell(i,j) != WALL || j == cols-1) {
					wall->max_x = (j)*CELLSIZE;
					scene->add(wall, QUEUE_OPAQUE);
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
					scene->add(wall, QUEUE_OPAQUE);
					wall = NULL;
				}
			}
		}
}

int checkCollisions (object_t *collider, object_t **with) 
{
	object_t *obj = NULL;
	int j=0;
	
	object_list_iterator *i = render_queue->iterator;
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
	object_list_iterator *i = render_queue->iterator;
	while (i != NULL) 
	{
		object_t *current = i->value;

		if (current->type == TYPE_BULLET || current->type == TYPE_CHARACTER)
		{
			object_t *with[COLLISION_SLOTS];
			int with_size = checkCollisions (current, with);
			int j;
			for (j=0; j<with_size; j++)
			{
				object_t *obj = with[j];
				obj->onCollision (obj, current);
				current->onCollision (current, obj);
			}
		}
		current->update (current);
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

void addObject (object_t *obj, RenderQueueMode mode)
{
	listAappend(render_queue, obj);
	if (mode == QUEUE_OPAQUE)
		listAappend(render_queue_opaque, obj);
	else
		listAappend(render_queue_transparent, obj);
}

void loadScene(char *file) {
	scene = malloc(sizeof(scene_t));
	buffer = readData(file, &lines, &cols);
	
	render_queue = newObjectList();
	render_queue_opaque = newObjectList();
	render_queue_transparent = newObjectList();
	
	scene->display = drawScene;
	scene->update = updateFunc;
	scene->add = addObject;
	
	findObjects();
	float max_x = cols*CELLSIZE;
	float max_z = lines*CELLSIZE;
	object_t *floor = newFloor (max_x, WALL_HEIGHT, max_z);
	scene->add (floor, QUEUE_OPAQUE);
	object_t *top = newTop (max_x, WALL_HEIGHT, max_z);
	scene->add (top, QUEUE_OPAQUE);
	
	free(buffer);
	
	addLighting();
}


void clean() {
	free(scene);
	listClear(render_queue);
	cleanTextures();
}











