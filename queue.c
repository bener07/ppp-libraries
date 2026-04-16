#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

// criar queue
void create(queue_t *q){
	q->begin = NULL;
	q->end = NULL;
	q->size = 0;
}
int empty(queue_t *q){
	return q->begin == NULL;
}

// inserir node na queue
void insert(queue_t *q, void* p){
	q_node_t *node = malloc(sizeof(q_node_t));
	if(node == NULL)
		return;
	node->data = p;
	node->next = NULL;
	if(empty(q)){
		q->begin = node;
	}
	else{
		// inserir na fila
		q->end->next = node;
	}
	q->end = node;
}

// delete
void *delete(queue_t *q){
	if(empty(q))
		return NULL;
	q_node_t *node = q->begin;
	q->begin = node->next;
	void *p = node->data;
	if(empty(q))
		q->end = NULL;
	free(node);
	return p;
}

void clean(queue_t *q){
	q_node_t *node;
	while(!empty(q)){
		node = q->begin;
		q->begin = node->next;
		free(node);
	}
	q->end = NULL;
}

int length(queue_t *q){
	q_node_t *node = q->begin;
	int i=0;
	if(empty(q))
		return i;
	for(; node->next!=NULL; i++){
		printf("%p", node->next);
		node = node->next;
	}
	return i;
}