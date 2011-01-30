
#include <stdlib.h>
#include "objectlist.h"
#include <stdio.h>

object_list_t *object_list;

void append(object_t *elem) {
	printf ("appending a %s\n", (elem->type == TYPE_WALL) ? "wall" : "bullet");

	object_list_iterator *tmp = malloc (sizeof(object_list_iterator));
	tmp->value =elem;
	tmp->next = object_list->first;
	object_list->first = tmp;
}

void clear() {
	object_list_iterator *current = object_list->first;
	if (current != NULL) {
		free(current->value);
		object_list_iterator *next = current->next;
		while (next != NULL) {
			free(current);
			current = next;
			free(current->value);
			next = current->next;
		}
	}
}

void delete (object_t *obj)
{
	object_list_iterator *curr = object_list->first, *prev = NULL;
	while (curr != NULL)
	{
		if (curr->value == obj)
		{
			if (prev == NULL) // found obj in the head
				object_list->first = curr->next;
			else
				prev->next = curr->next;

			free (curr->value);
			free (curr);
			return;
		}

		prev = curr;
		curr = curr->next;
	}
}

void createObjectList() {
	object_list = malloc(sizeof(object_list_t));
	object_list->first = NULL;
	//~ object_list->first = malloc(sizeof(object_list_iterator));
	//~ object_list->first->value = NULL;
	//~ object_list->first->next = NULL;
	//~ object_list->last = object_list->first;
	
	object_list->append = append;
	object_list->delete = delete;
	object_list->clear = clear;
}




