#ifndef INTECONC_H_
#define INTECONC_H_

// extern int n_intervalos;
// extern double erro;
// extern double area_atual;

// extern dados_t dados_iniciais;

// tarefa_t pega_tarefa();
// void *integra(void*);
double integral_conc(int, double, double, double, double(*f)(double));

#endif
