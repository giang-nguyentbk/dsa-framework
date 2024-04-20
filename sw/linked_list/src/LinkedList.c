#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "Node.h"

#ifdef ONE_HEADER_LIB
#include "dsa.h"
#else
#include "LinkedList.h"
#endif

/* PROTOTYPE DECLARATION */
size_t ll_size(struct LinkedList *linkedlist);
bool ll_empty(struct LinkedList *linkedlist);
void ll_clear(struct LinkedList *linkedlist);
void ll_insert(struct LinkedList *linkedlist, int index, void *data);
void ll_erase(struct LinkedList *linkedlist, int index);
void * ll_at(struct LinkedList *linkedlist, int index);
void ll_push_back(struct LinkedList *linkedlist, void *data);
void ll_pop_back(struct LinkedList *linkedlist);
void ll_swap(struct LinkedList *linkedlist, struct LinkedList *other_linkedlist);
void ll_sort(struct LinkedList *linkedlist, int (*compare_func)(void *pa, void *pb));
void ll_reverse(struct LinkedList *linkedlist);

/* HELPER FUNCTION */
static void llh_swap_node(struct LinkedList *linkedlist, int index1, int index2);

struct LinkedList *linked_list_constructor(void (*free_node_data_func)(void *node_data))
{
	struct LinkedList *ll = (struct LinkedList *)malloc(sizeof(struct LinkedList));
	if(ll == NULL)
	{
		printf("ERROR: linked_list_constructor - Failed to malloc LinkedList!");
		return NULL;
	}

	ll->head 		= NULL;
	ll->nr_elements		= 0;
	ll->free_func		= free_node_data_func;
	ll->size		= ll_size;
	ll->empty		= ll_empty;
	ll->clear		= ll_clear;
	ll->insert		= ll_insert;
	ll->erase		= ll_erase;
	ll->at			= ll_at;
	ll->push_back		= ll_push_back;
	ll->pop_back		= ll_pop_back;
	ll->swap		= ll_swap;
	ll->sort		= ll_sort;
	ll->reverse		= ll_reverse;

	return ll;
}


void linked_list_destructor(struct LinkedList **linkedlist)
{
	if(linkedlist == NULL || *linkedlist == NULL)
	{
		printf("ERROR: linked_list_destructor - Free a NULL pointer!");
		return;
	}

	(*linkedlist)->clear(*linkedlist);
	free(*linkedlist);
	*linkedlist = NULL;
}



size_t ll_size(struct LinkedList *linkedlist)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_size - Given a NULL LinkedList!");
		return 0;
	}

	return linkedlist->nr_elements;
}

bool ll_empty(struct LinkedList *linkedlist)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_empty - Given a NULL LinkedList!");
		return false;
	}

	return linkedlist->nr_elements ? false : true;
}

void ll_clear(struct LinkedList *linkedlist)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_clear - Given a NULL LinkedList!");
		return;
	}

	struct Node *iter = linkedlist->head;
	for(; iter != NULL;)
	{
		struct Node *next = iter->next;
		node_destructor(&iter, linkedlist->free_func);
		iter = next;
		linkedlist->nr_elements--;
	}

	linkedlist->head = NULL;
}

void ll_insert(struct LinkedList *linkedlist, int index, void *data)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_insert - Given a NULL LinkedList!");
		return;
	} else if(index < 0 || index >= linkedlist->nr_elements)
	{
		printf("ERROR: ll_insert - Given index out of bound!");
		return;
	} else if(data == NULL)
	{
		printf("ERROR: ll_insert - Given data is NULL!");
		return;
	}

	struct Node *new_node = node_constructor(data);

	/* Handle special cases */
	if(index == 0)
	{
		new_node->next = linkedlist->head;
		linkedlist->head->prev = new_node;
		linkedlist->head = new_node;
		linkedlist->nr_elements++;
		return;
	}

	struct Node *iter = linkedlist->head;
	int i = 0;
	for(; i < linkedlist->nr_elements; i++)
	{
		if(index == i)
		{
			/* Because we will insert a new node right before index 
			*  We have to save the prev node, then concatenate the new node to the start of remaining chain starting with current node at index */
			struct Node *prev_node = iter->prev;

			new_node->next = iter;
			iter->prev = new_node;

			prev_node->next = new_node;
			new_node->prev = prev_node;
			linkedlist->nr_elements++;
			return;
		}

		iter = iter->next;
	}

	if(i == linkedlist->nr_elements)
	{
		node_destructor(&new_node, linkedlist->free_func);
	}
}

void ll_erase(struct LinkedList *linkedlist, int index)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_erase - Given a NULL LinkedList!");
		return;
	} else if(index < 0 || index >= linkedlist->nr_elements)
	{
		printf("ERROR: ll_erase - Given index out of bound!");
		return;
	}

	/* Handle special cases */
	if(index == 0)
	{
		struct Node *tmp_node = linkedlist->head->next;
		node_destructor(&linkedlist->head, linkedlist->free_func);
		linkedlist->head = tmp_node;
		linkedlist->nr_elements--;
		return;
	}

	struct Node *iter = linkedlist->head;
	for(int i = 0; i < linkedlist->nr_elements; i++)
	{
		if(index == i)
		{
			iter->prev->next = iter->next;
			iter->next->prev = iter->prev;
			node_destructor(&iter, linkedlist->free_func);
			linkedlist->nr_elements--;
			return;
		}

		iter = iter->next;
	}
}

void * ll_at(struct LinkedList *linkedlist, int index)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_at - Given a NULL LinkedList!");
		return NULL;
	} else if(index < 0 || index >= linkedlist->nr_elements)
	{
		printf("ERROR: ll_at - Given index out of bound!");
		return NULL;
	}

	struct Node *iter = linkedlist->head;
	for(int i = 0; i < linkedlist->nr_elements; i++)
	{
		if(index == i)
		{
			return iter->data;
		}

		iter = iter->next;
	}

	return NULL;
}

void ll_push_back(struct LinkedList *linkedlist, void *data)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_push_back - Given a NULL LinkedList!");
		return;
	} else if(data == NULL)
	{
		printf("ERROR: ll_push_back - Given data is NULL!");
		return;
	}

	struct Node *new_node = node_constructor(data);

	/* Handle special cases */
	if(linkedlist->nr_elements == 0)
	{
		linkedlist->head = new_node;
		linkedlist->nr_elements++;
		return;
	}

	struct Node *iter = linkedlist->head;
	int i = 0;
	for(; i < linkedlist->nr_elements; i++)
	{
		if(i == linkedlist->nr_elements - 1)
		{
			iter->next = new_node;
			new_node->prev = iter;
			linkedlist->nr_elements++;
			return;
		}

		iter = iter->next;
	}

	if(i == linkedlist->nr_elements)
	{
		node_destructor(&new_node, linkedlist->free_func);
	}
}

void ll_pop_back(struct LinkedList *linkedlist)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_pop_back - Given a NULL LinkedList!");
		return;
	}

	linkedlist->erase(linkedlist, linkedlist->nr_elements - 1);
}

void ll_swap(struct LinkedList *linkedlist, struct LinkedList *other_linkedlist)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_swap - Given a NULL LinkedList!");
		return;
	} else if(other_linkedlist == NULL)
	{
		printf("ERROR: ll_swap - Given a NULL LinkedList!");
		return;
	}

	struct Node *head = linkedlist->head;
	linkedlist->head = other_linkedlist->head;
	other_linkedlist->head = head;
}

void ll_sort(struct LinkedList *linkedlist, int (*compare_func)(void *pa, void *pb))
{
	if(linkedlist == NULL)
	{
		printf("ERROR: ll_sort - Given a NULL LinkedList!");
		return;
	} else if(compare_func == NULL)
	{
		printf("ERROR: ll_sort - Given a NULL compare_func!");
		return;
	}


}

void ll_reverse(struct LinkedList *linkedlist);

static void llh_swap_node(struct LinkedList *linkedlist, int index1, int index2)
{
	if(linkedlist == NULL)
	{
		printf("ERROR: llh_swap_node - Given a NULL LinkedList!");
		return;
	} else if(index1 < 0 || index1 >= linkedlist->nr_elements)
	{
		printf("ERROR: llh_swap_node - Given index1 out of bound!");
		return;
	} else if(index2 < 0 || index2 >= linkedlist->nr_elements)
	{
		printf("ERROR: llh_swap_node - Given index2 out of bound!");
		return;
	} else if(index1 == index2)
	{
		return;
	}

	int smaller = index1 < index2 ? index1 : index2;
	int larger = index1 > index2 ? index1 : index2;

	struct Node *smaller_node, *tmp_node;

	struct Node *iter = linkedlist->head;
	for(int i = 0; i < linkedlist->nr_elements; i++)
	{
		if(i == smaller)
		{
			smaller_node = iter;
		} else if(i == larger)
		{
			tmp_node = smaller_node->prev;
			iter->prev->next = smaller_node;
			tmp_node->next = iter;
			tmp_node = smaller_node->next;
			iter->next->prev = smaller_node;
			iter->next = tmp_node;

			if(smaller == 0)
			{
				linkedlist->head = iter;
			}

			return;
		}

		iter = iter->next;
	}
}











