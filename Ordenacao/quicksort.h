#ifndef QUICKSORT_H_INCLUDED
#define QUICKSORT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "../Entidades/assinaturas.h"


void quickSort(FILE *arq, int p, int r, int *comparacoes);

int particao(FILE *arq, int p, int r, int *comparacoes);

void salvarDadosQuickSort(int comparacoes, double tempoExecucao);

#endif // QUICKSORT_H_INCLUDED