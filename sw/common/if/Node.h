/*
* ______________________       __________                                                    ______  
* ___  __ \_  ___/__    |      ___  ____/____________ _______ ___________      _________________  /__
* __  / / /____ \__  /| |      __  /_   __  ___/  __ `/_  __ `__ \  _ \_ | /| / /  __ \_  ___/_  //_/
* _  /_/ /____/ /_  ___ |      _  __/   _  /   / /_/ /_  / / / / /  __/_ |/ |/ // /_/ /  /   _  ,<   
* /_____/ /____/ /_/  |_|      /_/      /_/    \__,_/ /_/ /_/ /_/\___/____/|__/ \____//_/    /_/|_|  
*
*/

#ifndef __DSA_NODE_H__
#define __DSA_NODE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

struct Node {
	struct Node	*next;
	struct Node	*prev;

	/* This void pointer allows users to freely store any type of data they want */
	void		*data;
};

struct Node * node_constructor(void *data);
void node_destructor(struct Node **node, void (*free_func)(void *node));

#ifdef __cplusplus
}
#endif

#endif // __DSA_NODE_H__