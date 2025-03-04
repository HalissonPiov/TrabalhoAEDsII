#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "quicksort.h"
#include "../Entidades/assinaturas.h"


// void QuickSort(FILE *A, int p, int r){

//     int p, r, q;

//     if (p < r){
//         q = particao(A, p, r);
//         QuickSort(A, p, q - 1);
//         QuickSort(A, q + 1, r);
//     }
// }


// int particao(FILE *A, int p, int r){

//     FILE x;
//     FILE Aux;
//     x = A[r];

//     int i = p - 1;
//     for(int j = p; j < r; j++){
//         if (A[j].Chave <= x.Chave){
//             i = i + 1;
//             Aux = A[i];
//             A[i] = A[j];
//             A[j] = Aux;
//         }
//     }
//     Aux = A[i + 1];
//     A[i + 1] = A[r];
//     A[r] = Aux;
//     return i + 1;
// }

void salvarDadosQuickSort(int comparacoes, double tempoExecucao)
{
    FILE *arquivo = fopen("ArquivosLog/dadosQuickSort.txt", "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    // ----------------
    fprintf(arquivo, "Comparações: %d\n", comparacoes);
    fprintf(arquivo, "Tempo de execução: %.2f segundos\n", tempoExecucao);
    fclose(arquivo);
    // ----------------
}


void quickSort(FILE *arq, int p, int r, int *comparacoes) {
    if (p < r) {
        int q = particao(arq, p, r, comparacoes);
        quickSort(arq, p, q - 1, comparacoes);
        quickSort(arq, q + 1, r, comparacoes);
    }
}

int particao(FILE *arq, int p, int r, int *comparacoes) {
    
    TCliente pivo;  
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));

    int i = p - 1;

    // Lê o pivô: registro na posição r
    fseek(arq, (r - 1) * tamanho_registroCliente(), SEEK_SET);
    cliente = leCliente(arq);
    pivo = *cliente;  // copia o registro para 'pivo'
    free(cliente);

    for (int j = p; j < r; j++) {

        TCliente *aj = (TCliente *)malloc(sizeof(TCliente));

        // Lê o registro j
        fseek(arq, (j - 1) * tamanho_registroCliente(), SEEK_SET);
        aj = leCliente(arq);

        (*comparacoes)++;

        // Compara pelo campo ID
        if (aj->id <= pivo.id) {

            i++;
            // Realiza a troca entre os registros nas posições i e j

            // Lê o registro na posição i
            fseek(arq, (i - 1) * tamanho_registroCliente(), SEEK_SET);  
            TCliente *cliente_i = leCliente(arq);

            // Escreve o registro j (aj) na posição i
            fseek(arq, (i - 1) * tamanho_registroCliente(), SEEK_SET);
            salvaCliente(aj, arq);

            // Escreve o registro que estava na posição i (cliente_i) na posição j
            fseek(arq, (j - 1) * tamanho_registroCliente(), SEEK_SET);
            salvaCliente(cliente_i, arq);

            free(cliente_i);
        }
        free(aj);
    }

    // Troca o registro da posição i+1 com o pivô (registro r)
    fseek(arq, (i + 1 - 1) * tamanho_registroCliente(), SEEK_SET);
    TCliente *temp_ip1 = leCliente(arq);

    // Escreve o pivô na posição i+1
    fseek(arq, (i + 1 - 1) * tamanho_registroCliente(), SEEK_SET);
    salvaCliente(&pivo, arq);

    // Escreve o registro que estava em i+1 (temp_ip1) na posição r
    fseek(arq, (r - 1) * tamanho_registroCliente(), SEEK_SET);
    salvaCliente(temp_ip1, arq);

    free(temp_ip1);

    return i + 1;
}