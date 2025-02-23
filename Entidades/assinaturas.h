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

TCliente *leCliente(FILE *in);

void imprimirCliente(TCliente *cliente);

void criarBaseCliente(FILE *out, int tam);

void criarBaseClienteOrdenada(FILE *out, int tam);

void imprimirBaseCliente(FILE *out);

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

TProduto *leProduto(FILE *in);

void imprimirProduto(TProduto *produto);

void criarBaseProduto(FILE *out, int tam);

void criarBaseProdutoOrdenada(FILE *out, int tam);

void imprimirBaseProduto(FILE *out);

void cadastrarProduto(FILE *arqProdutos);

void shuffleProduto(int *vet, int TAM);


// ##################################### DECLARACOES ENTIDADE PEDIDO #####################################

typedef struct Pedido
{
    int id;
    int idCliente;
    char nomeCliente[50];
    int idProduto;
    char nomeProduto[50];
    char status[30];
    double valorTotal;  
} TPedido;

int tamanho_registroPedido();

TPedido *pedido(int id, TCliente cliente, TProduto produto, char *status, double valorTotal); 

void salvaPedido(TPedido *pedido, FILE *out);

int tamanho_arquivoPedido(FILE *arq);

TPedido *lePedido(FILE *in);

void imprimirPedido(TPedido *pedido);

void imprimirBasePedido(FILE *out);

void relatorioVendas(FILE *arqPedidos);


#endif // ASSINATURAS_H_INCLUDED
