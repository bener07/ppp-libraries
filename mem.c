#include <string.h>
#include <stdlib.h>
#include "mem.h"

void *storeData(void *ptr, size_t size){
	if(ptr == NULL) return NULL;

	void *block = malloc(size);
	if(block == NULL) return NULL;

	// copia o conteúdo de ptr para o bloco de memória
	memcpy(block, ptr, size);

	return block;
}
