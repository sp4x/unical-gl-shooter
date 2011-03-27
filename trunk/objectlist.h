/**
 * Copyright (C) 2011 Vincenzo Pirrone, Salvatore Loria
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


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
void queueAdd (object_list_t *list, object_t *elem);
void listDelete (object_list_t *this, object_t *elem);
void listClear(object_list_t *this, int clear_elems);

#endif
