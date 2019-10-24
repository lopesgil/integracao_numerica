#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"
#include "pilha.h"
#include "inteconc.h"

typedef struct inicio {
  double e;
  double (*f)(double);
} dados_t;

pthread_mutex_t m_pilha, m_total;
pthread_cond_t c_tamanho;

int inativos = 0;
int numThreads;

double area_atual = 0;
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
    rq = area(m, t_in.r, dados_iniciais.f);
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

  dados_iniciais.e = e;
  dados_iniciais.f = f;

  p_init(&p_tarefas);

  numThreads = nthreads;

  pthread_mutex_init(&m_pilha, NULL);
  pthread_mutex_init(&m_total, NULL);

  tarefa_inicial.area_maior = area(a, b, f);
  tarefa_inicial.l = a;
  tarefa_inicial.r = b;

  p_insere(&p_tarefas, tarefa_inicial);

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
  pthread_mutex_destroy(&m_total);

  p_destroi(&p_tarefas);

  free(sys_tids);
  return area_atual;
}
