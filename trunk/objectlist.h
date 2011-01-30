
#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include "object.h"



struct object_list_iterator {
	object_t *value;
	struct object_list_iterator *next;
};
typedef struct object_list_iterator object_list_iterator;

struct object_list_t {
	object_list_iterator *first;
	
	void (*append)(object_t *elem);
	void (*delete)(object_t *obj);
	void (*clear)();
};
typedef struct object_list_t object_list_t;
extern object_list_t *object_list;

void createObjectList();

#endif
