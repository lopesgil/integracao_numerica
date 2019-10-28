#include <stdlib.h>
#include <stdio.h>
#include "pilha.h"

// Inicialização do ponteiro da pilha, da coleção interna, e das variáveis de controle
void p_init(pilha_t ** p) {
  *p = malloc(sizeof(pilha_t));
  (*p)->indice = 0;
  (*p)->colecao = (tarefa_t *) malloc(sizeof(tarefa_t) * 256);
  (*p)->cap = 256;
  (*p)->tam = 0;
}

// Caso a pilha fique cheia, função chamada para realocá-la com o dobro de capacidade
void p_cheia(pilha_t *p) {
  tarefa_t *nova_colecao;
  p->cap = p->cap * 2;
  nova_colecao = (tarefa_t *) realloc(p->colecao, p->cap);
  p->colecao = nova_colecao;
}

// Indicador se a pilha está vazia
int p_vazia(pilha_t *p) {
  return !(p->tam);
}

// Lógica de inserção na pilha, verifica se a pilha se encontra cheia
void p_insere(pilha_t *p, tarefa_t t) {
  if(p->tam == p->cap) p_cheia(p);
  p->colecao[p->indice] = t;
  p->indice++;
  p->tam++;
}

// Lógica de retirada de tarefas da pilha
tarefa_t p_retira(pilha_t *p) {
  tarefa_t ret;
  p->indice--;
  ret = p->colecao[p->indice];
  p->tam--;
  return ret;
}

// Função para a liberação de memória alocada
void p_destroi(pilha_t *p) {
  free(p->colecao);
  free(p);
}
