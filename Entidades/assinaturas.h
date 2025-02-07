#ifndef ASSINATURAS_H_INCLUDED  
#define ASSINATURAS_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


// ##################################### DECLARACOES ENTIDADE CLIENTE #####################################

typedef struct Cliente
{
    int id;
    char nome[50];
    char endereco[100];
    char contato[40];
    int posicao;
} TCliente;

int tamanho_registroCliente();

TCliente *cliente(int id, char *nome, char *endereco, char *contato);

void salvaCliente(TCliente *cliente, FILE *out);

// Le um cliente do arquivo in na posicao atual do cursor
// Retorna um ponteiro para cliente lido do arquivo
TCliente *leCliente(FILE *in);

void imprimirCliente(TCliente *cliente);

void criarBaseCliente(FILE *out, int tam);

void criarBaseClienteOrdenada(FILE *out, int tam);

// // embaralha base de dados
// void shuffle(int *vet, int MAX, int MIN); // ---> precisa ser revisado ainda

// Imprime a base de dados
void imprimirBaseCliente(FILE *out);

// // Compara dois cliente
// // Retorna 1 se os valores dos atributos de ambos forem iguais
// // e 0 caso contrario
// int compara(TCliente *c1, TCliente *c2);

int tamanho_arquivoCliente(FILE *arq);

void shuffleCliente(int *vet, int TAM);

// ##################################### DECLARACOES ENTIDADE PRODUTO #####################################

typedef struct Produto
{
    int id;
    char nome[50];
    char categoria[20];
    double preco;
    int estoque;
} TProduto;

int tamanho_registroProduto();

TProduto *produto(int id, char *nome, char *categoria, double preco, int estoque);

void salvaProduto(TProduto *produto, FILE *out);

int tamanho_arquivoProduto(FILE *arq);

// Le um produto do arquivo in na posicao atual do cursor
// Retorna um ponteiro para produto lido do arquivo
TProduto *leProduto(FILE *in);

void imprimirProduto(TProduto *produto);

void criarBaseProduto(FILE *out, int tam);

void criarBaseProdutoOrdenada(FILE *out, int tam);

// Imprime a base de dados
void imprimirBaseProduto(FILE *out);


// // Compara dois produtos
// // Retorna 1 se os valores dos atributos de ambos forem iguais
// // e 0 caso contrario
// int compara(TProduto *p1, TProduto *p2);

void cadastrarProduto(FILE *arqProdutos);

void shuffleProduto(int *vet, int TAM);


// ##################################### DECLARACOES ENTIDADE PEDIDO #####################################

typedef struct Pedido
{
    int id;
    // TCliente cliente;    // ## Mudança que precisa ser feita nas funções que envolvam a entidade Pedido
    int idCliente;
    char nomeCliente[50];
    int idProduto;
    char nomeProduto[50];
    // TProduto produto;
    char status[30];
    double valorTotal;  
} TPedido;

// Retorna tamanho do pedido em bytes
int tamanho_registroPedido();

// Cria pedido.
TPedido *pedido(int id, TCliente cliente, TProduto produto, char *status, double valorTotal); 

// Salva pedido no arquivo out, na posicao atual do cursor
void salvaPedido(TPedido *pedido, FILE *out);

// retorna a quantidade de registros no arquivo
int tamanho_arquivoPedido(FILE *arq);

// Le um pedido do arquivo in na posicao atual do cursor
// Retorna um ponteiro para pedido lido do arquivo
TPedido *lePedido(FILE *in);

// void lerPedido(FILE *arqPedidos);

// Imprime pedido
void imprimirPedido(TPedido *pedido);

// Imprime a base de dados
void imprimirBasePedido(FILE *out);

// Compara dois pedidos
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
// int compara(TPedido *p1, TPedido *p2);

void relatorioVendas(FILE *arqPedidos);


#endif // ASSINATURAS_H_INCLUDED
