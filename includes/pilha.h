#ifndef PILHA_H_
#define PILHA_H_

/*
  Estrutura para representar as tarefas executadas pelas threads
  [l, r] são o intervalo cuja área será calculada
  area_maior é a área do retângulo maior calculada na iteração anterior
*/
typedef struct tarefa {
  double l;
  double r;
  double area_maior;
} tarefa_t;

/*
  Representação da pilha de tarefas usada na comunicação entre as threads
*/
typedef struct pilha {
  tarefa_t *colecao;
  int indice;
  int tam;
  int cap;
} pilha_t;

/*
  Funções para o uso da estrutura de pilha pelas threads
*/
void p_init(pilha_t **);
void p_cheia(pilha_t *);
int p_vazia(pilha_t *);
void p_insere(pilha_t *, tarefa_t);
tarefa_t p_retira(pilha_t *);
void p_destroi(pilha_t *);

#endif
