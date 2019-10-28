#ifndef INTECONC_H_
#define INTECONC_H_

/*
 Função responsável pelo algoritmo concorrente
 Versão utilizando pilha para garantir o balanceamento de carga (ineficiente)
 Parâmetros e retorno idênticos à versão com divisão inicial de tarefa
*/
double integral_conc(int, double, double, double, double(*f)(double));

#endif
