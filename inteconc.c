#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"
#include "inteconc.h"

typedef struct inicio {
  double base_inicial;
  double area_inicial;
  double e;
  double a;
  double b;
  double (*f)(double);
} dados_t;

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

void p_init (pilha_t **p) {
  *p = (pilha_t *) malloc(sizeof(pilha_t));
  (*p)->topo = NULL;
  (*p)->tamanho = 0;
}

int p_vazia(pilha_t **p) {
  return !((*p)->tamanho);
}

no_t *novo_no(tarefa_t t) {
  no_t *novo = (no_t *) malloc(sizeof(no_t));
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
  if(p_vazia(p)) {
    exit(-1);
  }
  no_t *temp = (*p)->topo;
  (*p)->topo = (*p)->topo->proximo;
  t = temp->t;
  free(temp);
  (*p)->tamanho--;
  return t;
}

pthread_mutex_t m_pilha, m_inativos, m_total;
pthread_cond_t c_tamanho;

int inativos = 0;
int numThreads;

int n_intervalos = 2;
double erro;
double area_atual = 0;
double base;
dados_t dados_iniciais;

pilha_t *p_tarefas;

void *integra(void *arg) {
  int id = * (int *) arg;
  tarefa_t t_in, t_l, t_r;
  double q, lq, rq, m, erro;
  double q_total = 0;

  while(1) {
    pthread_mutex_lock(&m_pilha);
    inativos++;
    if(inativos == numThreads && p_vazia(&p_tarefas)) {
      pthread_cond_broadcast(&c_tamanho);
      pthread_mutex_unlock(&m_pilha);
      break;
    }

    while(p_vazia(&p_tarefas)) {
      /* printf("Thread %d: se bloqueou\n", id); */
      pthread_cond_wait(&c_tamanho, &m_pilha);
      if(p_vazia(&p_tarefas) && inativos == numThreads) break;
    }
    /* printf("Thread %d: voltou\n", id); */

    if (p_vazia(&p_tarefas) && inativos == numThreads) {
      pthread_mutex_unlock(&m_pilha);
      break;
    }
    t_in = p_retira(&p_tarefas);
    inativos--;
    pthread_mutex_unlock(&m_pilha);

    q = t_in.area_maior;
    m = (t_in.l + t_in.r) / 2;
    lq = area(t_in.l, m, dados_iniciais.f);
    //printf("lq: %lf\n", lq);
    rq = area(m, t_in.r, dados_iniciais.f);
    // printf("rq: %lf\n", rq);
    erro = fabs(q - (lq + rq));
    if(erro > dados_iniciais.e) {
      t_l.l = t_in.l; t_l.r = m; t_l.area_maior = lq;
      t_r.r = t_in.r;
      t_r.l = m;
      t_r.area_maior = rq;
      pthread_mutex_lock(&m_pilha);
      p_insere(&p_tarefas, t_l);
      p_insere(&p_tarefas, t_r);
      pthread_mutex_unlock(&m_pilha);
      pthread_cond_broadcast(&c_tamanho);
    } else {
      q_total += q;
    }
  }

  pthread_mutex_lock(&m_total);
  area_atual += q_total;
  pthread_mutex_unlock(&m_total);

  free(arg);
  pthread_exit(NULL);
}

double integral_conc(int nthreads, double a, double b, double e, double (*f)(double)) {
  pthread_t *sys_tids;
  int *tid;
  int t;
  tarefa_t tarefa_inicial;

  //dados_iniciais.base_inicial = (b - a) / 2;
  //dados_iniciais.area_inicial = area(a, b, f);
  dados_iniciais.e = e;
  //dados_iniciais.a = a;
  //dados_iniciais.b = b;
  dados_iniciais.f = f;

  p_init(&p_tarefas);

  numThreads = nthreads;

  //base = dados_iniciais.base_inicial;

  pthread_mutex_init(&m_pilha, NULL);
  pthread_mutex_init(&m_inativos, NULL);
  pthread_mutex_init(&m_total, NULL);

  // printf("Mutex inicializados\n");

  tarefa_inicial.area_maior = area(a, b, f);
  tarefa_inicial.l = a;
  tarefa_inicial.r = b;

  // printf("Tarefa inicial inicializadas\n");

  p_insere(&p_tarefas, tarefa_inicial);
  // printf("Tarefa inicial inserida");

  sys_tids = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
  for (t = 0; t < nthreads; t++) {
    tid = (int *) malloc(sizeof(int));
    *tid = t;
    if(pthread_create(&sys_tids[t], NULL, integra, (void *) tid)) exit(-1);
  }

  for(t = 0; t < nthreads; t++) {
    if(pthread_join(sys_tids[t], NULL)) exit(-1);
  }

  pthread_mutex_destroy(&m_pilha);
  pthread_mutex_destroy(&m_inativos);
  pthread_mutex_destroy(&m_total);

  free(p_tarefas);
  free(sys_tids);
  return area_atual;
}
