#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include "funcs.h"
#include "inteseq.h"
#include "inteconc.h"

int main() {
  double resultado_seq, resultado_conc;
  double comeco, fim, tempo_concorrente, tempo_sequencial;

  GET_TIME(comeco);
  resultado_conc = integral_conc(4, -10, 20, 0.000000001, f_g);
  GET_TIME(fim);
  tempo_concorrente = fim - comeco;

  GET_TIME(comeco);
  resultado_seq = integral_seq(-10, 20, 0.000000001, f_g);
  GET_TIME(fim);
  tempo_sequencial = fim - comeco;

  printf("Resultado concorrente: %.9lf\n", resultado_conc);
  printf("Tempo de execução concorrente: %.9lf\n", tempo_concorrente);
  printf("Resultado sequencial: %.9lf\n", resultado_seq);
  printf("Tempo de execução sequencial: %.9lf\n", tempo_sequencial);
  return 0;
}
