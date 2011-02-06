#ifndef CAMERA_H
#define CAMERA_H

#define PI 3.141592654f;

typedef struct camera_t
{
	// camera position
	float pos_x, pos_y, pos_z;
	// camera rotation
	float rot_x, rot_y;
	// movement's velocity
	float vel;
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
