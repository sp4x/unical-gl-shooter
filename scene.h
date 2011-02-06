
#ifndef SCENE_H
#define SCENE_H

#include "object.h"

#define FREE_SPACE '0'
#define WALL '1'


typedef struct scene_t {
	
	void (*display) (void);
	object_t * (*checkCollisions) (float, float, float, float *, float *, float *);
	
} scene_t;

extern scene_t *scene;

void loadScene (char *file);
void clean (void);

#endif
