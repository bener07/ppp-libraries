# Variáveis de configuração
CC = gcc
# O -I./libs permite que faças #include "queue.h" em vez de "libs/queue.h"
CFLAGS = -I./libs -Wall -lm -g

# 1. Procura automaticamente todos os ficheiros .c na pasta libs
LIB_SRCS = $(wildcard libs/*.c)

# 2. Transforma a lista de .c em .o (ex: libs/queue.c -> libs/queue.o)
LIB_OBJS = $(LIB_SRCS:.c=.o)

.PHONY: compile clean

# Alvo principal
compile: $(LIB_OBJS)
	@if [ -z "$(FILE)" ]; then \
		echo "Erro: Indica o ficheiro. Ex: make FILE=teste.c"; \
	else \
		$(CC) $(CFLAGS) "$(FILE)" $(LIB_OBJS); \
		echo "--- Compilação de $(FILE) concluída ---"; \
		echo "--- A executar o programa ---"; \
		./a.out; \
	fi

# 3. Regra Genérica: Compila CADA .c da pasta libs para o seu próprio .o
# Só recompila o ficheiro que foi alterado!
libs/%.o: libs/%.c libs/%.h
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "Objeto atualizado: $@"

clean:
	rm -f libs/*.o a.out

