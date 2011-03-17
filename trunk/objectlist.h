
#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include "object.h"



typedef struct object_list_iterator {
	object_t *value;
	struct object_list_iterator *next;
	
} object_list_iterator;

typedef struct object_list_t {
	object_list_iterator *iterator;
	
} object_list_t;

object_list_t *newObjectList();
void listAppend (object_list_t *this, object_t *elem);
void listDelete (object_list_t *this, object_t *elem);
void listClear(object_list_t *this);

#endif
