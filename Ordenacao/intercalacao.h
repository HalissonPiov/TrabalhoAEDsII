#ifndef INTERCALACAO_H_INCLUDED
#define INTERCALACAO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Entidades/assinaturas.h"

// Estrutura para armazenar os primeiros registros de cada arquivo
typedef struct
{
    TCliente cliente;
    int origem; // Índice do arquivo de origem
} ElementoMemoria;

void intercalacaoOtima(int numArquivos, int F);

void intercalarArquivos(char arquivos[][50], int numArquivos, char *arquivoSaida);

void memMinimaElementos(ElementoMemoria heap[], int n, int i);

#endif // INTERCALACAO_H_INCLUDED