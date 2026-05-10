typedef struct list_node{
        struct list_node **next;        // Array de ponteiros para ordenação dinâmica
        void *data;                                     // ponteiro genérico para qualquer estrutura
}list_node_t;

typedef struct list{
        list_node_t **first;            // primeiros ponteiros
        list_node_t *last_searched;     // cache para ajudar na pesquisa
        size_t orderingPointers;        // número de ponteiros usados para a ordenação
        size_t size;                            // tamanho da lista
}list_t;


list_t *list_create(int orderingPointers); // devolve o ponteiro para a lista

int list_empty(list_t *l);

list_node_t *list_search(
		list_t *l,
		void *key,
		int (*condition)(void *, void*),
		int orderPointer
); // procura na lista p

void list_insert(
		list_t *l,
		void *data,
		int (*condition)(void *, void*),
		void *key
); // recebe os dados para colocar no node e a lista

void list_sort(
		list_t *l,
		int (*higher)(void *, void *),
		int orderer
);

void *list_delete(list_t *l, void *key, int (*condition)(void *, void*), int orderPointer);
void list_sort(list_t *l, int (*condition)(void *, void*), int orderPointer);
void list_print(list_t *l, void (*printFunction)(void *), int orderPointer);

void freeList(list_t *l);
