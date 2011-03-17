
#ifndef OBJECTLIST_H
#define OBJECTLIST_H

#include "object.h"



typedef struct object_list_iterator {
	object_t *value;
	struct object_list_iterator *next;
} object_list_iterator;

//~ typedef struct object_list_t {
	//~ object_list_iterator *first;
	//~ object_list_iterator *last;
	//~ 
	//~ void (*append) (struct object_list_t *this, object_t *elem);
	//~ void (*delete) (struct object_list_t *this, object_t *obj);
	//~ void (*clear) (struct object_list_t *this);
//~ } object_list_t;

typedef struct object_list_t {
	object_list_iterator *iterator;
} object_list_t;

object_list_t *newObjectList();
void listAppend (object_list_t *this, object_t *elem);
void listDelete (object_list_t *this, object_t *elem);
void listClear(object_list_t *this);

#endif
