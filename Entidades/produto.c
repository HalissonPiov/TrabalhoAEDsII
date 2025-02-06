#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "assinaturas.h"

int tamanho_registroProduto()
{
    return sizeof(int) + sizeof(char) * 50 + sizeof(char) * 20 + sizeof(double) + sizeof(int);
}

// Cria produto.
TProduto *produto(int id, char *nome, char *categoria, double preco, int estoque)
{
    TProduto *produto = (TProduto *)malloc(sizeof(TProduto));
    // inicializa espaco de memoria com ZEROS
    if (produto)
        memset(produto, 0, sizeof(TProduto));
    // copia valores para os campos de produto
    produto->id = id;
    strcpy(produto->nome, nome);
    strcpy(produto->categoria, categoria);
    produto->preco = preco;
    produto->estoque = estoque;
    return produto;
}

// Salva produto no arquivo out, na posicao atual do cursor
void salvaProduto(TProduto *produto, FILE *out)
{
    fwrite(&produto->id, sizeof(int), 1, out);
    // produto->nome ao inves de &produto->nome, pois string ja eh um ponteiro
    fwrite(produto->nome, sizeof(char), sizeof(produto->nome), out);
    fwrite(produto->categoria, sizeof(char), sizeof(produto->categoria), out);
    fwrite(&produto->preco, sizeof(double), 1, out);
    fwrite(&produto->estoque, sizeof(int), 1, out);
}

// retorna a quantidade de registros no arquivo
int tamanho_arquivoProduto(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registroProduto());
    return tam;
}

// Le um produto do arquivo in na posicao atual do cursor
// Retorna um ponteiro para produto lido do arquivo
TProduto *leProduto(FILE *in)
{
    TProduto *produto = (TProduto *)malloc(sizeof(TProduto));
    if (0 >= fread(&produto->id, sizeof(int), 1, in))
    { // o que isso faz
        free(produto);
        return NULL;
    }
    fread(produto->nome, sizeof(char), sizeof(produto->nome), in);
    fread(produto->categoria, sizeof(char), sizeof(produto->categoria), in);
    fread(&produto->preco, sizeof(double), 1, in);
    fread(&produto->estoque, sizeof(int), 1, in);
    return produto;
}

// Imprime produto
void imprimirProduto(TProduto *produto)
{
    printf("**********************************************");
    printf("\nProduto de ID ");
    printf("%d", produto->id);
    printf("\nNome: ");
    printf("%s", produto->nome);
    printf("\nCategoria: ");
    printf("%s", produto->categoria);
    printf("\nPreco: ");
    printf("%4.2f", produto->preco);
    printf("\nEstoque: ");
    printf("%d", produto->estoque);
    printf("\n**********************************************");
}

// Cria a base de dados
void criarBaseProduto(FILE *out, int tam)
{

    int vet[tam];
    TProduto *p;

    for (int i = 0; i < tam; i++)
        vet[i] = i + 1;

    shuffleProduto(vet, tam);

    printf("\nGerando a base de produtos...\n");

    for (int i = 0; i < tam; i++)
    {
        p = produto(vet[i], "Aleatorio", "Eletrodomestico", 1253 * (i + 1), 999);
        salvaProduto(p, out);
    }

    free(p);
}

// Cria a base de dados
void criarBaseProdutoOrdenada(FILE *out, int tam)
{

    int vet[tam], v = 0;
    TProduto *p;

    for (int i = 0; i < tam; i++)
        vet[i] = i + 1;

    printf("\nGerando a base de produtos...\n");

    for (int i = 0; i < tam; i++)
    {
        v++;
        p = produto(vet[i], "Aleatorio", "Eletrodomestico", 1253 * v, 999);
        salvaProduto(p, out);
    }

    free(p);
}

// // embaralha base de dados
// void shuffle(int *vet, int MAX, int MIN)
// {
//     srand(time(NULL));
//     for (int i = MAX - MIN - 1; i > 0; i--)
//     {
//         int j = rand() % (i);
//         int tmp = vet[j];
//         vet[j] = vet[i];
//         vet[i] = tmp;
//     }
// }

void imprimirBaseProduto(FILE *out)
{

    printf("\n\nImprimindo a base de produtos...\n");

    rewind(out);
    TProduto *p;

    while ((p = leProduto(out)) != NULL)
        imprimirProduto(p);

    free(p);
}

// int compara(TProduto *p1, TProduto *p2)
// {
//     if (p1 == NULL)
//     {
//         return (p2 == NULL);
//     }
//     if (p1->id != p2->id)
//     {
//         return 0;
//     }
//     if (strcmp(p1->nome, p2->nome) != 0)
//     {
//         return 0;
//     }
//     return 1;
// }

void shuffleProduto(int *vet, int TAM)
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