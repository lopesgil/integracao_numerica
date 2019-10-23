#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "inteseq.h"

double integral_seq_ite(double a, double b, double e, double (*f)(double)) {
  double passo, erro, area_anterior, area_posterior = 0, n = 2;
  double area_f, stp;
  int i;

  area_anterior = area(a, b, f);
  do {
    passo =  (b - a) / n;
    stp = a;
    for(i = 0; i < n; i++) {
      area_posterior += area(stp, stp + passo, f);
      stp += passo;
    }
    erro = fabs(area_posterior - area_anterior);
    area_f = area_anterior;
    area_anterior = area_posterior;
    area_posterior = 0;
    n *= 2;
  } while (erro > e);
  return area_f;
}

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
