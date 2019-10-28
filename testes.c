#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "includes/timer.h"
#include "includes/funcs.h"
#include "includes/inteseq.h"
#include "includes/inteconc_div.h"
#include "includes/inteconc.h"

#define VERS 3
#define NUM_FUNC 7

void processa_ganho(double temp[NUM_FUNC][VERS]) {
  int i;
  double ganho_d, ganho_p;

  printf("Ganhos: (tempo sequencial) / (tempo concorrente)\n");
  for (i = 0; i < NUM_FUNC; i++) {
    ganho_d = temp[i][0] / temp[i][1];
    ganho_p = temp[i][0] / temp[i][2];
    printf("f(%c) -> Concorrente(D): [%6.4lf]  Concorrente(P): [%6.4lf]\n", 97 + i, ganho_d, ganho_p);
  }
}

int main(int argc, char *argv[]) {
  double a, b, erro, res;
  double comeco, fim, tempo;
  double tempos_f[NUM_FUNC][VERS];
  int i, j, nThreads;
  double (*funcs[NUM_FUNC])(double) = {f_a, f_b, f_c, f_d, f_e, f_f, f_g};

  if (argc < 5) {
    fprintf(stderr, "Digite: %s <nº de threads> <a> <b> <erro máximo>\n", argv[0]);
    exit(-1);
  }

  nThreads = atoi(argv[1]);
  a = atof(argv[2]);
  b = atof(argv[3]);
  erro = atof(argv[4]);

  for (i = 0; i < NUM_FUNC; i++) {
    for (j = 0; j < VERS; j++) {
      tempos_f[i][j] = -1;
    }
  }

  printf("Algoritmo sequencial:\n");
  for (i = 0; i < NUM_FUNC; i++) {
    if((i == 1) && ((a < -1 || b > 1) || (b < -1 || a > 1)))
      continue;

    GET_TIME(comeco);
    res = integral_seq(a, b, erro, funcs[i]);
    GET_TIME(fim);
    tempo = fim - comeco;
    tempos_f[i][0] = tempo;

    printf("f(%c) -> resultado: %15.9lf | tempo: %15.9lf\n", 97 + i, res, tempo);
  }
  printf("\n");

  printf("Algoritmo concorrente com divisão inicial de tarefas:\n");
  for (i = 0; i < NUM_FUNC; i++) {
    if ((i == 1) && ((a < -1 || b > 1) || (b < -1 || a > 1)))
      continue;

    GET_TIME(comeco);
    res = integral_conc_div(nThreads, a, b, erro, funcs[i]);
    GET_TIME(fim);
    tempo = fim - comeco;
    tempos_f[i][1] = tempo;

    printf("f(%c) -> resultado: %15.9lf | tempo: %15.9lf\n", 97 + i, res, tempo);
  }
  printf("\n");

  printf("Algoritmo concorrente com pilha de tarefas:\n");
  for (i = 0; i < NUM_FUNC; i++) {
    if ((i == 1) && ((a < -1 || b > 1) || (b < -1 || a > 1)))
      continue;

    GET_TIME(comeco);
    res = integral_conc(nThreads, a, b, erro, funcs[i]);
    GET_TIME(fim);
    tempo = fim - comeco;
    tempos_f[i][2] = tempo;

    printf("f(%c) -> resultado: %15.9lf | tempo: %15.9lf\n", 97 + i, res, tempo);
  }
  printf("\n");

  processa_ganho(tempos_f);

  return 0;
}

