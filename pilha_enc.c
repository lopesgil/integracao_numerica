#include <stdlib.h>
#include "pilha_enc.h"

void p_init(pilha_t **p) {
  *p = (pilha_t *)malloc(sizeof(pilha_t));
  (*p)->topo = NULL;
  (*p)->tamanho = 0;
}

int p_vazia(pilha_t **p) { return !((*p)->tamanho); }

no_t *novo_no(tarefa_t t) {
  no_t *novo = (no_t *)malloc(sizeof(no_t));
  novo->proximo = NULL;
  novo->t = t;
  return novo;
}

void p_insere(pilha_t **p, tarefa_t t) {
  no_t *in = novo_no(t);
  in->proximo = (*p)->topo;
  (*p)->topo = in;
  (*p)->tamanho++;
}

tarefa_t p_retira(pilha_t **p) {
  tarefa_t t;
  if (p_vazia(p)) {
    exit(-1);
  }
  no_t *temp = (*p)->topo;
  (*p)->topo = (*p)->topo->proximo;
  t = temp->t;
  free(temp);
  (*p)->tamanho--;
  return t;
}
