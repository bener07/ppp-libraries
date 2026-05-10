#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include "stack.h"
#include "mem.h"

s_node_t **create_stack(){
	s_node_t **stack = malloc(sizeof(s_node_t**));
	*stack = NULL;
	return stack;
}

int empty_stack(s_node_t **current){
	return *current == NULL;
}

void *pop_stack(s_node_t **current){
	if(empty_stack(current)){
		printf("Stack vazia\n");
		return NULL;
	}
	s_node_t *node = *current;
	void *data = node->data;
	*current = node->next;
	free(node);
	return data;
}

void push_to_stack(s_node_t **current, void *data, size_t size){
	s_node_t *node = malloc(sizeof(s_node_t));
	if(node==NULL)return;
	node->data = storeData(data, size);
//	if(!empty_stack(current)){
	node->next = *current; // seria o mesmo que stack->current
//	}
	*current = node;
}

void clean_stack(s_node_t **current){
	s_node_t *node;
	while(!empty_stack(current)){
		node = *current;
		*current = node->next;
		free(node->data);
		free(node);
	}
}

void showStack(s_node_t **stack){
	int i = 0;
	for(s_node_t *ptr = *stack; ptr != NULL; ptr = ptr->next, i++){
		printf("%d: %d \n", i,*((int*) ptr->data) );
	}
	printf("\n");
}
void *getLastNode(s_node_t **stack){
	s_node_t *ptr;
	for(ptr = *stack; ptr->next != NULL; ptr=ptr->next);
	return ptr;
}
