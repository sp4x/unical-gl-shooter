#include <GL/glut.h>
#include <string.h>

/* true when player dies */
GLboolean gameover;

/* current screen resolution */
char resolution[30];
int width, height;

/* global timer */
extern struct timeval timer;

/* game over... */
void game_over (void);

/* update the global timer */
void update_timer (void);

/* returns the current time (in seconds) */
double get_time (void);
