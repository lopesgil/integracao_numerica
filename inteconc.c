#include <stdlib.h>
#include <pthread.h>
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
  double base;
  double ponto_medio;
} tarefa_t;

pthread_mutex_t n_mutex;
int numThreads;

int n_intervalos = 2;
double erro;
double area_atual = 0;
double base;
dados_t dados_iniciais;

void *integra(void *arg) {
  int id = * (int *) arg;
  int i;
  int n;
  double area_thread;

  do {
    pthread_mutex_lock(&n_mutex);
    n = n_intervalos;
    pthread_mutex_unlock(&n_mutex);
    area_thread = 0;
    for(i = id; i < n; i += numThreads) {

    }
  } while (erro > dados_iniciais.e);

  free(arg);
  pthread_exit(NULL);
}

double integral_conc(int nthreads, double a, double b, double e, double (*f)(double)) {
  pthread_t *sys_tids;
  int *tid;
  int t;

  dados_iniciais.base_inicial = (b - a) / 2;
  dados_iniciais.area_inicial = area(a, b, f);
  dados_iniciais.e = e;
  dados_iniciais.a = a;
  dados_iniciais.b = b;
  dados_iniciais.f = f;

  numThreads = nthreads;

  base = dados_iniciais.base_inicial;

  pthread_mutex_init(&n_mutex, NULL);

  sys_tids = (pthread_t *) malloc(sizeof(pthread_t) * t);
  for (t = 0; t < nthreads; t++) {
    tid = malloc(sizeof(int));
    *tid = t;
    if(pthread_create(&sys_tids[t], NULL, integra, (void *) tid)) exit(-1);
  }

  for(t = 0; t < nthreads; t++) {
    if(pthread_join(sys_tids[t], NULL)) exit(-1);
  }

  pthread_mutex_destroy(&n_mutex);

  free(sys_tids);
  return dados_iniciais.area_inicial;
}
