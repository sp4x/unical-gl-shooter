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


#include <stdlib.h>
#include "objectlist.h"
#include <stdio.h>


void listAppend (object_list_t *list, object_t *elem)
{
	object_list_iterator *tmp = malloc (sizeof(object_list_iterator));
	tmp->value = elem;
	tmp->next = list->iterator;
	list->iterator = tmp;
}

void listClear (object_list_t *list, int clear_elems)
{
	object_list_iterator *current = list->iterator;
	if (current != NULL) {
		if (clear_elems)
			free(current->value);
		object_list_iterator *next = current->next;
		while (next != NULL) {
			free(current);
			current = next;
			if (clear_elems)
				free(current->value);
			next = current->next;
		}
	}
}

void listDelete (object_list_t *list, object_t *obj)
{
	object_list_iterator *curr = list->iterator;
	object_list_iterator *prev = NULL;
	while (curr != NULL)
	{
		if (curr->value == obj)
		{
			if (prev == NULL) // found obj in the head 
				list->iterator = curr->next;
			else
				prev->next = curr->next;

			free (curr);
			return;
		}

		prev = curr;
		curr = curr->next;
	}
}

object_list_t *newObjectList()
{
	object_list_t *list = malloc(sizeof(object_list_t));
	list->iterator = NULL;
	return list;
}




