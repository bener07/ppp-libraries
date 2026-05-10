#include "listas.h"
#include <stdio.h>
#include <stdlib.h>

list_t *list_create(int orderPointer){
	list_t *l = calloc(1,sizeof(list_t));
	if(l == NULL){
		return NULL;
	}
	l->orderingPointers = orderPointer;
	l->first = calloc(orderPointer, sizeof(list_node_t *));
	l->size = 0;
	return l;
}


void freeNode(list_t *l, list_node_t *ptr){
	free(ptr->next);
	free(ptr);
	l->size--;
	l->last_searched = NULL;
}
int alwaysCondition(void*, void*){
	return 1; // garantir que elimina sempre o primeiro elemento da lista
}

void detachNode(list_t *l, void *key, int (*condition)(void*, void*)){
	// apenas elimina a referência dos ponteiros
	// remover todas as referências do nó a eliminar
	list_node_t *ptr;
	for(int i = 0; i<l->orderingPointers; i++){
		ptr = list_search(l, key, condition, i);
		if(ptr == NULL){
			l->first[i] = l->first[i]->next[i];
		}else{
			if(ptr->next[i] != NULL)
				ptr->next[i] = ptr->next[i]->next[i];
			else
				ptr->next[i] = NULL;
		}
	}
}

// Em vez de pesquisar, percorre cada índice e remove o nó alvo diretamente
void detachNodeDirect(list_t *l, list_node_t *target) {
    for (int i = 0; i < l->orderingPointers; i++) {
        if (l->first[i] == target) {
            l->first[i] = target->next[i];
        } else {
            list_node_t *cur = l->first[i];
            while (cur != NULL && cur->next[i] != target) {
                cur = cur->next[i];
            }
            if (cur != NULL) {
                cur->next[i] = target->next[i];
            }
        }
    }
}

void clearList(list_t *l){
	while(!list_empty(l)){
		list_node_t *target = l->first[0];
        detachNodeDirect(l, target);
        freeNode(l, target);
	}
}
void freeList(list_t *l){
	if(!list_empty(l)){ // enquanto não estiver vazia então esvazia-a
		clearList(l);
	}
	free(l->first);		// dar free da lista de primeiros elementos
	free(l);			// libertar o endereço da lista
}
/*
*	list_delete ->	elimina um nó na lista. Trata das referências todas
*		Devolve: 	ponteiro de dados para o conteúdo alocado
*	Atenção!! A função condição necessita de ser uma que encontre o último nó.
*			Não usar funções que façam igualdades
*/
// assumindo que a função de condição é válida
void *list_delete(list_t *l, void *key, int (*condition)(void *, void*), int orderPointer){
	if(list_empty(l)) // lista vazia
		return NULL;

	list_node_t *ptr = list_search(l, key, condition, 0)->next[0]; // encontrar o elemento
	detachNode(l, key, condition); // Fazer o detach do nó na lista. Eliminar as referências

	void *data = ptr->data;
	freeNode(l, ptr);

	return data;
};

int list_empty(list_t *l){
	return l->first[0] == NULL;
}

list_node_t *list_search(list_t *l, void *key, int (*condition)(void*, void*), int orderPointer) {
    if (list_empty(l)) return (list_node_t *) -1;		// lista vazia

    if (!condition(l->first[orderPointer]->data, key)) {	// é o primeiro nó
        return NULL;
    }

    list_node_t *prev = l->first[orderPointer];
    
    if (l->last_searched != NULL && condition(l->last_searched->data, key)) {
        prev = l->last_searched;
    }

	list_node_t *cur = prev->next[orderPointer];
    while (cur != NULL && condition(cur->data, key)) {
        prev = cur;
        cur = cur->next[orderPointer];
    }
    l->last_searched = prev;
    return prev;
}

void list_insert(list_t *l, void *data, int (*condition)(void *, void*), void *key){
	list_node_t *node = malloc(sizeof(list_node_t));
	if(node == NULL) return;
	if(condition == NULL) condition = alwaysCondition;

	node->data = data;
	node->next = calloc(l->orderingPointers, sizeof(list_node_t *));
	// na eventualidade de problemas na alocação de memória
	if(node->next == NULL){
		free(node);
		return;
	}

	if(list_empty(l)){
		for(int i = 0; i<l->orderingPointers; i++)
			l->first[i] = node;
		l->size++;
		return;
	}

	// encontra o último nó onde a condição foi válida
	list_node_t *point_of_insertion = list_search(l, key, condition, 0);

	// caso a key seja o elemento mais pequeno
	if(point_of_insertion == NULL){
		node->next[0] = l->first[0];
		l->first[0] = node;
	}
	else{
		node->next[0] = point_of_insertion->next[0];
		point_of_insertion->next[0] = node;
	}

	l->size++;
}

void list_print(list_t *l, void (*printFunction)(void *), int orderPointer){
	list_node_t *node = l->first[orderPointer];
	while(node!=NULL){
		printFunction(node->data);
		node = node->next[orderPointer];
	}
	return;
}

void list_prepare_new_order(list_t *l, int orderPointer){
	list_node_t *cur = l->first[0];

	l->first[orderPointer] = cur;

	while(cur != NULL){
		cur->next[orderPointer] = cur->next[0];
		cur = cur->next[0];
	}
}

// aplicação de um insertion sort
void list_sort(list_t *l, int (*higher)(void *, void*), int orderPointer){
	if(l->size <= 1) {printf("Elementos Insuficientes!\n"); return;}

	list_prepare_new_order(l, orderPointer);

	list_node_t *cur = l->first[orderPointer];
	list_node_t *sorted_list = NULL; // a nova lista com uma ordenação diferente

	while (cur != NULL) {
        list_node_t *next_node = cur->next[orderPointer]; // Guardar o resto da lista

        // se a lista ordenada com uma nova chave for não ordenada
        // se o elemento da nova ordenação for maior que o atual
        if (sorted_list == NULL || higher(sorted_list->data, cur->data)) {
            cur->next[orderPointer] = sorted_list;
            sorted_list = cur;
        }
        else{
            list_node_t *p = sorted_list;
            // procura o próximo mais pequeno
            while (p->next[orderPointer] != NULL && !higher(p->next[orderPointer]->data, cur->data)) {
                p = p->next[orderPointer];
            }
            cur->next[orderPointer] = p->next[orderPointer];
            p->next[orderPointer] = cur;
        }

        cur = next_node;
    }
    l->first[orderPointer] = sorted_list;
}