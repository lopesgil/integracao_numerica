#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "inteseq.h"

double integral_seq(double a, double b, double e, double (*f)(double)) {
  double q, lq, rq, m;
  double erro;

  m = (b + a) / 2;
  q = area(a, b, f);
  lq = area(a, m, f);
  rq = area(m, b, f);

  erro = fabs(q - (lq + rq));
  if (erro > e) {
    q = integral_seq(a, m, e, f) + integral_seq(m, b, e, f);
  }

return q;
}
