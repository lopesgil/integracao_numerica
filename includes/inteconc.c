#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>
#include "utils.h"
#include "pilha.h"
#include "inteconc.h"

// Dados iniciais fixos do problema: erro máximo e função a ser integrada
typedef struct inicio {
  double e;
  double (*f)(double);
} dados_t;
dados_t dados_iniciais;

// Variáveis usadas na sincronização do acesso aos dados
pthread_mutex_t m_pilha, m_total;
pthread_cond_t c_tamanho;

// Outras variáveis globais usadas no algoritmo para determinar a condição de parada
int inativos = 0;
int numThreads;

// Acumulador da área total
double area_atual = 0;

// Pilha usada para a comunicação de tarefas entre as threads
pilha_t *p_tarefas;

void *integra(void *arg) {
  int id = * (int *) arg;
  // Flag para decidir se a thread precisa buscar outra tarefa na pilha
  int segurou = 0;
  // Tarefa de entrada e tarefas de saída determinadas pela thread
  tarefa_t t_in, t_l, t_r;
  // Variáveis da lógica de quadratura
  double q, lq, rq, m, erro;
  double q_total = 0;

  while(1) {
    // Avalia se é necessário retirar uma tarefa da pilha
    if (!segurou) {
      pthread_mutex_lock(&m_pilha);
      inativos++;
      // Se todas as threads estão inativas e a pilha está vazia, não há mais intervalos a serem calculados
      if(inativos == numThreads && p_vazia(p_tarefas)) {
        // Sinaliza outras possíves threads bloqueadas
        pthread_cond_broadcast(&c_tamanho);
        pthread_mutex_unlock(&m_pilha);
        break;
      }

      // Se a pilha está vazia, a thread se bloqueia esperando uma inserção
      while(p_vazia(p_tarefas)) {
        pthread_cond_wait(&c_tamanho, &m_pilha);
        // Se ao voltar do bloqueio todas as threads estiverem inativas, a execução encerrou
        if(p_vazia(p_tarefas) && inativos == numThreads) break;
      }

      if (p_vazia(p_tarefas) && inativos == numThreads) {
        // Verifica novamente para encerrar a execução (pode ser melhorado)
        pthread_mutex_unlock(&m_pilha);
        break;
      }
      // Retira uma tarefa da pilha e remove a thread da contagem de inativos
      t_in = p_retira(p_tarefas);
      inativos--;
      pthread_mutex_unlock(&m_pilha);
    }

    // Algoritmo da quadratura
    // q representa a área do retângulo maior (previamente calculada)
    q = t_in.area_maior;
    m = (t_in.l + t_in.r) / 2;
    // lq e rq representam as áreas dos retângulos menores
    lq = area(t_in.l, m, dados_iniciais.f);
    rq = area(m, t_in.r, dados_iniciais.f);
    // Compara a área maior com a soma das áreas dos retângulos menores
    erro = fabs(q - (lq + rq));
    if(erro > dados_iniciais.e) {
      // Define a subtarefa esquerda com intervalo [l, m] e área maior lq
      t_l.l = t_in.l;
      t_l.r = m;
      t_l.area_maior = lq;
      // Define a subtarefa direita com intervalo [m, r] e área maior rq
      t_r.l = m;
      t_r.r = t_in.r;
      t_r.area_maior = rq;
      pthread_mutex_lock(&m_pilha);
      // Insere a subtarefa esquerda na pilha e segura a subtarefa direita (minimizar os acessos à memória)
      p_insere(p_tarefas, t_l);
      t_in = t_r;
      segurou = 1;
      pthread_mutex_unlock(&m_pilha);
      pthread_cond_broadcast(&c_tamanho);
    } else {
      segurou = 0;
      // acumula o resultado parcial calculado pela thread
      q_total += q;
    }
  }

  pthread_mutex_lock(&m_total);
  // soma o resultado parcial da thread ao valor global
  area_atual += q_total;
  pthread_mutex_unlock(&m_total);

  free(arg);
  pthread_exit(NULL);
}

// Função envelope da lógica concorrente
double integral_conc(int nthreads, double a, double b, double e, double (*f)(double)) {
  pthread_t *sys_tids;
  int *tid;
  int t;
  tarefa_t tarefa_inicial;

  // Inicializa os valores globais fixos
  dados_iniciais.e = e;
  dados_iniciais.f = f;

  // Inicializa a pilha
  p_init(&p_tarefas);

  numThreads = nthreads;

  // Inicializa os mutex
  pthread_mutex_init(&m_pilha, NULL);
  pthread_mutex_init(&m_total, NULL);

  // Calcula a tarefa baseada no intervalo inicial, que será empilhada para iniciar a lógica das threads
  tarefa_inicial.area_maior = area(a, b, f);
  tarefa_inicial.l = a;
  tarefa_inicial.r = b;

  // Empilha a tarefa inicial
  p_insere(p_tarefas, tarefa_inicial);

  // Dispara as threads
  sys_tids = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
  for (t = 0; t < nthreads; t++) {
    tid = (int *) malloc(sizeof(int));
    *tid = t;
    if(pthread_create(&sys_tids[t], NULL, integra, (void *) tid)) exit(-1);
  }

  // Aguarda o término das threads
  for(t = 0; t < nthreads; t++) {
    if(pthread_join(sys_tids[t], NULL)) exit(-1);
  }

  // Libera a memória alocada e as estruturas usadas
  pthread_mutex_destroy(&m_pilha);
  pthread_mutex_destroy(&m_total);

  p_destroi(p_tarefas);

  free(sys_tids);
  return area_atual;
}
