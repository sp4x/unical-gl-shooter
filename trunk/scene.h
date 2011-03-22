
#ifndef SCENE_H
#define SCENE_H

#include "object.h"

#define FREE_SPACE '0'
#define WALL '1'
#define TURRET 't'
#define CUBE 'c'
#define WINDOW 'w'
#define LIGHT 'l'

#define CELLSIZE 2

/** struct that represents the scene 
 */
typedef struct scene_t {
	/** draw the scene */
	void (*display) (void);
	/** update the scene */
	void (*update) (void);
	/** add an object to the scene */
	void (*add) (object_t *obj);
	/** remove an object from the scene */
	void (*remove) (object_t *obj);
	
} scene_t;

/** externalized instance of scene */
extern scene_t *scene;

/** load the scene from file */
void loadScene (char *file);
/** clean the scene and free memory */
void clean (void);

#endif
