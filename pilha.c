#include <stdlib.h>
#include <stdio.h>
#include "pilha.h"

void p_init(pilha_t ** p) {
  *p = malloc(sizeof(pilha_t));
  (*p)->indice = 0;
  (*p)->colecao = (tarefa_t *) malloc(sizeof(tarefa_t) * 256);
  (*p)->cap = 256;
  (*p)->tam = 0;
}

void p_cheia(pilha_t *p) {
  tarefa_t *nova_colecao;
  p->cap = p->cap * 2;
  nova_colecao = (tarefa_t *) realloc(p->colecao, p->cap);
  p->colecao = nova_colecao;
}

int p_vazia(pilha_t *p) {
  return !(p->tam);
}

void p_insere(pilha_t *p, tarefa_t t) {
  if(p->tam == p->cap) p_cheia(p);
  p->colecao[p->indice].area_maior = t.area_maior;
  p->colecao[p->indice].l = t.l;
  p->colecao[p->indice].r = t.r;
  p->indice++;
  p->tam++;
}

tarefa_t p_retira(pilha_t *p) {
  tarefa_t ret;
  p->indice--;
  ret = p->colecao[p->indice];
  p->tam--;
  return ret;
}

void p_destroi(pilha_t *p) {
  free(p->colecao);
  free(p);
}
