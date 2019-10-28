#ifndef INTESEQ_H_
#define INTESEQ_H_

/*
 Função representando o algoritmo recursivo da quadratura
 adaptativa clássica
 Recebe o intervalo [a, b], o erro máximo e, e a função f a ser integrada
 Retorna o valor aproximado da integral da função f no intervalo [a, b]
*/

double integral_seq(double, double, double, double (*)(double));

#endif
