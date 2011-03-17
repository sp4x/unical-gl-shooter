#include <GL/glut.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#include "object.h"

#define RAD_TO_DEG 180.0/M_PI
#define DEG_TO_RAD M_PI/180.0

/* true when player dies */
GLboolean gameover;

/* current screen resolution */
char resolution[30];
int width, height;

/* true if you want to show objects bounds */
GLboolean showbounds;

/* global timer */
extern struct timeval timer;

/* game over... */
void game_over (void);

/* update the global timer */
void update_timer (void);

/* returns the current time (in seconds) */
double get_time (void);

/* return a string identifying the type of the object o */
char *objtype (object_t *o);
