#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "Node.h"

struct Node * node_constructor(void *data)
{
	struct Node *node = (struct Node *)malloc(sizeof(struct Node));
	if(node == NULL)
	{
		printf("ERROR: node_constructor - Failed to malloc Node!");
		return NULL;
	}

	node->next	= NULL;
	node->prev	= NULL;
	node->data	= data;

	return node;
}

void node_destructor(struct Node **node, void (*free_func)(void *node))
{
	if(node == NULL || *node == NULL)
	{
		printf("ERROR: node_destructor - Free a NULL pointer!");
	}
	
	free_func((*node)->data);
	free(*node);
	*node = NULL;
}