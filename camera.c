#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "camera.h"
//~ #include "scene.h"
#include "objectlist.h"

// externalized instance
camera_t *cam;

void move_forward (void)
{
	float rot_x_rad = cam->character->rot_x / 180*PI;
	float rot_y_rad = cam->character->rot_y / 180*PI;
	
	cam->character->pos_x += sin(rot_y_rad)*cam->character->vel;
	cam->character->pos_z += cos(rot_y_rad)*cam->character->vel;
	cam->character->pos_y += sin(rot_x_rad)*cam->character->vel;
	
	if (cam->character->pos_y < cam->min_y)
		cam->character->pos_y = cam->min_y;
	if (cam->character->pos_y > cam->max_y)
		cam->character->pos_y = cam->max_y;
}

void move_backward (void)
{
	float rot_x_rad = cam->character->rot_x / 180*PI;
	float rot_y_rad = cam->character->rot_y / 180*PI;
	
	cam->character->pos_x -= sin(rot_y_rad)*cam->character->vel;
	cam->character->pos_z -= cos(rot_y_rad)*cam->character->vel;
	cam->character->pos_y -= sin(rot_x_rad)*cam->character->vel;
	
	if (cam->character->pos_y < cam->min_y)
		cam->character->pos_y = cam->min_y;
	if (cam->character->pos_y > cam->max_y)
		cam->character->pos_y = cam->max_y;
}

void strafe_left (void)
{
	float rot_x_rad = cam->character->rot_x / 180*PI;
	float rot_y_rad = cam->character->rot_y / 180*PI;
	
	cam->character->pos_x += cos(rot_y_rad)*cam->character->vel;
	cam->character->pos_z -= sin(rot_y_rad)*cam->character->vel;
}

void strafe_right (void)
{
	float rot_x_rad = cam->character->rot_x / 180*PI;
	float rot_y_rad = cam->character->rot_y / 180*PI;
	
	cam->character->pos_x -= cos(rot_y_rad)*cam->character->vel;
	cam->character->pos_z += sin(rot_y_rad)*cam->character->vel;
}

void rotate_left (void)
{
	cam->character->rot_y += 0.3;
}

void rotate_right (void)
{
	cam->character->rot_y -= 0.3;
}

void update (void)
{
	glRotatef (-cam->character->rot_x, 1.0, 0.0, 0.0);
	glRotatef (-cam->character->rot_y, 0.0, 1.0, 0.0);
	glRotatef (180, 0.0, 1.0, 0.0);
	glTranslatef (-cam->character->pos_x, -cam->character->pos_y, -cam->character->pos_z);	
}

void init_camera (void)
{
	cam = malloc (sizeof (camera_t));

	cam->move_forward = move_forward;
	cam->move_backward = move_backward;
	cam->strafe_left = strafe_left;
	cam->strafe_right = strafe_right;
	cam->rotate_left = rotate_left;
	cam->rotate_right = rotate_right;
	cam->update = update;
	cam->min_y = 3;
	cam->max_y = 3;
	
	cam->character = newCharacter (10, 3, 10);
	object_list->append (cam->character);
}

void clean_camera (void)
{
	free (cam);
}

