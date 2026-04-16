#include <stdlib.h>
#include "stack.h"

void create_stack(s_node_t **current){
	current = NULL;
}

int empty_stack(s_node_t **current){
	return (*current)->next == NULL;
}

void *pop_stack(s_node_t **current){
	if(empty(current)){
		return NULL;
	}
	void *data = (*current)->data;
	s_node_t *node = *current;
	*current = node->next;
	free(node);
	return data;
}

void push_stack(s_node_t **current, void *data){
	s_node_t *node = malloc(sizeof(s_node_t));
	if(node!=NULL){
		node->data = data;
		node->next = *current; // seria o mesmo que stack->current
 		**current = *node;
	}
}

void clean_stack(s_node_t **current){
	s_node_t *node;
	for(node = *current;!empty(current); *current = (*current)->next)
		free(node);
}