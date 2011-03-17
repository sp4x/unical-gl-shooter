
#ifndef SCENE_H
#define SCENE_H

#include "object.h"

#define FREE_SPACE '0'
#define WALL '1'
#define TURRET 't'
#define CUBE 'c'

#define CELLSIZE 2

typedef enum { QUEUE_OPAQUE, QUEUE_TRANSPARENT } RenderQueueMode;

typedef struct scene_t {
	
	void (*display) (void);
	void (*update) (void);
	void (*add) (object_t *obj, RenderQueueMode mode);
	
} scene_t;

extern scene_t *scene;

void loadScene (char *file);
void clean (void);

#endif
