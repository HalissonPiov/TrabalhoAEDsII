#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <stdio.h>

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

TCliente *buscaSequencialClienteEditar(int chave, FILE *in)
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
        return comparacoes;
    }
    else
    {
        printf("Cliente nao encontrado");
        return NULL;
    }

    free(c);
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