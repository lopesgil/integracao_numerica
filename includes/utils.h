#ifndef UTILS_H_
#define UTILS_H_

/*
  Função utilitária que calcula de um retângulo
  Recebe os limites do intervalo [l, r] e a função f
  Retorna a área do retângulo com base [l, r] e altura f(m) onde m
  é o ponto médio
*/

double area(double, double, double (*)(double));

#endif
