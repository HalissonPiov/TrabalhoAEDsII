#ifndef ORDENACAOEXTERNA_H_INCLUDED
#define ORDENACAOEXTERNA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Entidades/assinaturas.h"


// *********************************************** ENTIDADE CLIENTE ***********************************************

int selecaoNaturalCliente(FILE *entrada, int M);

void inserirMemoriaCliente(TCliente heap[], int *n, TCliente novo);

TCliente extrairMinCliente(TCliente heap[], int *n);

void memMinimaCliente(TCliente heap[], int n, int i);

void trocarCliente(TCliente *a, TCliente *b);

void salvarDadosClassificacao(int comparacoes, double tempoExecucao);

void verificaParticoesCliente(int numParticoes);


// *********************************************** ENTIDADE CLIENTE ***********************************************

int selecaoNaturalProduto(FILE *entrada, int M);

void inserirMemoriaProduto(TProduto heap[], int *n, TProduto novo);

TProduto extrairMinProduto(TProduto heap[], int *n);

void memMinimaProduto(TProduto heap[], int n, int i);

void trocarProduto(TProduto *a, TProduto *b);

void salvarDadosClassificacao(int comparacoes, double tempoExecucao);

void verificaParticoesProduto(int numParticoes);

#endif // ORDENACAOEXTERNA_H_INCLUDED