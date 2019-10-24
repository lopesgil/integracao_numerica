#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "includes/timer.h"
#include "includes/funcs.h"
#include "includes/inteseq.h"
#include "includes/inteconc.h"
#include "includes/inteconc_div.h"

int main(int argc, char *argv[]) {
  double a, b, e, resultado_seq, resultado_conc, resultado_conc_div;
  double comeco, fim, tempo_concorrente, tempo_concorrente_div, tempo_sequencial;
  double (*f)(double);
  int nThreads;

  if (argc < 5) {
    fprintf(stderr, "Digite: %s <nº de threads> <a> <b> <erro> \n", argv[0]);
    exit(-1);
  }

  nThreads = atoi(argv[1]);
  a = atof(argv[2]);
  b = atof(argv[3]);
  e = atof(argv[4]);
  f = f_f;

  GET_TIME(comeco);
  resultado_conc = integral_conc(nThreads, a, b, e, f);
  GET_TIME(fim);
  tempo_concorrente = fim - comeco;

  GET_TIME(comeco);
  resultado_seq = integral_seq(a, b, e, f);
  GET_TIME(fim);
  tempo_sequencial = fim - comeco;

  GET_TIME(comeco);
  resultado_conc_div = integral_conc_div(nThreads, a, b, e, f);
  GET_TIME(fim);
  tempo_concorrente_div = fim - comeco;

  printf("Comparativo da função (f):\n");
  printf("Resultado concorrente com divisão inicial: %.9lf\n", resultado_conc_div);
  printf("Tempo de execução concorrente com divisão inicial: %.9lf\n", tempo_concorrente_div);
  printf("Resultado concorrente: %.9lf\n", resultado_conc);
  printf("Tempo de execução concorrente: %.9lf\n", tempo_concorrente);
  printf("Resultado sequencial: %.9lf\n", resultado_seq);
  printf("Tempo de execução sequencial: %.9lf\n", tempo_sequencial);
  return 0;
}
