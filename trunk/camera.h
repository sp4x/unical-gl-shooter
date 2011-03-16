#ifndef CAMERA_H
#define CAMERA_H

#include "object.h"

typedef struct camera_t
{
	// the player object
	object_t *character;
	// movements limits
	int min_y, max_y;
	// movement's components
	float mov_x, mov_y, mov_z;

	// move functions
	void (*move_forward) (void);
	void (*move_backward) (void);
	void (*strafe_left) (void);
	void (*strafe_right) (void);
	void (*rotate_left) (void);
	void (*rotate_right) (void);
	// update function
	void (*update) (void);
	
} camera_t;

extern camera_t *cam;

void init_camera (void);
void clean_camera (void);

#endif
