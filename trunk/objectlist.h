
#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include "object.h"



struct object_list_iterator {
	object_t *value;
	struct object_list_iterator *next;
	struct object_list_iterator *prev;
};
typedef struct object_list_iterator object_list_iterator;

struct object_list_t {
	object_list_iterator *first;
	object_list_iterator *last;
	
	void (*append) (object_t *elem);
	void (*delete) (object_t *obj);
	void (*clear) (void);
};
typedef struct object_list_t object_list_t;
extern object_list_t *object_list;

void createObjectList();

#endif
