#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>

#include "buscaSequencial.h"

void salvarDadosSequencial(int comparacoes, double tempoExecucao)
{
    FILE *arquivo = fopen("ArquivosLog/dadosBusca.txt", "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    // ----------------
    fprintf(arquivo, "Comparações: %d\n", comparacoes);
    fprintf(arquivo, "Tempo de execução: %f segundos\n", tempoExecucao);
    fclose(arquivo);
    // ----------------
}

TCliente *buscaSequencialCliente(int chave, FILE *in)
{

    TCliente *c;
    int achou = 0;
    rewind(in);
    int comparacoes = 0;
    clock_t temporizadorInicio = clock();

    while ((c = leCliente(in)) != NULL)
    {
        comparacoes++;

        if (c->id == chave)
        {
            achou = 1;
            break;
        }
    }

    clock_t temporizadorFim = clock();
    double tempoExecucao = ((double)(temporizadorFim - temporizadorInicio)) / CLOCKS_PER_SEC;

    if (achou == 1)
    {
        salvarDadosSequencial(comparacoes, tempoExecucao);
        return c;
    }
    else
    {
        salvarDadosSequencial(comparacoes, tempoExecucao);
        printf("Cliente nao encontrado");
        return NULL;
    }

    free(c);
}

int buscaSequencialPosicaoCliente(int chave, FILE *arquivo)
{
    clock_t clock(void);

    double TEMP_INICIAL, TEMP_FINAL;
    double TEMP_EXECUCAO;

    TCliente *clienteProcurado = NULL;

    int contador = 0;
    int posicao = 0;

    bool flag = false;

    TEMP_INICIAL = clock();

    rewind(arquivo);

    while ((clienteProcurado = leCliente(arquivo)) != NULL)
    {
        contador++;
        if (clienteProcurado->id == chave)
        {
            flag = true;
            break;
        }
        posicao++;
    }

    TEMP_FINAL = clock();
    TEMP_EXECUCAO = (TEMP_FINAL - TEMP_INICIAL) / CLOCKS_PER_SEC;

    if (flag)
    {
        return posicao;
    }
    else
    {
        return -1;
    }
}

TProduto *buscaSequencialProduto(int chave, FILE *in)
{

    TProduto *p;
    int achou = 0;
    rewind(in);
    int comparacoes = 0;
    clock_t temporizadorInicio = clock();

    while ((p = leProduto(in)) != NULL)
    {
        comparacoes++;

        if (p->id == chave)
        {
            achou = 1;
            break;
        }
    }

    clock_t temporizadorFim = clock();
    double tempoExecucao = ((double)(temporizadorFim - temporizadorInicio)) / CLOCKS_PER_SEC;

    if (achou == 1)
    {
        salvarDadosSequencial(comparacoes, tempoExecucao);
        return p;
    }
    else
    {
        salvarDadosSequencial(comparacoes, tempoExecucao);
        printf("\nProduto nao encontrado\n");
        return NULL;
    }
}

TPedido *buscaSequencialPedido(int chave, FILE *in)
{

    TPedido *p;
    int achou = 0;
    rewind(in);
    int comparacoes = 0;
    clock_t temporizadorInicio = clock();

    while ((p = lePedido(in)) != NULL)
    {
        comparacoes++;

        if (p->id == chave)
        {
            achou = 1;
            break;
        }
    }

    clock_t temporizadorFim = clock();
    double tempoExecucao = ((double)(temporizadorFim - temporizadorInicio)) / CLOCKS_PER_SEC;

    if (achou == 1)
    {
        salvarDadosSequencial(comparacoes, tempoExecucao);
        return p;
    }
    else
    {
        salvarDadosSequencial(comparacoes, tempoExecucao);
        printf("Pedido nao encontrado");
        return NULL;
    }
}