#ifndef BUSCABINARIA_H_INCLUDED
#define BUSCABINARIA_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Entidades/assinaturas.h"

// typedef struct Funcionario
// {
//     int cod;
//     char nome[50];
//     char cpf[15];
//     char data_nascimento[11];
//     double salario;
// } TFunc;


void salvarDadosBinaria(int comparacoes, double tempoExecucao);

TCliente *buscaBinariaCliente(FILE *out, int codigo);

TProduto *buscaBinariaProduto(FILE *out, int codigo);

TPedido *buscaBinariaPedido(FILE *out, int codigo);


#endif // BUSCASEQUENCIAL_H_INCLUDED
