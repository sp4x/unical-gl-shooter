
#ifndef SCENE_H
#define SCENE_H

#include "object.h"

#define FREE_SPACE '0'
#define WALL '1'
#define TURRET 't'
#define CUBE 'c'
#define WINDOW 'w'

#define CELLSIZE 2



typedef struct scene_t {
	
	void (*display) (void);
	void (*update) (void);
	void (*add) (object_t *obj);
	void (*remove) (object_t *obj);
	
} scene_t;

extern scene_t *scene;

void loadScene (char *file);
void clean (void);

#endif
