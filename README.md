# Bibliotecas para PPP
No âmbito da disciplina de Príncipios de Programação Procedimental, fizeram-se estas bibliotecas, em C, para permitir o uso generalizado das estruturas de dados dadas na disciplina. Como a biblioteca partilhada pelos professores necessitava de uma constante alteração para cada exercício da folha de problemas 4 e tenho preguiça de as alterar, fiz estas bibliotecas (baseadas no código dado nas aulas) que funcionam com qualquer tipo de dados. De uma forma muito simples divide-se a estrutra do código da seguinte forma: Esqueleto da estrutura -> nós -> dados

# NOTA IMPORTANTE!!
NÃO INCLUIR TODAS AS BIBLIOTECAS no mesmo ficheiro

# Funcionamento
O funcionamento das bibliotecas segue o seguinte paradigma:
 1. Incluir o ficheiro pretendido
 2. Declarar a variavél como pretender (como fixa ou como dinâmica, se bem que é melhor se for fixa exemplo: `queue_t q;`)
 3. inicializar/criar a variável. Este passo é crucial para evitar comportamentos indefinidos dos ponteiros
 4. usar as funções :)



# Compilação
Para compilar os ficheiros é necessário incluir todas as bibliotecas usadas. Exemplo de compilação com o ficheiro de exemplo na queue.h:
```bash
gcc test.c queue.c
```
Isto no caso de o ficheiro test.c estar na mesma pasta que o queue.h

## Para gente com pouca paciência
Como é óbvio, fiz um Makefile para compilar qualquer ficheiro que inclua estas bibliotecas. Para isto é necessário a seguinte estrutura de ficheiros:
```
(PPP)
|
| -------> Makefile
| -------> libs/ (este repositório, atenção para copiar o Makefile deste repo)
| -------> pasta_exemplo
|          |
|          |----> test.c
| -------> file.c
```
### Compilação com o Makefile
Primeiro vê o que está no Makefile e percebe o que lá está. Depois copia o Makefile para que fique idêntico à estrutura de exemplo. Move-se o ficheiro Makefile para a pasta acima:
```bash
mv ./Makefile ../
```
A pasta PPP puderá ter as pastas e ficheiros que quiserem. Para compilar basta usar o make da seguite forma:
```bash
make compile FILE=pasta_exemplo/test.c
```
## Atenção!
A compilação com o make precisa de estar na pasta raíz do diretório. VER ESTRUTURA DE FICHEIROS EXEMPLO!



# Bibliotecas
De momento as bibliotecas implementadas são as seguintes:
 - stack.h (biblioteca de pilhas)
 - queue.h (biblioteca de filas)
Em baixo está a explicação do uso das bibliotecas assim com pequenos exemplos.
## Filas (queue.h)
Aqui está um exemplo de uso da queue.h. Este programa cria duas estruturas aleatórias e completamente à toa, o que mostra como a queue funciona com qualquer tipo de dados introduzidos para os nós. Aqui apenas é preciso ter atenção ao delete(&q) que devolve um pointer e não uma estrutura de dados!
 NOTA: o delete devolve um pointer e é preciso fazer casting do tipo para a dada estrutura!!!!
```c
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

struct outraCoisa{
    int coisoNoutraStruct;
    float numero;
};

struct UmaEstrutura{
    int coiso;
    char string[20];
    struct outraCoisa elementosNovos;
};

int main(){
    queue_t q; // alocar a q
    create(&q); // inicializar a fila
    struct UmaEstrutura el;
    el.coiso = 10;
    el.elementosNovos = {20, 2304.12};
    insert(&q, &el); // inserir o o elemento na queue q
    printf("Coiso do el: %d\n", el.coiso);
    struct UmaEstrutura *ptr = (UmaEstrutura *) delete(&q);
    el = *ptr;
    printf("Coiso mas vindo da queue: %d\n", el.coiso);
    return 0;
}
```

## Pilhas (stack.h)
Ainda está a ser aprefeiçoada.
Já vai...

# Desenvolvedores
Na eventualidade de encontrarem um bug ou considerarem que existe uma melhor forma de fazer alguma coisa, então criem um issue e informem.<br>Obrigado ;)
