#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"
#include "inteconc_div.h"

// Estrutura para representação das tarefas iniciais das threads
typedef struct tarefa {
  double l;
  double r;
} tarefa_t;

tarefa_t *tarefas_iniciais;

// áreas parciais calculadas pelas threads, erro máximo e função a ser integrada
double *areas_threads;
double e;
double (*func)(double);

// Função recursiva de quadratura, idêntica ao algoritmo sequencial
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

// Função de execução das threads
void *integra_div(void *arg) {
  int id = * (int *) arg;
  double area_thread;
  tarefa_t tarefa = tarefas_iniciais[id];

  // Cada thread chama a função de quadratura a partir do seu intervalo inicial
  // Ao final, guardam o resultado no vetor designado para os resultados parciais
  area_thread = quadratura_div(tarefa.l, tarefa.r);
  areas_threads[id] = area_thread;

  free(arg);
  pthread_exit(NULL);
}

// Função envelope da lógica concorrente
double integral_conc_div(int nThreads, double a, double b, double erro, double (*f)(double)) {
  pthread_t *sys_tids;
  int t;
  int *tid;
  double divisao;
  double area_total = 0;

  // Inicializa os valores globais usados pelas threads
  e = erro;
  func = f;
  areas_threads = (double *) malloc(sizeof(double) * nThreads);

  // Divide o intervalo inicial em subintervalos para cada uma das threads
  divisao = (b - a) / nThreads;
  tarefas_iniciais = (tarefa_t *) malloc(sizeof(tarefa_t) * nThreads);
  for (t = 0; t < nThreads; t++) {
    tarefas_iniciais[t].l = a + (t * divisao);
    tarefas_iniciais[t].r = a + ((t+1) * divisao);
  }

  // Dispara as threads
  sys_tids = (pthread_t *) malloc(sizeof(pthread_t) * nThreads);
  for (t = 0; t < nThreads; t++) {
    tid = (int *) malloc(sizeof(int));
    *tid = t;
    pthread_create(&sys_tids[t], NULL, integra_div, (void *) tid);
  }

  // Ao término de cada thread, soma o seu resultado parcial ao resultado geral
  for (t = 0; t < nThreads; t++) {
    pthread_join(sys_tids[t], NULL);
    area_total += areas_threads[t];
  }

  // Libera a memória usada e retorna o resultado encontrado
  free(areas_threads);
  free(sys_tids);
  free(tarefas_iniciais);
  return area_total;
}
