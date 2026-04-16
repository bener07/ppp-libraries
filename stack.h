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
  void *(*create)(s_node_t **);
  int *(*empty)(s_node_t **);
}stack_t;

void create(s_node_t **current);
int empty(s_node_t **current);
void *pop(s_node_t **current);
void push(s_node_t **current);
void clean(s_node_t **current);