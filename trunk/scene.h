
#ifndef SCENE_H
#define SCENE_H

#include "object.h"

#define FREE_SPACE '0'
#define WALL '1'

#define CELLSIZE 5
#define HEIGHT 10

char *buffer;

struct scene_t {
	
	void (*display)();
	object_t * (*checkCollisions)(float, float, float, float *, float *, float *);
};

typedef struct scene_t scene_t;
extern scene_t *scene;



void loadScene(char *file);
void clean();


#endif
