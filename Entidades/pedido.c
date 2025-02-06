#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <time.h>

#include "assinaturas.h"

// Retorna tamanho do pedido em bytes
// int tamanho_registroPedido()
// {
//     return sizeof(int) + sizeof(TCliente) + sizeof(TProduto) + sizeof(char) * 20 + sizeof(double);
// }

int tamanho_registroPedido()
{
    // return sizeof(int) + tamanho_registroCliente() + tamanho_registroProduto() + sizeof(char) * 20 + sizeof(double);
    return sizeof(int) + sizeof(char) * 50 + sizeof(int) + sizeof(char) * 50 + sizeof(char) * 30 + sizeof(double);
}

// Cria pedido.
TPedido *pedido(int id, TCliente cliente, TProduto produto, char *status, double valorTotal) // verificar se produtos se define assim
{
    TPedido *pedido = (TPedido *)malloc(sizeof(TPedido));
    // inicializa espaco de memoria com ZEROS
    if (pedido)
        memset(pedido, 0, sizeof(TPedido));
    // copia valores para os campos de pedido
    pedido->id = id;
    // pedido->cliente = cliente;
    // pedido->produto = produto;
    strcpy(pedido->nomeCliente, cliente.nome);
    pedido->idCliente = cliente.id;
    strcpy(pedido->nomeProduto, produto.nome);
    pedido->idProduto = produto.id;
    strcpy(pedido->status, status);
    pedido->valorTotal = valorTotal;
    return pedido;
}

// Salva pedido no arquivo out, na posicao atual do cursor
void salvaPedido(TPedido *pedido, FILE *out)
{
    
    // fwrite(pedido, sizeof(TPedido), 1, out);
    fwrite(&pedido->id, sizeof(int), 1, out);
    // fwrite(&pedido->cliente, sizeof(TCliente), sizeof(&pedido->cliente), out);
    // fwrite(&pedido->produto, sizeof(TProduto), sizeof(&pedido->produto), out);

    fwrite(pedido->nomeCliente, sizeof(char), sizeof(pedido->nomeCliente), out);
    fwrite(&pedido->idCliente, sizeof(int), 1, out);
    fwrite(pedido->nomeProduto, sizeof(char), sizeof(pedido->nomeProduto), out);
    fwrite(&pedido->idProduto, sizeof(int), 1, out);

    // pedido->nome ao inves de &pedido->nome, pois string ja eh um ponteiro
    fwrite(pedido->status, sizeof(char), 30, out);
    fwrite(&pedido->valorTotal, sizeof(double), 1, out);
}

// retorna a quantidade de registros no arquivo
int tamanho_arquivoPedido(FILE *arq)
{
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho_registroPedido());
    rewind(arq);
    return tam;
}

// Le um pedido do arquivo in na posicao atual do cursor
// Retorna um ponteiro para pedido lido do arquivo
TPedido *lePedido(FILE *in)
{
    TPedido *pedido = (TPedido *)malloc(sizeof(TPedido));
    if (0 >= fread(&pedido->id, sizeof(int), 1, in))
    {
        free(pedido);
        return NULL;
    }
    // fread(&pedido->cliente, sizeof(TCliente), 1, in);
    // fread(&pedido->produto, sizeof(TProduto), 1, in);

    fread(pedido->nomeCliente, sizeof(char), 50, in);
    fread(&pedido->idCliente, sizeof(int), 1, in);
    fread(pedido->nomeProduto, sizeof(char), 50, in);
    fread(&pedido->idProduto, sizeof(int), 1, in);
    fread(&pedido->valorTotal, sizeof(int), 1, in);
    fread(pedido->status, sizeof(char)*30, sizeof(pedido->status), in);

    return pedido;
}

// void lerPedido(FILE *arqPedidos)
// {
//     TPedido pedido;

//     while (fread(&pedido, sizeof(TPedido), 1, arqPedidos))
//     {
//         printf("\nPedido ID: %d\n", pedido.id);
//         printf("Cliente: %s\n", pedido.cliente); // ## VERIFICAR: se é assim que se imprime um cliente (estrutura)
//         printf("Lista de produtos: %s\n", pedido.produto);
//         printf("Status: %s\n", pedido.status);
//         printf("Valor total: R$ %.2f\n", pedido.valorTotal);

//         // Aloca memória para armazenar os produtos temporariamente
//         // TProduto *produtos = (TProduto *)malloc(pedido.qtdProdutos * sizeof(TProduto));

//         // if (!produtos)
//         // {
//         //     printf("Erro ao alocar memória!\n");
//         //     return;
//         // }

//         // Lê os produtos do arquivo
//         fread(produtos, sizeof(TProduto), , arqPedidos);

//         // Exibe os produtos do pedido
//         for (int i = 0; i < pedido.qtdProdutos; i++)
//         {
//             printf(" - Produto ID: %s | Nome: %s | Categoria: %s | Preço: R$ %.2f | Estoque: %d |\n", , produtos[i].id, produtos[i].nome, produtos[i].categoria, produtos[i].preco, produtos[i].estoque);
//         }

//         free(produtos); // Libera a memória alocada
//     }
// }

// Imprime pedido
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
    printf("%4.2f", pedido->valorTotal);
    printf("\n**********************************************");

    // printf("**********************************************");
    // printf("\nPedido de ID: ");
    // printf("%d", pedido->id);
    // printf("\nNome do cliente: ");
    // printf("%s", pedido->cliente.nome);
    // printf("\nID do cliente: ");
    // printf("%d", pedido->cliente.id);
    // printf("\nProduto: ");
    // printf("%s", pedido->produto.nome);
    // printf("\nID do produto: ");
    // printf("%d", pedido->produto.id);
    // printf("\nStatus: ");
    // printf("%s", pedido->status);
    // printf("\nValor total: ");
    // printf("%4.2f", pedido->produto.preco);
    // printf("\n**********************************************");
}

// // Cria a base de dados      ## NÃO DEVE SER USADO PARA ENTIDADE PEDIDO
// void criarBase(FILE *out, int tam)
// {

//     int vet[tam];
//     TPedido *p;

//     for (int i = 0; i < tam; i++)
//         vet[i] = i + 1;

//     shuffle(vet, tam, (tam * 60) / 100);

//     printf("\nGerando a base de dados...\n");

//     for (int i = 0; i < tam; i++)
//     {
//         p = pedido(vet[i], "A", "000.000.000-00", "01/01/1980", 3000 * i);
//         salva(p, out);
//     }

//     free(p);
// }

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

// int compara(TPedido *p1, TPedido *p2)    ## NÃO DEVE SER USADO PARA ENTIDADE PEDIDO
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

void relatorioVendas(FILE *arqPedidos)
{

    // arqPedidos = fopen(&arqPedidos, "rb"); // Abrir arquivo para leitura binária
    if (arqPedidos == NULL)
    {
        printf("Erro ao abrir o arquivo %s\n", &arqPedidos);
        return;
    }

    TPedido pedido;
    double totalVendas = 0.0;
    int pedidosPagos = 0;

    printf("\n=== RELATÓRIO DE VENDAS ===\n");

    // Percorre o arquivo lendo os pedidos
    while (fread(&pedido, sizeof(TPedido), 1, arqPedidos))
    {
        // Verifica se o pedido está pago
        if (strcmp(pedido.status, "Pagamento realizado") == 0)
        {
            totalVendas += pedido.valorTotal;
            pedidosPagos++;

            // Exibe detalhes do pedido
            imprimirPedido(&pedido);
            // printf("Pedido ID: %d | Total: R$ %.2f\n", pedido.idPedido, pedido.valorTotal);
        }
    }

    // Exibe resumo final
    printf("\nTotal de pedidos pagos: %d\n", pedidosPagos);
    printf("Valor total das vendas: R$ %.2f\n", totalVendas);

    fclose(arqPedidos);
}
