#ifndef BUSCABINARIA_H_INCLUDED
#define BUSCABINARIA_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Entidades/assinaturas.h"


void salvarDadosBinaria(int comparacoes, double tempoExecucao);

TCliente *buscaBinariaCliente(FILE *out, int codigo);

TProduto *buscaBinariaProduto(FILE *out, int codigo);

TPedido *buscaBinariaPedido(FILE *out, int codigo);


#endif // BUSCASEQUENCIAL_H_INCLUDED
