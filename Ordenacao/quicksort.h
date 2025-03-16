#ifndef QUICKSORT_H_INCLUDED
#define QUICKSORT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "../Entidades/assinaturas.h"


void quickSortCliente(FILE *arq, int p, int r, int *comparacoes);

int particaoCliente(FILE *arq, int p, int r, int *comparacoes);

void quickSortProduto(FILE *arq, int p, int r, int *comparacoes);

int particaoProduto(FILE *arq, int p, int r, int *comparacoes);

void salvarDadosQuickSort(int comparacoes, double tempoExecucao);

#endif // QUICKSORT_H_INCLUDED