#ifndef QUEUE_LOADED
#endif
#define QUEUE_LOADED
typedef struct q_node{
	void *data;
	struct q_node *next;
}q_node_t;

typedef struct queue{
	q_node_t *begin;
	q_node_t *end;
	int size;
}queue_t;


// criar queue
void create(queue_t *q);
int empty(queue_t *q);

// inserir node na queue
void insert(queue_t *q, void *p);
// delete
void* delete(queue_t *q);
void clean(queue_t *q);
int length(queue_t *q);