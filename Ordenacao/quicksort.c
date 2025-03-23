#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "quicksort.h"
#include "../Entidades/assinaturas.h"

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

void quickSortCliente(FILE *arq, int p, int r, int *comparacoes)
{
    if (p < r)
    {
        int q = particaoCliente(arq, p, r, comparacoes);
        quickSortCliente(arq, p, q - 1, comparacoes);
        quickSortCliente(arq, q + 1, r, comparacoes);
    }
}

int particaoCliente(FILE *arq, int p, int r, int *comparacoes)
{

    TCliente pivo;
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));

    int i = p - 1;

    // Lê o pivô: registro na posição r
    fseek(arq, (r - 1) * tamanho_registroCliente(), SEEK_SET);
    cliente = leCliente(arq);
    pivo = *cliente; 
    free(cliente);

    for (int j = p; j < r; j++)
    {

        TCliente *aj = (TCliente *)malloc(sizeof(TCliente));

        // Lê o registro j
        fseek(arq, (j - 1) * tamanho_registroCliente(), SEEK_SET);
        aj = leCliente(arq);

        (*comparacoes)++;

        // Compara pelo campo ID, o pivô com o elemento J
        if (aj->id <= pivo.id)
        {

            i++;
            // Lê o registro na posição i
            fseek(arq, (i - 1) * tamanho_registroCliente(), SEEK_SET);
            TCliente *cliente_i = leCliente(arq);

            // Troca os registros
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

    // Troca os registros
    // Escreve o pivô na posição i+1
    fseek(arq, (i + 1 - 1) * tamanho_registroCliente(), SEEK_SET);
    salvaCliente(&pivo, arq);

    // Escreve o registro que estava em i+1 (temp_ip1) na posição r
    fseek(arq, (r - 1) * tamanho_registroCliente(), SEEK_SET);
    salvaCliente(temp_ip1, arq);

    free(temp_ip1);

    return i + 1;
}

void quickSortProduto(FILE *arq, int p, int r, int *comparacoes)
{
    if (p < r)
    {
        int q = particaoProduto(arq, p, r, comparacoes);
        quickSortProduto(arq, p, q - 1, comparacoes);
        quickSortProduto(arq, q + 1, r, comparacoes);
    }
}

int particaoProduto(FILE *arq, int p, int r, int *comparacoes)
{

    TProduto pivo;
    TProduto *produto = (TProduto *)malloc(sizeof(TProduto));

    int i = p - 1;

    // Lê o pivô: registro na posição r
    fseek(arq, (r - 1) * tamanho_registroProduto(), SEEK_SET);
    produto = leProduto(arq);
    pivo = *produto; // copia o registro para 'pivo'
    free(produto);

    for (int j = p; j < r; j++)
    {

        TProduto *aj = (TProduto *)malloc(sizeof(TProduto));

        // Lê o registro j
        fseek(arq, (j - 1) * tamanho_registroProduto(), SEEK_SET);
        aj = leProduto(arq);

        (*comparacoes)++;

        // Compara pelo campo ID
        if (aj->id <= pivo.id)
        {

            i++;
            // Realiza a troca entre os registros nas posições i e j

            // Lê o registro na posição i
            fseek(arq, (i - 1) * tamanho_registroProduto(), SEEK_SET);
            TProduto *produto_i = leProduto(arq);

            // Escreve o registro j (aj) na posição i
            fseek(arq, (i - 1) * tamanho_registroProduto(), SEEK_SET);
            salvaProduto(aj, arq);

            // Escreve o registro que estava na posição i (produto_i) na posição j
            fseek(arq, (j - 1) * tamanho_registroProduto(), SEEK_SET);
            salvaProduto(produto_i, arq);

            free(produto_i);
        }
        free(aj);
    }

    // Troca o registro da posição i+1 com o pivô (registro r)
    fseek(arq, (i + 1 - 1) * tamanho_registroProduto(), SEEK_SET);
    TProduto *temp_ip1 = leProduto(arq);

    // Escreve o pivô na posição i+1
    fseek(arq, (i + 1 - 1) * tamanho_registroProduto(), SEEK_SET);
    salvaProduto(&pivo, arq);

    // Escreve o registro que estava em i+1 (temp_ip1) na posição r
    fseek(arq, (r - 1) * tamanho_registroProduto(), SEEK_SET);
    salvaProduto(temp_ip1, arq);

    free(temp_ip1);

    return i + 1;
}