/*
* ______________________       __________                                                    ______  
* ___  __ \_  ___/__    |      ___  ____/____________ _______ ___________      _________________  /__
* __  / / /____ \__  /| |      __  /_   __  ___/  __ `/_  __ `__ \  _ \_ | /| / /  __ \_  ___/_  //_/
* _  /_/ /____/ /_  ___ |      _  __/   _  /   / /_/ /_  / / / / /  __/_ |/ |/ // /_/ /  /   _  ,<   
* /_____/ /____/ /_/  |_|      /_/      /_/    \__,_/ /_/ /_/ /_/\___/____/|__/ \____//_/    /_/|_|  
*
*/

#ifndef __DSA_LINKED_LIST_H__
#define __DSA_LINKED_LIST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "Node.h"

struct LinkedList {
	struct Node	*head;
	size_t		nr_elements;
	void (*free_func)(void *node_data);

	/* Get idea from C++ STL data structures, below methods should be somehow similar */
	size_t (*size)(struct LinkedList *linkedlist);	// get size which is nr_elements
	bool (*empty)(struct LinkedList *linkedlist);	// check if LinkedList is empty
	void (*clear)(struct LinkedList *linkedlist);    // clear all elements and free data of all elements through freeFunc provided by users, if they don't want to free() anything, have to a do_nothing() function there
	void (*insert)(struct LinkedList *linkedlist, int index, void *data); // Insert an element right before at index position
	void (*erase)(struct LinkedList *linkedlist, int index); // Erase an element at index
	void * (*at)(struct LinkedList *linkedlist, int index); // Retrieve data of an element at index
	void (*push_back)(struct LinkedList *linkedlist, void *data); // Insert an element at the end of LinkedList
	void (*pop_back)(struct LinkedList *linkedlist); // Erase the last element
	void (*swap)(struct LinkedList *linkedlist, struct LinkedList *other_linkedlist); // swap the whole elements of two LinkedList
	void (*sort)(struct LinkedList *linkedlist, int (*compare_func)(void *pa, void *pb)); // Sort LinkedList according to user-defined compare function
	void (*reverse)(struct LinkedList *linkedlist); // Reverse LinkedList
};

struct LinkedList *linked_list_constructor(void (*free_node_data_func)(void *node_data));
void linked_list_destructor(struct LinkedList **linkedlist);


#ifdef __cplusplus
}
#endif

#endif // __DSA_LINKED_LIST_H__