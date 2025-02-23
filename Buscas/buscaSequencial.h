#ifndef BUSCASEQUENCIAL_H_INCLUDED
#define BUSCASEQUENCIAL_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Entidades/assinaturas.h"

void salvarDadosSequencial(int comparacoes, double tempoExecucao);

TCliente *buscaSequencialCliente(int chave, FILE *in);

int buscaSequencialPosicaoCliente(int chave, FILE *arquivo);

TProduto *buscaSequencialProduto(int chave, FILE *in);

TPedido *buscaSequencialPedido(int chave, FILE *in);




#endif // BUSCASEQUENCIAL_H_INCLUDED
