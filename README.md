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

# Bibliotecas
De momento as bibliotecas implementadas são as seguintes:
 - stack.h (biblioteca de pilhas)
 - queue.h (biblioteca de filas)
Em baixo está a explicação do uso das bibliotecas assim com pequenos exemplos.
## Filas (queue.h)

```c
```

## Pilhas (stack.h)
