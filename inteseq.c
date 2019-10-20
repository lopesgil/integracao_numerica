#include <stdio.h>
#include <math.h>
#include "utils.h"
#include "inteseq.h"

double integral_seq(double a, double b, double e, double (*f)(double)) {
  double passo, erro, area_anterior, area_posterior = 0, n = 2;
  double stp;
  int i;

  area_anterior = area(a, b, f);
  do {
    passo = fabs(b - a) / n;
    stp = a;
    for(i = 0; i < n; i++) {
      area_posterior += area(stp, stp + passo, f);
      stp += passo;
    }
    erro = fabs(area_posterior - area_anterior);
    area_anterior = area_posterior;
    area_posterior = 0;
    n *= 2;
  } while (erro > e);
  return area_anterior;
}
