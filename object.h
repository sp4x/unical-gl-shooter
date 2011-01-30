
#ifndef OBJECT_H
#define OBJECT_H

#define TYPE_WALL 0
#define TYPE_BULLET 1
#define TYPE_TURRET 2

struct object_t {
	
	float pos_x, pos_y, pos_z; //position
	float min_x, max_x, min_y, max_y, min_z, max_z; //bounds

	float rot_x, rot_y;
	float vel;
	int type;

	void (*display)();
};
typedef struct object_t object_t;

int hasCollision(object_t *obj, float x, float z, float y, float *available_x, float *available_y, float *available_z);



#endif
