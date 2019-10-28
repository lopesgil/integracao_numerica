#ifndef INTECONC_DIV_H_
#define INTECONC_DIV_H_

/*
  Função envelope para o algoritmo concorrente de integração
  Versão utilizando divisão inicial de tarefas (não garante balanceamento)
  Recebe o número de threads; o intervalo [a,b]; o erro máximo e;
  e a função f cuja integral se deseja obter
  Retorna o valor aproximado da integral
*/

double integral_conc_div(int, double, double, double, double(*)(double));

#endif
