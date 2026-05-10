# Documentação — Biblioteca `listas`

Biblioteca de lista ligada com suporte a **múltiplas ordenações simultâneas**. Cada nó tem um array de ponteiros `next[]`, o que permite manter a mesma coleção de dados ordenada por critérios diferentes ao mesmo tempo, sem duplicar os dados.

---

## Índice

- [Estruturas de dados](#estruturas-de-dados)
- [API](#api)
  - [`list_create`](#list_create)
  - [`list_empty`](#list_empty)
  - [`list_search`](#list_search)
  - [`list_insert`](#list_insert)
  - [`list_delete`](#list_delete)
  - [`list_sort`](#list_sort)
  - [`list_print`](#list_print)
- [Funções internas](#funções-internas-não-exportadas)
- [Exemplos de uso](#exemplos-de-uso)
  - [Lista de inteiros](#setup-inicial--lista-de-inteiros-com-uma-ordenação)
  - [Duas ordenações simultâneas](#duas-ordenações-simultâneas--lista-de-structs-por-nome-e-por-idade)
  - [Apagar um elemento](#apagar-um-elemento)
  - [Procurar um nó](#procurar-um-nó)
- [Notas e armadilhas comuns](#notas-e-armadilhas-comuns)
- [Referências](#referências)

---

## Estruturas de dados

A biblioteca assenta em duas structs definidas em `estruturas.h`:

```c
// Nó da lista
typedef struct list_node_t {
    void *data;               // ponteiro para os dados do nó
    struct list_node_t **next; // array de ponteiros next (um por ordenação)
} list_node_t;

// Lista
typedef struct list_t {
    list_node_t **first;      // array de cabeças (uma por ordenação)
    list_node_t *last_searched; // cache do último nó visitado em list_search
    int orderingPointers;     // número de ordenações suportadas
    int size;                 // número de elementos
} list_t;
```

> O campo `last_searched` é uma otimização interna. Não o manipules diretamente.

---

## API

### `list_create`

```c
list_t *list_create(int orderingPointers);
```

Aloca e inicializa uma lista com suporte a `orderingPointers` ordenações.  
Devolve `NULL` se a alocação falhar.

**Parâmetros**
- `orderingPointers` — número de ordenações distintas que a lista vai suportar. Mínimo: `1`.

---

### `list_empty`

```c
int list_empty(list_t *l);
```

Devolve `1` se a lista estiver vazia, `0` caso contrário.  
Verifica apenas `l->first[0]`, por isso a lista de base (índice 0) é sempre a referência.

---

### `list_search`

```c
list_node_t *list_search(
    list_t *l,
    void *key,
    int (*condition)(void *, void *),
    int orderPointer
);
```

Percorre a lista pelo índice `orderPointer` e devolve o **nó imediatamente antes** do primeiro nó que faz `condition` devolver `0`. Ou seja, encontra o "ponto de inserção" para a `key`.

**Valores de retorno**
- `NULL` — a `key` seria inserida antes do primeiro elemento (é a menor)
- `(list_node_t *) -1` — lista vazia
- ponteiro válido — nó predecessor do ponto de corte

**Importante:** a `condition` deve devolver `1` enquanto o cursor deve continuar a avançar, e `0` quando encontra o nó onde parar. Não é uma função de igualdade — é uma função de ordem.

---

### `list_insert`

```c
void list_insert(
    list_t *l,
    void *data,
    int (*condition)(void *, void *),
    void *key
);
```

Insere `data` na lista na posição determinada por `condition` e `key`, usando o índice de ordenação `0`.  
Se `condition` for `NULL`, insere sempre no fim da lista (usa `alwaysCondition` internamente).

**Nota:** `list_insert` apenas atualiza o índice `0`. Para criar ordenações adicionais, usa `list_sort`.

---

### `list_delete`

```c
void *list_delete(
    list_t *l,
    void *key,
    int (*condition)(void *, void *),
    int orderPointer
);
```

Remove o nó que se segue ao ponto de corte encontrado por `list_search` no índice `orderPointer`.  
Devolve o ponteiro `data` do nó removido — **a responsabilidade de libertar esse bloco de memória é do chamador**.

Devolve `NULL` se a lista estiver vazia.

**Atenção:** tal como em `list_search`, a `condition` tem de ser uma função de ordem, não de igualdade. Para apagar um elemento específico, a `condition` deve devolver `1` para todos os elementos anteriores ao alvo.

---

### `list_sort`

```c
void list_sort(
    list_t *l,
    int (*higher)(void *, void *),
    int orderPointer
);
```

Ordena a lista pelo índice `orderPointer` usando **insertion sort**.  
Copia primeiro o índice `0` para `orderPointer` com `list_prepare_new_order`, e depois reordena apenas os ponteiros `next[orderPointer]` — os dados não são movidos.

**Parâmetros**
- `higher(a, b)` — devolve `1` se `a` for "maior" que `b` (a lista fica ordenada do menor para o maior)

Requer pelo menos 2 elementos. Se a lista tiver ≤ 1 elemento, imprime uma mensagem e regressa.

---

### `list_print`

```c
void list_print(
    list_t *l,
    void (*printFunction)(void *),
    int orderPointer
);
```

Percorre a lista pelo índice `orderPointer` e chama `printFunction` em cada nó.

---

## Funções internas (não exportadas)

| Função | O que faz |
|---|---|
| `freeNode` | Liberta o array `next` e o nó em si; decrementa `size` |
| `freeList` | Esvazia a lista e liberta toda a memória |
| `clearList` | Remove todos os nós chamando `list_delete` em loop |
| `detachNode` | Remove as referências a um nó em **todos** os índices de ordenação |
| `list_prepare_new_order` | Copia o índice `0` para um novo índice antes de ordenar |
| `alwaysCondition` | Condição que devolve sempre `1`; usada para apagar o primeiro elemento |

---

## Exemplos de uso

### Setup inicial — lista de inteiros com uma ordenação

```c
#include "listas.h"
#include <stdio.h>
#include <stdlib.h>

// condition: devolve 1 enquanto o elemento da lista for menor ou igual à key
int int_less_or_equal(void *node_data, void *key) {
    return *(int *)node_data <= *(int *)key;
}

void print_int(void *data) {
    printf("%d -> ", *(int *)data);
}

int main() {
    list_t *l = list_create(1); // 1 ordenação

    int values[] = {5, 2, 8, 1, 4};
    for (int i = 0; i < 5; i++) {
        int *v = malloc(sizeof(int));
        *v = values[i];
        list_insert(l, v, int_less_or_equal, v); // insere ordenado
    }

    list_print(l, print_int, 0); // 1 -> 2 -> 4 -> 5 -> 8 ->
    printf("NULL\n");

    freeList(l);
    return 0;
}
```

---

### Duas ordenações simultâneas — lista de structs por nome e por idade

```c
#include "listas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[64];
    int age;
} Person;

// ordenação por nome (alfabética)
int by_name(void *a, void *b) {
    return strcmp(((Person *)a)->name, ((Person *)b)->name) <= 0;
}

// higher por idade: devolve 1 se a->age > b->age (para list_sort)
int age_higher(void *a, void *b) {
    return ((Person *)a)->age > ((Person *)b)->age;
}

void print_person_name(void *data) {
    printf("%s -> ", ((Person *)data)->name);
}

void print_person_age(void *data) {
    printf("%d -> ", ((Person *)data)->age);
}

int main() {
    list_t *l = list_create(2); // 2 ordenações: índice 0 = nome, índice 1 = idade

    const char *names[] = {"Carlos", "Ana", "Bruno", "Diana"};
    int ages[]          = {30, 25, 35, 28};

    for (int i = 0; i < 4; i++) {
        Person *p = malloc(sizeof(Person));
        strncpy(p->name, names[i], 63);
        p->age = ages[i];
        list_insert(l, p, by_name, p); // insere ordenado por nome (índice 0)
    }

    printf("Por nome:  ");
    list_print(l, print_person_name, 0); // Ana -> Bruno -> Carlos -> Diana
    printf("NULL\n");

    list_sort(l, age_higher, 1); // cria ordenação por idade no índice 1

    printf("Por idade: ");
    list_print(l, print_person_age, 1); // 25 -> 28 -> 30 -> 35
    printf("NULL\n");

    freeList(l);
    return 0;
}
```

---

### Apagar um elemento

```c
// Condição: devolve 1 para todos os elementos com age < 30
// Assim, list_delete vai apagar o primeiro elemento com age >= 30
int age_before_30(void *node_data, void *key) {
    (void)key;
    return ((Person *)node_data)->age < 30;
}

// ...
int dummy = 0;
Person *removed = (Person *)list_delete(l, &dummy, age_before_30, 0);
if (removed) {
    printf("Removido: %s\n", removed->name);
    free(removed);
}
```

> **Lembra-te:** `list_delete` devolve o ponteiro `data`. Tens de chamar `free` sobre ele se a memória for tua.

---

### Procurar um nó

```c
// Verifica se existe alguém com age <= 28
int age_lte_28(void *node_data, void *key) {
    (void)key;
    return ((Person *)node_data)->age <= 28;
}

int dummy = 0;
list_node_t *result = list_search(l, &dummy, age_lte_28, 0);

if (result == (list_node_t *) -1) {
    printf("Lista vazia\n");
} else if (result == NULL) {
    printf("Elemento seria o primeiro\n");
} else {
    // result->next[0] é o nó alvo (se existir)
    if (result->next[0] != NULL)
        printf("Encontrado: %s\n", ((Person *)result->next[0]->data)->name);
}
```

---

## Notas e armadilhas comuns

**`list_search` devolve o predecessor, não o nó em si.**  
O nó que te interessa é sempre `result->next[orderPointer]`. Verifica `NULL` antes de aceder.

**`list_insert` só atualiza o índice 0.**  
Depois de inserções, as ordenações adicionais (índice ≥ 1) ficam desatualizadas. Chama `list_sort` novamente se precisares que estejam corretas.

**A `condition` em `list_search`/`list_insert`/`list_delete` não é uma função de igualdade.**  
Deve devolver `1` para avançar e `0` para parar. Se passares uma função de igualdade, o comportamento não é o esperado.

**`freeList` não é exportada no `.h`.**  
Se precisares de a usar fora da biblioteca, expõe-a no header ou chama `clearList` seguido de `free(l->first); free(l);` manualmente.

**`last_searched` é invalidado ao apagar nós.**  
A biblioteca faz `l->last_searched = NULL` em `freeNode`, por isso não há problema com ponteiros dangling desde que não guardes cópias externas do campo.

---

## Referências

### Listas ligadas e estruturas de dados

- [Linked List — Wikipedia](https://en.wikipedia.org/wiki/Linked_list) — visão geral de listas ligadas simples, duplas e circulares
- [Skip List — Wikipedia](https://en.wikipedia.org/wiki/Skip_list) — estrutura conceitualmente próxima: múltiplos ponteiros `next` por nó para acelerar pesquisa
- [Unrolled Linked List — Wikipedia](https://en.wikipedia.org/wiki/Unrolled_linked_list) — variante com melhor desempenho de cache

### Algoritmos de ordenação

- [Insertion Sort — Wikipedia](https://en.wikipedia.org/wiki/Insertion_sort) — algoritmo usado em `list_sort`; O(n²) no pior caso, eficiente para listas pequenas ou quase ordenadas
- [Sorting Algorithm Visualizer](https://visualgo.net/en/sorting) — animações de insertion sort e outros algoritmos

### Gestão de memória em C

- [malloc / calloc / free — cppreference](https://en.cppreference.com/w/c/memory) — referência das funções de alocação dinâmica usadas na biblioteca
- [Valgrind](https://valgrind.org/) — ferramenta para detetar memory leaks e acessos inválidos; útil para testar esta biblioteca
- [AddressSanitizer (ASan)](https://clang.llvm.org/docs/AddressSanitizer.html) — alternativa ao Valgrind integrada no compilador (`-fsanitize=address`)

### Ponteiros para funções em C

- [Function Pointers — cppreference](https://en.cppreference.com/w/c/language/pointer#Pointers_to_functions) — sintaxe e uso de ponteiros para funções como `int (*condition)(void *, void *)`
- [qsort — cppreference](https://en.cppreference.com/w/c/algorithm/qsort) — exemplo de API da stdlib que usa o mesmo padrão de função de comparação via ponteiro

### `void *` e genericidade em C

- [void pointer — cppreference](https://en.cppreference.com/w/c/language/pointer#Pointers_to_void) — comportamento de `void *`, conversões implícitas e limitações
- [Generic Data Structures in C — Ben Hoyt](https://benhoyt.com/writings/hash-table-in-c/) — artigo prático sobre estruturas de dados genéricas com `void *`

### Ferramentas de desenvolvimento

- [GCC — GNU Compiler Collection](https://gcc.gnu.org/) — compilador; usar `-Wall -Wextra -g` para detetar problemas em tempo de compilação
- [GDB — GNU Debugger](https://www.sourceware.org/gdb/) — debugger para inspecionar listas nó a nó em runtime
- [Clang Static Analyzer](https://clang-analyzer.llvm.org/) — análise estática para detetar use-after-free e null dereferences
