#include "funcs.h"
#include <math.h>

double f_a(double x) {
  return 1 + x;
}

double f_b(double x) {
    return sqrt(1 - pow(x, 2));
}

double f_c(double x) {
  return sqrt(1 + pow(x, 4));
}

double f_d(double x) {
  return sin(pow(x, 2));
}

double f_e(double x) {
  return cos(pow(M_E, -x));
}

double f_f(double x) {
  return (cos(pow(M_E, -x))) * x;
}

double f_g(double x) {
  return (cos(pow(M_E, -x))) * (0.005 * pow(x, 3) + 1);
}

