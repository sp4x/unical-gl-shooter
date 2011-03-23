#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "camera.h"
#include "scene.h"
#include "object.h"
#include "objectlist.h"
#include "input.h"
#include "util.h"

// mouse position in previous frame
double mouse_x = 0, mouse_y = 0;

int first_time = 1;

int key_state[256];
int special_state[256];
int mouse_button_state[5];

enum
{
	MOUSE_LEFT_BUTTON,
	MOUSE_MIDDLE_BUTTON,
	MOUSE_RIGHT_BUTTON,
	MOUSE_SCROLL_UP,
	MOUSE_SCROLL_DOWN
};

void mouse_func (int button, int state, int x, int y)
{
	mouse_button_state[button] = (state == GLUT_DOWN) ? DOWN : UP;

	// DEBUG: shoot a bullet at left button press 
	//~ if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
	//~ {
		//~ object_t *bullet = newBullet (cam->character);
		//~ object_list->append (bullet);
	//~ }
}

void mouse_motion (int x, int y)
{
	if (first_time)
	{
		mouse_x = x;
		mouse_y = y;
		first_time = 0;
	}
	double x_diff = x - mouse_x;
	double y_diff = y - mouse_y;	

	cam->character->rot_x -= y_diff;
	if (cam->character->rot_x > 90)
		cam->character->rot_x = 90;
	if (cam->character->rot_x < -90)
		cam->character->rot_x = -90;
	cam->character->rot_y -= x_diff;
	if (cam->character->rot_y > 360 || cam->character->rot_y < -360)
		cam->character->rot_y = 0;

	mouse_x = x;
	mouse_y = y;
	
	if (x == 0)
	{
		glutWarpPointer (width-2, y);
		mouse_x = width-2;
	}
	if (x == width-1)
	{
		glutWarpPointer (1, y);
		mouse_x = 1;
	}
	if (y == 0)
	{
		glutWarpPointer (x, height-2);
		mouse_y = height-2;
	}
	if (y == height-1)
	{
		glutWarpPointer (x, 1);
		mouse_y = 1;
	}
}

void key_down (unsigned char key, int x, int y)
{
	/* DEBUG: show objects bounds */
	if (key == 'b')
	{
		showbounds = !showbounds;
	}
	if (key == 'l')
	{
		if (glIsEnabled (GL_LIGHT6))
			glDisable (GL_LIGHT6);
		else
			glEnable (GL_LIGHT6);
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
		/* shoot a bullet each 0.05 sec */
		cam->character->curr_time = get_time();
		if (cam->character->curr_time - cam->character->last_time >= 0.05)
		{
			cam->character->last_time = get_time();
			object_t *bullet = newBullet (cam->character);
			bullet->vel = 0.7;
			scene->add (bullet);
		}
	}
}
