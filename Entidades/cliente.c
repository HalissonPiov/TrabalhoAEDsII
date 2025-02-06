#ifndef CLIENTE_C
#define CLIENTE_C

#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "assinaturas.h"

int tamanho_registroCliente()
{
    return sizeof(int) + sizeof(char) * 50 + sizeof(char) * 100 + sizeof(char) * 40;
}

// Cria cliente.
TCliente *cliente(int id, char *nome, char *endereco, char *contato)
{
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));
    // inicializa espaco de memoria com ZEROS
    if (cliente)
        memset(cliente, 0, sizeof(TCliente));
    // copia valores para os campos de cliente
    cliente->id = id;
    strcpy(cliente->nome, nome);
    strcpy(cliente->endereco, endereco);
    strcpy(cliente->contato, contato);
    return cliente;
}

// Salva cliente no arquivo out, na posicao atual do cursor
void salvaCliente(TCliente *cliente, FILE *out)
{
    // fwrite(cliente, sizeof(TCliente), 1, out);
    // fflush(out);

    fwrite(&cliente->id, sizeof(int), 1, out); // VERIFICAR pq 1
    // cliente->nome ao inves de &cliente->nome, pois string ja eh um ponteiro
    fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), out);
    fwrite(cliente->endereco, sizeof(char), sizeof(cliente->endereco), out);
    fwrite(cliente->contato, sizeof(char), sizeof(cliente->contato), out);
}

// retorna a quantidade de registros no arquivo
int tamanho_arquivoCliente(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registroCliente());
    return tam;
}

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
TCliente *leCliente(FILE *in)
{
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));
    if (0 >= fread(&cliente->id, sizeof(int), 1, in))
    {
        free(cliente);
        return NULL;
    }
    fread(cliente->nome, sizeof(char), 50, in);
    fread(cliente->endereco, sizeof(char), 100, in);
    fread(cliente->contato, sizeof(char), 40, in);
    return cliente;
}

// Imprime cliente
void imprimirCliente(TCliente *cliente)
{
    printf("**********************************************");
    printf("\nCliente de ID ");
    printf("%d", cliente->id);
    printf("\nNome: ");
    printf("%s", cliente->nome);
    printf("\nEndereco: ");
    printf("%s", cliente->endereco);
    printf("\nContato: ");
    printf("%s", cliente->contato);
    printf("\n**********************************************");
}

// Cria a base de dados
void criarBaseCliente(FILE *out, int tam)
{
    // int *vet = (int *)malloc(tam * sizeof(int));
    int vet[tam];
    TCliente *c;

    for (int i = 0; i < tam; i++)
        vet[i] = i + 1;

    shuffleCliente(vet, tam);

    printf("\nGerando a base de dados...\n");

    for (int i = 0; i < tam; i++)
    {
        c = cliente(vet[i], "Anonimo", "Endereco Residencial", "emailanonimo@gmail.com");
        salvaCliente(c, out);
    }

    free(c);
}

// Cria a base de dados
void criarBaseClienteOrdenada(FILE *out, int tam)
{

    int vet[tam];
    TCliente *c;

    for (int i = 0; i < tam; i++)
        vet[i] = i + 1;

    printf("\nGerando a base de clientes...\n");

    for (int i = 0; i < tam; i++)
    {
        c = cliente(vet[i], "Anonimo", "Endereco Residencial", "emailanonimo@gmail.com");
        salvaCliente(c, out);
    }

    free(c);
}

void imprimirBaseCliente(FILE *out)
{
    printf("\nImprimindo a base de clientes...\n");

    rewind(out);
    TCliente *c = leCliente(out);

    if (c == NULL)
    {
        printf("Base de dados vazia\n");
        return;
    }

    while (c != NULL)
    {
        imprimirCliente(c);
        c = leCliente(out);
    }
    free(c);
}

void shuffleCliente(int *vet, int TAM)
{
    srand(time(NULL));
    for (int i = TAM - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int tmp = vet[j];
        vet[j] = vet[i];
        vet[i] = tmp;
    }
}

// int compara(TCliente *c1, TCliente *c2)
// {
//     if (c1 == NULL)
//     {
//         return (c2 == NULL);
//     }
//     if (c1->id != c2->id)
//     {
//         return 0;
//     }
//     if (strcmp(c1->nome, c2->nome) != 0)
//     {
//         return 0;
//     }
//     return 1;

#endif