
#include <stdio.h>
#include "object.h"

#define COLLISION_GAP 0.1


int hasCollision(object_t *obj, float x, float y, float z, float *available_x, float *available_y, float *available_z) {
	if (x > obj->min_x && x < obj->max_x 
		&& y > obj->min_y && y < obj->max_y
		&& z > obj->min_z && z < obj->max_z) { //(x,y,z) in object bounds
		
		
		if (available_x == NULL || available_y == NULL || available_z == NULL)
			return 1;
			
		float coords[] = { x, y, z };
		float sides[] = { obj->min_x, obj->max_x, obj->min_y, obj->max_y,  obj->min_z, obj->max_z };
		float *available_coords[] = { available_x, available_y, available_z };
		int coord_index = 0, side_index = 0;
		for (coord_index = 0; coord_index < 3; coord_index++) {
			float min_side_gap = coords[coord_index] - sides[side_index]; //c - min_side
			float max_side_gap = sides[side_index+1] - coords[coord_index]; //max_side -c
			
			if ( min_side_gap > 0 && min_side_gap <= COLLISION_GAP) {
				*( available_coords[coord_index] ) = sides[side_index];
			}
			else if ( max_side_gap > 0 && max_side_gap <= COLLISION_GAP) {
				*( available_coords[coord_index] ) = sides[side_index+1];
			}
			
			side_index += 2;
		}
		return 1;
	}
	return 0;
}



