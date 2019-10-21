#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"
#include "funcs.h"
#include "inteseq.h"

int main() {
  double resultado;
  double comeco, fim, tempo;

  GET_TIME(comeco);
  resultado = integral_seq(-1, -9, 0.00000001, f_g);
  GET_TIME(fim);
  tempo = fim - comeco;
  printf("Resultado: %.5lf\n", resultado);
  printf("Tempo de execução: %.9lf\n", tempo);
  return 0;
}
