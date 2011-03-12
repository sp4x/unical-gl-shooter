#ifndef CAMERA_H
#define CAMERA_H

#define PI 3.141592654f;

#include "object.h"

typedef struct camera_t
{
	object_t *character;
	// movements limits
	int min_y, max_y;
	
	void (*move_forward) (void);
	void (*move_backward) (void);
	void (*strafe_left) (void);
	void (*strafe_right) (void);
	void (*rotate_left) (void);
	void (*rotate_right) (void);
	void (*update) (void);
	
} camera_t;

extern camera_t *cam;

void init_camera (void);
void clean_camera (void);

#endif
