#include <math.h>
#include "utils.h"

double area(double a, double b, double (*f)(double)) {
  double base = fabs(b - a);
  double ponto_medio = (b + a) / 2;
  return base * f(ponto_medio);
}

