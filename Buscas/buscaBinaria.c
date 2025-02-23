#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "buscaBinaria.h"


void salvarDadosBinaria(int comparacoes, double tempoExecucao)
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

TCliente *buscaBinariaCliente(FILE *out, int codigo)
{

    int comparacoes = 0;
    double tempoExecucao;
    int inicio = 0;
    fseek(out, 0, SEEK_END);
    int fim = ftell(out) / 194 - 1;
    clock_t temporizadorInicio = clock();

    while (inicio <= fim)
    {
        int meio = (inicio + fim) / 2;
        comparacoes++;

        fseek(out, meio * 194, SEEK_SET);
        TCliente *c = leCliente(out);

        if (c == NULL)
        {
            break;
        }

        if (c->id == codigo)
        {
            clock_t temporizadorFinal = clock();
            tempoExecucao = ((double)(temporizadorFinal - temporizadorInicio)) / CLOCKS_PER_SEC;
            salvarDadosBinaria(comparacoes, tempoExecucao);
            return c;
        }
        else if (c->id < codigo)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }

        free(c);
    }

    clock_t temporizadorFinal = clock();
    tempoExecucao = ((double)(temporizadorFinal - temporizadorInicio)) / CLOCKS_PER_SEC;
    salvarDadosBinaria(comparacoes, tempoExecucao);
    return NULL;
}


TProduto *buscaBinariaProduto(FILE *out, int codigo)
{

    int comparacoes = 0;
    double tempoExecucao;
    int inicio = 0;
    fseek(out, 0, SEEK_END);
    int fim = ftell(out) / tamanho_registroProduto() - 1;
    clock_t temporizadorInicio = clock();

    while (inicio <= fim)
    {
        int meio = (inicio + fim) / 2;
        comparacoes++;

        fseek(out, meio * tamanho_registroProduto(), SEEK_SET);
        TProduto *p = leProduto(out);

        if (p == NULL)
        {
            break;
        }

        if (p->id == codigo)
        {
            clock_t temporizadorFinal = clock();
            tempoExecucao = ((double)(temporizadorFinal - temporizadorInicio)) / CLOCKS_PER_SEC;
            salvarDadosBinaria(comparacoes, tempoExecucao);
            return p;
        }
        else if (p->id < codigo)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }

        free(p);
    }

    clock_t temporizadorFinal = clock();
    tempoExecucao = ((double)(temporizadorFinal - temporizadorInicio)) / CLOCKS_PER_SEC;
    salvarDadosBinaria(comparacoes, tempoExecucao);
    return NULL;
}


TPedido *buscaBinariaPedido(FILE *out, int codigo)
{

    int comparacoes = 0;
    double tempoExecucao;
    int inicio = 0;
    fseek(out, 0, SEEK_END);
    int fim = ftell(out) / tamanho_registroPedido() - 1;
    clock_t temporizadorInicio = clock();

    while (inicio <= fim)
    {
        int meio = (inicio + fim) / 2;
        comparacoes++;

        fseek(out, meio * tamanho_registroPedido(), SEEK_SET);
        TPedido *p = lePedido(out);

        if (p == NULL)
        {
            break;
        }

        if (p->id == codigo)
        {
            clock_t temporizadorFinal = clock();
            tempoExecucao = ((double)(temporizadorFinal - temporizadorInicio)) / CLOCKS_PER_SEC;
            salvarDadosBinaria(comparacoes, tempoExecucao);
            return p;
        }
        else if (p->id < codigo)
        {
            inicio = meio + 1;
        }
        else
        {
            fim = meio - 1;
        }

        free(p);
    }

    clock_t temporizadorFinal = clock();
    tempoExecucao = ((double)(temporizadorFinal - temporizadorInicio)) / CLOCKS_PER_SEC;
    salvarDadosBinaria(comparacoes, tempoExecucao);
    return NULL;
}