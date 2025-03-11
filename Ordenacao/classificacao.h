#ifndef ORDENACAOEXTERNA_H_INCLUDED
#define ORDENACAOEXTERNA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Entidades/assinaturas.h"


int selecaoNatural(FILE *entrada, int M);

void inserirMemoria(TCliente heap[], int *n, TCliente novo);

TCliente extrairMin(TCliente heap[], int *n);

void memMinimo(TCliente heap[], int n, int i);

void trocar(TCliente *a, TCliente *b);

void salvarDadosClassificacao(int comparacoes, double tempoExecucao);

void verificaParticoes(int numParticoes);

#endif // ORDENACAOEXTERNA_H_INCLUDED