#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "camera.h"
#include "scene.h"

// externalized instance
camera_t *cam;

void move_forward (void)
{
	float rot_x_rad = cam->rot_x / 180*PI;
	float rot_y_rad = cam->rot_y / 180*PI;
	
	cam->pos_x += sin(rot_y_rad)*cam->vel;
	cam->pos_z += cos(rot_y_rad)*cam->vel;
	cam->pos_y += sin(rot_x_rad)*cam->vel;
	
	scene->checkCollisions(cam->pos_x, cam->pos_y, cam->pos_z, &cam->pos_x, &cam->pos_y, &cam->pos_z);

	if (cam->pos_y < cam->min_y)
		cam->pos_y = cam->min_y;
	if (cam->pos_y > cam->max_y)
		cam->pos_y = cam->max_y;
}

void move_backward (void)
{
	float rot_x_rad = cam->rot_x / 180*PI;
	float rot_y_rad = cam->rot_y / 180*PI;
	
	cam->pos_x -= sin(rot_y_rad)*cam->vel;
	cam->pos_z -= cos(rot_y_rad)*cam->vel;
	cam->pos_y -= sin(rot_x_rad)*cam->vel;
	
	scene->checkCollisions(cam->pos_x, cam->pos_y, cam->pos_z, &cam->pos_x, &cam->pos_y, &cam->pos_z);

	if (cam->pos_y < cam->min_y)
		cam->pos_y = cam->min_y;
	if (cam->pos_y > cam->max_y)
		cam->pos_y = cam->max_y;
}

void strafe_left (void)
{
	float rot_x_rad = cam->rot_x / 180*PI;
	float rot_y_rad = cam->rot_y / 180*PI;
	
	cam->pos_x += cos(rot_y_rad)*cam->vel;
	cam->pos_z -= sin(rot_y_rad)*cam->vel;
	
	scene->checkCollisions(cam->pos_x, cam->pos_y, cam->pos_z, &cam->pos_x, &cam->pos_y, &cam->pos_z);
}

void strafe_right (void)
{
	float rot_x_rad = cam->rot_x / 180*PI;
	float rot_y_rad = cam->rot_y / 180*PI;
	
	cam->pos_x -= cos(rot_y_rad)*cam->vel;
	cam->pos_z += sin(rot_y_rad)*cam->vel;
	
	scene->checkCollisions(cam->pos_x, cam->pos_y, cam->pos_z, &cam->pos_x, &cam->pos_y, &cam->pos_z);
}

void rotate_left (void)
{
	cam->rot_y += 0.3;
}

void rotate_right (void)
{
	cam->rot_y -= 0.3;
}

void update (void)
{
	glRotatef (-cam->rot_x, 1.0, 0.0, 0.0);
	glRotatef (-cam->rot_y, 0.0, 1.0, 0.0);
	glRotatef (180, 0.0, 1.0, 0.0);
	glTranslatef (-cam->pos_x, -cam->pos_y, -cam->pos_z);
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
	cam->rot_x = 0;
	cam->rot_y = 0;
	cam->pos_x = 10;
	cam->pos_y = 3;
	cam->pos_z = 10;
	cam->vel = 0.01;
	cam->min_y = 3;
	cam->max_y = 3;
}

void clean_camera (void)
{
	free (cam);
}

