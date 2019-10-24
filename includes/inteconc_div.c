#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"
#include "inteconc_div.h"

typedef struct tarefa {
  double l;
  double r;
} tarefa_t;

tarefa_t *tarefas_iniciais;

pthread_mutex_t m_total;

double *areas_threads;
double e;
double (*func)(double);

double quadratura_div(double l, double r) {
  double q, lq, rq, m;
  double erro;

  m = (l + r) / 2;
  q = area(l, r, func);
  lq = area(l, m, func);
  rq = area(m, r, func);

  erro = fabs(q - (lq + rq));
  if (erro > e) {
    q = quadratura_div(l, m) + quadratura_div(m, r);
  }

  return q;
}

void *integra_div(void *arg) {
  int id = * (int *) arg;
  double area_thread;
  tarefa_t tarefa = tarefas_iniciais[id];

  area_thread = quadratura_div(tarefa.l, tarefa.r);
  areas_threads[id] += area_thread;

  free(arg);
  pthread_exit(NULL);
}

double integral_conc_div(int nThreads, double a, double b, double erro, double (*f)(double)) {
  pthread_t *sys_tids;
  int t;
  int *tid;
  double divisao;
  double area_total = 0;

  e = erro;
  func = f;

  areas_threads = (double *) malloc(sizeof(double) * nThreads);

  divisao = (b - a) / nThreads;
  tarefas_iniciais = (tarefa_t *) malloc(sizeof(tarefa_t) * nThreads);
  for (t = 0; t < nThreads; t++) {
    tarefas_iniciais[t].l = a + (t * divisao);
    tarefas_iniciais[t].r = a + ((t+1) * divisao);
  }

  sys_tids = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
  for (t = 0; t < nThreads; t++) {
    tid = (int *) malloc(sizeof(int));
    *tid = t;
    pthread_create(&sys_tids[t], NULL, integra_div, (void *) tid);
  }

  for (t = 0; t < nThreads; t++) {
    pthread_join(sys_tids[t], NULL);
    area_total += areas_threads[t];
  }

  free(areas_threads);
  free(sys_tids);
  free(tarefas_iniciais);
  return area_total;
}
