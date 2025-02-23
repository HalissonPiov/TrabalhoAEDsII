#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "assinaturas.h"


int tamanho_registroPedido()
{
    return sizeof(int) + sizeof(int) + sizeof(char) * 50 + sizeof(int) + sizeof(char) * 50 + sizeof(char) * 30 + sizeof(double);
}

TPedido *pedido(int id, TCliente cliente, TProduto produto, char *status, double valorTotal) 
{
    TPedido *pedido = (TPedido *)malloc(sizeof(TPedido));
    if (pedido)
        memset(pedido, 0, sizeof(TPedido));
    pedido->id = id;
    strcpy(pedido->nomeCliente, cliente.nome);
    pedido->idCliente = cliente.id;
    strcpy(pedido->nomeProduto, produto.nome);
    pedido->idProduto = produto.id;
    strcpy(pedido->status, status);
    pedido->valorTotal = valorTotal;
    return pedido;
}

void salvaPedido(TPedido *pedido, FILE *out)
{
    
    fwrite(&pedido->id, sizeof(int), 1, out);
    fwrite(pedido->nomeCliente, sizeof(char), sizeof(pedido->nomeCliente), out);
    fwrite(&pedido->idCliente, sizeof(int), 1, out);
    fwrite(pedido->nomeProduto, sizeof(char), sizeof(pedido->nomeProduto), out);
    fwrite(&pedido->idProduto, sizeof(int), 1, out);
    fwrite(pedido->status, sizeof(char), 30, out);
    fwrite(&pedido->valorTotal, sizeof(double), 1, out);
}

int tamanho_arquivoPedido(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registroPedido());
    rewind(arq);
    return tam;
}

TPedido *lePedido(FILE *in)
{
    TPedido *pedido = (TPedido *)malloc(sizeof(TPedido));
    if (0 >= fread(&pedido->id, sizeof(int), 1, in))
    {
        free(pedido);
        return NULL;
    }
    fread(pedido->nomeCliente, sizeof(char), 50, in);
    fread(&pedido->idCliente, sizeof(int), 1, in);
    fread(pedido->nomeProduto, sizeof(char), 50, in);
    fread(&pedido->idProduto, sizeof(int), 1, in);
    fread(pedido->status, sizeof(char), 30, in);
    fread(&pedido->valorTotal, sizeof(double), 1, in);

    return pedido;
}

void imprimirPedido(TPedido *pedido)
{

    printf("**********************************************");
    printf("\nPedido de ID: ");
    printf("%d", pedido->id);
    printf("\nNome do cliente: ");
    printf("%s", pedido->nomeCliente);
    printf("\nID do cliente: ");
    printf("%d", pedido->idCliente);
    printf("\nProduto: ");
    printf("%s", pedido->nomeProduto);
    printf("\nID do produto: ");
    printf("%d", pedido->idProduto);
    printf("\nStatus: ");
    printf("%s", pedido->status);
    printf("\nValor total: ");
    printf("%.2f", pedido->valorTotal);
    printf("\n**********************************************");
}

void imprimirBasePedido(FILE *out)
{

    printf("Imprimindo a base de pedidos...\n");

    rewind(out);
    TPedido *p = NULL;

    while ((p = lePedido(out)) != NULL){
        imprimirPedido(p);
    }

    free(p);
}


void relatorioVendas(FILE *arqPedidos)
{

    fflush(arqPedidos);

    TPedido *pedido = (TPedido *)malloc(sizeof(TPedido));
    double totalVendas = 0.0;
    int pedidosPagos = 0;

    printf("\n=== Relatorio de vendas ===\n");

    while (fread(&pedido, 194, 1, arqPedidos))
    {
        
            totalVendas += pedido->valorTotal;
            pedidosPagos++;

            imprimirPedido(pedido);
        }

    printf("\nTotal de pedidos pagos: %d\n", pedidosPagos);
    printf("Valor total das vendas: R$ %.2f\n", totalVendas);
    
}