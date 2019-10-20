#include <stdio.h>
#include <stdlib.h>
#include "funcs.h"
#include "inteseq.h"

int main() {
  double resultado;

  resultado = integral_seq(0, 3, 0.00000001, f_g);
  printf("Resultado: %.5lf\n", resultado);
  return 0;
}
