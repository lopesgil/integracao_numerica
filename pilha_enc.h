#ifndef PILHA_ENC_H_
#define PILHA_ENC_H_

typedef struct tarefa {
  double l;
  double r;
  double area_maior;
} tarefa_t;

typedef struct no {
  tarefa_t t;
  struct no *proximo;
} no_t;

typedef struct pilha {
  no_t *topo;
  int tamanho;
} pilha_t;

void p_init(pilha_t **);
int p_vazia(pilha_t *);
no_t *novo_no(tarefa_t);
void p_insere(pilha_t *, tarefa_t);
tarefa_t p_retira(pilha_t *);
void p_destroi(pilha_t *);

#endif
