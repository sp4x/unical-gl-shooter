#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "camera.h"
#include "objectlist.h"
#include "object.h"
#include "input.h"

// mouse position in previous frame
float mouse_x = 0, mouse_y = 0;

// true if running
int speed = 0;

int first_time = 1;

int key_state[256];
int special_state[256];
int mouse_button_state[5];

clock_t prev = 0, curr;

enum
{
	MOUSE_LEFT_BUTTON = 0,
	MOUSE_MIDDLE_BUTTON = 1,
	MOUSE_RIGHT_BUTTON = 2,
	MOUSE_SCROLL_UP = 3,
	MOUSE_SCROLL_DOWN = 4
};

void mouse_func (int button, int state, int x, int y)
{
	mouse_button_state[button] = (state == GLUT_DOWN) ? DOWN : UP;
}

void mouse_motion (int x, int y)
{
	if (first_time)
	{
		mouse_x = x;
		mouse_y = y;
		first_time = 0;
	}
	float x_diff = x - mouse_x;
	float y_diff = y - mouse_y;	

	cam->rot_x -= y_diff;
	if (cam->rot_x > 90)
		cam->rot_x = 90;
	if (cam->rot_x < -90)
		cam->rot_x = -90;
	cam->rot_y -= x_diff;
	if (cam->rot_y > 360 || cam->rot_y < -360)
		cam->rot_y = 0;

	mouse_x = x;
	mouse_y = y;
	
	if (x == 0)
	{
		glutWarpPointer (1278, y);
		mouse_x = 1278;
	}
	if (x == 1279)
	{
		glutWarpPointer (1, y);
		mouse_x = 1;
	}
	if (y == 0)
	{
		glutWarpPointer (x, 798);
		mouse_y = 798;
	}
	if (y == 799)
	{
		glutWarpPointer (x, 1);
		mouse_y = 1;
	}
}

void key_down (unsigned char key, int x, int y)
{
	// DEBUG: stampa la lista degli elementi
	if (key == 'k')
	{
		object_list_iterator *it = object_list->first;
		for (it; it != NULL; it = it->next)
		{	
			printf ("%s\n", (it->value->type == TYPE_WALL) ? "wall" : "bullet");
		}
	}
	key_state[key] = DOWN;
}

void key_up (unsigned char key, int x, int y)
{
	key_state[key] = UP;
}

void spec_key_down (int key, int x, int y)
{
	special_state[key] = DOWN;
}

void spec_key_up (int key, int x, int y)
{
	special_state[key] = UP;
}

void input_update (void)
{
	if (key_state['w'] == DOWN)
	{
		cam->move_forward();
	}
	if (key_state['s'] == DOWN)
	{
		cam->move_backward();
	}
	if (key_state['q'] == DOWN)
	{
		cam->rotate_left();
	}
	if (key_state['e'] == DOWN)
	{
		cam->rotate_right();
	}
	if (key_state['a'] == DOWN)
	{
		cam->strafe_left();
	}
	if (key_state['d'] == DOWN)
	{
		cam->strafe_right();
	}
	if (key_state[27] == DOWN)
	{
		glutLeaveGameMode();
		exit(0);
	}
	if (mouse_button_state[MOUSE_LEFT_BUTTON] == DOWN)
	{
		// spara un colpo ogni 0.1 secondi circa
		curr = clock();
		if (prev == 0)
			prev = clock();
		double diff = curr - prev;
		if (diff / CLOCKS_PER_SEC > 0.1) {
			object_t *bullet = newBullet (
				cam->pos_x, cam->pos_y, cam->pos_z, cam->rot_x, cam->rot_y);
			object_list->append (bullet);
			glutPostRedisplay ();
			prev = curr;
		}
	}
}
