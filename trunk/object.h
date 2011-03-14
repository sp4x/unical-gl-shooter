
#ifndef OBJECT_H
#define OBJECT_H

#define TYPE_WALL 0
#define TYPE_BULLET 1
#define TYPE_TURRET 2
#define TYPE_FLOOR 3
#define TYPE_TOP 4
#define TYPE_CHARACTER 5
#define TYPE_CUBE 6

#define WALL_HEIGHT 10

typedef struct object_t {
	
	float pos_x, pos_y, pos_z; 		/* position */
	float 	min_x, max_x, 
			min_y, max_y, 
			min_z, max_z; 			/* bounds */

	double rot_x, rot_y; 			/* direction */
	float vel; 						/* movement's vel */
	int type; 						/* obj type 	*/
	int energy; 					/* current energy */
	int score;						/* current score (just for player) */

	double curr_time, last_time;	/* useful for timer-dependent functions (e.g. shoot) */

	int owner_type; 				/* just for bullet: who shoot this bullet? */

	/** draw this object */
	void (*display) (struct object_t *this);
	/** what to do on collision with obj? */
	void (*onCollision) (struct object_t *this, struct object_t *obj);
	
	void (*update) (struct object_t *this);
	
} object_t;

int hasCollision (object_t *this, object_t *obj);

/** create functions */
object_t *newObject(float min_x, float min_y, float min_z);
object_t *newCharacter (int pos_x, int pos_y, int pos_z);
object_t *newWall (float min_x, float max_x, float min_z, float max_z);
object_t *newBullet (struct object_t *owner);
object_t *newFloor (float max_x, float max_y, float max_z);
object_t *newTop (float max_x, float max_y, float max_z);
object_t *newTurret (float min_x, float min_z);
object_t *newCube(float min_x, float min_z);
#endif
