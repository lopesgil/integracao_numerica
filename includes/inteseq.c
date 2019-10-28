#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "inteseq.h"

double integral_seq(double a, double b, double e, double (*f)(double)) {
  double q, lq, rq, m;
  double erro;

  // Calcula o ponto médio, a área do retângulo maior e as áreas dos retângulos menores
  m = (b + a) / 2;
  q = area(a, b, f);
  lq = area(a, m, f);
  rq = area(m, b, f);

  // Avalia a diferença entre a área do retângulo maior e a soma das áreas dos retângulos menores
  erro = fabs(q - (lq + rq));
  if (erro > e) {
    // Se a diferença for maior do que o erro máximo, divide o problema em dois e repete a operação recursivamente
    q = integral_seq(a, m, e, f) + integral_seq(m, b, e, f);
  }

  // Ao encontrar um resultado satisfatório, retorna-o
  return q;
}
