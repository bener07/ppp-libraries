#include <stddef.h>
#ifndef STACK_LOADED
#endif
#define STACK_LOADED

/*
  As stacks, ou pilhas, é o equivalente a uma pilha de livros. Os livros são adicionados uns em cima dos outros.
  A única referência que temos será o próximo livro por baixo, ou seja, o próximo elemento é o próximo livro a ser visto.
  Node <-- S
   |
   |
  Node
   |
   |
  Node
   |
   |
  Node (o primeiro elemento colocado, ou seja, o último elemento)
   |
   |
  NULL
*/
typedef	struct s_node{
	void *data;
	struct s_node *next;
}s_node_t;

typedef struct stack{
	s_node_t *current;
}stack_t;

s_node_t **create_stack();
int empty_stack(s_node_t **current);
void *pop_stack(s_node_t **current);
void push_to_stack(s_node_t **current, void *data, size_t size);
void clean_stack(s_node_t **current);
void showStack(s_node_t **current);
void *getLastNode(s_node_t **stack);
