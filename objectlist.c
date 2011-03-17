
#include <stdlib.h>
#include "objectlist.h"
#include <stdio.h>


void listAappend(object_list_t *list, object_t *elem) {
	object_list_iterator *tmp = malloc (sizeof(object_list_iterator));
	tmp->value = elem;
	tmp->next = list->iterator;
	list->iterator = tmp;
}

void listClear(object_list_t *list) {
	object_list_iterator *current = list->iterator;
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

			free (curr->value);
			free (curr);
			return;
		}

		prev = curr;
		curr = curr->next;
	}
}

object_list_t *newObjectList() {
	object_list_t *list = malloc(sizeof(object_list_t));
	//~ list->iterator = malloc(sizeof(object_list_iterator));
	//~ list->iterator->value = NULL;
	//~ list->iterator->next = NULL;
	list->iterator = NULL;
	//~ this->first = malloc(sizeof(object_list_iterator));
	//~ this->first->value = NULL;
	//~ this->first->next = NULL;
	//~ this->last = this->first;
	
	//~ this->append = append;
	//~ this->delete = delete;
	//~ this->clear = clear;
	
	return list;
}




