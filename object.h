
#ifndef OBJECT_H
#define OBJECT_H

#define TYPE_WALL 0
#define TYPE_BULLET 1
#define TYPE_TURRET 2
#define TYPE_FLOOR 3
#define TYPE_TOP 4

#define CELLSIZE 5
#define WALL_HEIGHT 10

typedef struct object_t {
	
	float pos_x, pos_y, pos_z; 		/* position */
	float 	min_x, max_x, 
			min_y, max_y, 
			min_z, max_z; 			/* bounds */

	float rot_x, rot_y; 			/* direction */
	float vel; 						/* movement's vel */
	int type; 						/* obj type 	*/
	int energy; 					/* current energy */

	/** draw this object */
	void (*display) (struct object_t *this);
	/** what to do on collision? */
	void (*onCollision) (struct object_t *this);
	
} object_t;

int hasCollision (object_t *obj, float x, float z, float y, float *available_x, float *available_y, float *available_z);

/** create functions */
object_t *newWall (float min_x, float max_x, float min_z, float max_z);
object_t *newBullet (float pos_x, float pos_y, float pos_z, float rot_x, float rot_y);
object_t *newFloor (float max_x, float max_y, float max_z);
object_t *newTop (float max_x, float max_y, float max_z);

#endif
