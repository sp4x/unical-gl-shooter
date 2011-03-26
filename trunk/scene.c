
#include <stdio.h>
#include <stdlib.h>

#include "scene.h"
#include "object.h"
#include "objectlist.h"
#include "texture.h"
#include "hud.h"
#include "util.h"

#define COLLISION_SLOTS 5

/** scene instance */
scene_t *scene;
/** queue of object to update and render */
object_list_t *render_queue;
/** queue of opaque object (to render first) */
object_list_t *render_queue_opaque;
/** queue of transparent object (to render last) */
object_list_t *render_queue_transparent;

char *buffer;
int lines, cols;

float light_pos[8][4];
int n_lights = 0;

char getSceneCell(int i, int j)
{
	int index = i*cols+j;
	return buffer[index];
}

void placeLights()
{
	GLfloat position[] = {20, WALL_HEIGHT*2, 0, 1};
	glLightfv(GL_LIGHT7, GL_POSITION, position);
	GLfloat direction[] = {0,-1,-1};
	glLightfv(GL_LIGHT7, GL_SPOT_DIRECTION, direction);
	
	GLfloat cam_position[] = {cam->character->pos_x, cam->character->pos_y, cam->character->pos_z, 1};
	glLightfv(GL_LIGHT6, GL_POSITION, cam_position);
	float rot_y = cam->character->rot_y*DEG_TO_RAD;
	float rot_x = cam->character->rot_x*DEG_TO_RAD;
	GLfloat cam_direction[] = {sin(rot_y)*cos(rot_x), sin(rot_x), cos(rot_y)*cos(rot_x)};
	glLightfv(GL_LIGHT6, GL_SPOT_DIRECTION, cam_direction);

	GLfloat spot_direction[] = {0,-1,0};
	GLfloat emissive[] = {1,1,1,1};
	GLfloat none[] = {0,0,0,1};
	int i;
	for (i=0; i<n_lights; i++) {
		glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION, spot_direction);
		glLightfv(GL_LIGHT0+i, GL_POSITION, light_pos[i]);
		glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
		glPushMatrix();
			glColor3f(1,1,1);
			glTranslatef(light_pos[i][0], light_pos[i][1], light_pos[i][2]);
			glutSolidSphere(0.5,10,10);
		glPopMatrix();
		glMaterialfv(GL_FRONT, GL_EMISSION, none);
	}
}

/** DEBUG: draw bounds of each object */
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


void drawScene()
{
	placeLights();
	if (showbounds)
		drawBounds();
	object_list_t *queues[] = { render_queue_opaque, render_queue_transparent };
	int i;
	for (i=0; i<2; i++) {
		object_list_iterator *it = queues[i]->iterator;
		while (it != NULL) {
			object_t *obj = it->value;
			obj->display(obj);
			it = it->next;
		}
	}
}

char *readData(char *file, int *lines, int *cols)
{
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
	int window = 0;
	
	//horizontal walls
	for (i=0; i<lines; i++) {
		for (j=0; j<cols; j++) {
			if ( !wall ) {
				if ( getSceneCell(i,j) == TURRET) {
					object_t *turret = newTurret( j*CELLSIZE, i*CELLSIZE );
					scene->add(turret);
				}
				else if ( getSceneCell(i,j) == CUBE ) {
					object_t *cube = newCube( j*CELLSIZE, i*CELLSIZE );
					add_cube();
					scene->add(cube);
				}
				else if ( getSceneCell(i,j) == WALL && j<cols-1 && getSceneCell(i,j+1) != FREE_SPACE ) {
					wall = newWall( j*CELLSIZE, 0, i*CELLSIZE, (i+1)*CELLSIZE );
				}
				else if ( getSceneCell(i,j) == WINDOW && j<cols-1 && getSceneCell(i,j+1) != FREE_SPACE ) {
					wall = newWall( j-1*CELLSIZE, 0, i*CELLSIZE, (i+1)*CELLSIZE );
					wall->transparent = 1;
					window = 1;
				}
				else if ( getSceneCell(i,j) == LIGHT ) {
					light_pos[n_lights][0] = j*CELLSIZE+CELLSIZE/2;
					light_pos[n_lights][1] = CELLSIZE*5;
					light_pos[n_lights][2] = i*CELLSIZE+CELLSIZE/2;
					light_pos[n_lights][3] = 1;
					n_lights++;
				}
				//~ j++;
			}
			else if (window && (getSceneCell(i,j) != WINDOW || j == cols-1)) {
					wall->max_x = (j)*CELLSIZE;
					scene->add(wall);
					wall = NULL;
					window = 0;
					j--;
			}
			else if (!window && (getSceneCell(i,j) != WALL || j == cols-1)) {
					wall->max_x = (j)*CELLSIZE;
					scene->add(wall);
					wall = NULL;
			}
		}
	}

	//vertical walls
	for (j=0; j<cols; j++) {
		for (i=0; i<lines; i++) {
			if ( !wall && getSceneCell(i,j) == WALL && i<lines-1 && getSceneCell(i+1,j) == WALL ) {
				if ((j<cols-1 && getSceneCell(i,j+1) != WALL) || j == cols-1)
					if ((j>0 && getSceneCell(i,j-1) != WALL) || j == 0) {
						wall = newWall(j*CELLSIZE, (j+1)*CELLSIZE, (i)*CELLSIZE, 0);
					}
			} else if ( wall ) {
				if (getSceneCell(i,j) != WALL || i == lines - 1 ) {
					wall->max_z = (i)*CELLSIZE;
					scene->add(wall);
					wall = NULL;
				}
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

		if (current->collides)
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
		if (current->energy > 0) {
			i = i->next;
		} else {
			if (current->type == TYPE_CHARACTER)
				game_over();
			i = i->next;
			scene->remove (current);
		}
	}
}

void addLighting() {
	GLfloat  white[] =  { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat  red[] =  { 1.0f, 0, 0, 1.0f };
	GLfloat  none[] = { 0, 0, 0, 0};
	
	glLightfv(GL_LIGHT7, GL_AMBIENT, none);
	glLightfv(GL_LIGHT7, GL_DIFFUSE, white);
	glLightf(GL_LIGHT7, GL_SPOT_CUTOFF, 30.0f);
	glLightf(GL_LIGHT7, GL_SPOT_EXPONENT, 4);
	glEnable(GL_LIGHT7);
	
	glLightfv(GL_LIGHT6, GL_AMBIENT, none);
	glLightfv(GL_LIGHT6, GL_DIFFUSE, red);
	glLightf(GL_LIGHT6, GL_SPOT_CUTOFF, 15.0f);
	glLightf(GL_LIGHT6, GL_SPOT_EXPONENT, 64);

	//~ glLightModelfv(GL_LIGHT_MODEL_AMBIENT, none);
	
	int i;
	for (i=0; i<n_lights; i++) {
		glLightfv(GL_LIGHT0+i,GL_AMBIENT,none);
		glLightfv(GL_LIGHT0+i,GL_DIFFUSE,white);
		glLightfv(GL_LIGHT0+i,GL_SPECULAR,white);
		glLightf(GL_LIGHT0+i, GL_SPOT_CUTOFF, 45.0f);
		glLightf(GL_LIGHT0+i, GL_SPOT_EXPONENT, 16);
		glEnable(GL_LIGHT0+i);
	}
	
	glMaterialfv(GL_FRONT, GL_SPECULAR,white);
    glMateriali(GL_FRONT, GL_SHININESS,1);
}

void addObject (object_t *obj)
{
	listAppend (render_queue, obj);
	if (!obj->transparent)
		listAppend (render_queue_opaque, obj);
	else
		listAppend (render_queue_transparent, obj);
}

void removeObject (object_t *obj)
{
	listDelete (render_queue, obj);
	if (obj->transparent)
		listDelete (render_queue_transparent, obj);
	else
		listDelete (render_queue_opaque, obj);
	//~ if (obj->data != NULL)
		//~ free (obj->data);
	free (obj);
}

void loadScene (char *file)
{
	scene = malloc(sizeof(scene_t));
	buffer = readData(file, &lines, &cols);
	
	render_queue = newObjectList();
	render_queue_opaque = newObjectList();
	render_queue_transparent = newObjectList();
	
	scene->display = drawScene;
	scene->update = updateFunc;
	scene->add = addObject;
	scene->remove = removeObject;

	object_t *skybox = newSkybox ();
	scene->add (skybox);

	findObjects();
	float max_x = cols*CELLSIZE;
	float max_z = lines*CELLSIZE;
	object_t *floor = newFloor (max_x, WALL_HEIGHT, max_z);
	scene->add (floor);
	object_t *top = newTop (max_x, WALL_HEIGHT, max_z);
	scene->add (top);
	
	object_t *solarSystem = newSolarSystem (20, 3, -10);
	scene->add (solarSystem);


	free(buffer);
	
	addLighting();
}

void clean()
{
	free(scene);
	listClear(render_queue, 1);
	listClear(render_queue_opaque, 0);
	listClear(render_queue_transparent, 0);
	cleanTextures();
}











