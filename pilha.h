#ifndef PILHA_H_
#define PILHA_H_

typedef struct tarefa {
  double l;
  double r;
  double area_maior;
} tarefa_t;

typedef struct pilha {
  tarefa_t *colecao;
  int indice;
  int tam;
  int cap;
} pilha_t;

void p_init(pilha_t **);
void p_cheia(pilha_t **);
int p_vazia(pilha_t **);
void p_insere(pilha_t **, tarefa_t);
tarefa_t p_retira(pilha_t **);
void p_destroi(pilha_t **);

#endif
