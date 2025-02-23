#ifndef ENTRADA_DADOS_H
#define ENTRADA_DADOS_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "entradaDados.h"
#include "../Buscas/buscaSequencial.h"

void cadastrarProduto(FILE *arqProdutos)
{

    int id;
    char nome[50];
    char categoria[20];
    double preco;
    int estoque;

    printf("Digite o ID do produto: ");
    scanf("%d", &id);
    fflush(stdin);

    if (buscaSequencialProduto(id, arqProdutos) == NULL)
    {
        printf("ID de produto pode ser utilizado\n\n");
    }

    else
    {
        printf("ID de produto ja cadastrado!\n");
        return;
    }

    printf("Digite o nome do produto: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite a categoria do produto: ");
    fgets(categoria, 20, stdin);
    categoria[strcspn(categoria, "\n")] = 0;

    printf("Digite o preco do produto: ");
    scanf("%lf", &preco);

    printf("Digite o estoque do produto: ");
    scanf("%d", &estoque);

    TProduto *p = produto(id, nome, categoria, preco, estoque);
    fseek(arqProdutos, 0, SEEK_END);
    salvaProduto(p, arqProdutos);

    printf("\nProduto cadastrado com sucesso!\n\n");
    imprimirProduto(p);

    free(p);
}

void cadastrarCliente(FILE *arqClientes)
{

    int id;
    char nome[50];
    char endereco[100];
    char contato[40];

    printf("Digite o ID do cliente: ");
    scanf("%d", &id);
    getchar();

    if (buscaSequencialCliente(id, arqClientes) == NULL)
    {
        printf("\nID de cliente pode ser utilizado\n\n");
    }

    else
    {
        printf("ID de cliente ja cadastrado!\n");
        return;
    }

    printf("Digite o nome do cliente: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite o endereco do cliente: ");
    fgets(endereco, 100, stdin);
    endereco[strcspn(endereco, "\n")] = 0;

    printf("Digite o contato do cliente: ");
    fgets(contato, 40, stdin);
    contato[strcspn(contato, "\n")] = 0;

    TCliente *c = cliente(id, nome, endereco, contato);
    fseek(arqClientes, 0, SEEK_END);
    salvaCliente(c, arqClientes);

    printf("\nCliente cadastrado com sucesso!\n\n");
    imprimirCliente(c);

    free(c);
}

void editarCliente(TCliente *client, FILE *arqClientes)
{

    int id;
    char nome[50];
    char endereco[100];
    char contato[40];

    printf("\nInsira os novos dados do cliente\n");
    printf("Digite o nome do cliente: ");
    fflush(stdin);
    fgets(nome, 50, stdin);
    // nome[strcspn(nome, "\n")] = 0;

    printf("Digite o endereco do cliente: ");
    fflush(stdin);
    fgets(endereco, 100, stdin);

    printf("Digite o contato do cliente: ");
    fflush(stdin);
    fgets(contato, 40, stdin);

    int posicao = (buscaSequencialPosicaoCliente(client->id, arqClientes));

    if (posicao == -1)
    {
        printf("Posicao de cliente nao encontrado\n");
        return;
    }
    
    TCliente *cli = cliente(client->id, nome, endereco, contato);

    printf("DEBUG -> Posicao: %d\n", posicao);

    fseek(arqClientes, 194, SEEK_SET);

    salvaCliente(cli, arqClientes);

    printf("DEBUG ->>>>");
    imprimirCliente(cli);

    printf("\nCliente editado com sucesso!\n");
}

void excluirCliente(TCliente *cliente, FILE *arqClientes)
{
    int idCliente = cliente->id;

    arqClientes = fopen("C:\\Users\\halis\\Desktop\\halisson\\halisson\\TrabalhoAEDsII\\ArquivosDat\\cliente.dat", "rb+");
    if (arqClientes == NULL)
    {
        perror("Erro ao abrir arquivo");
    }

    fseek(arqClientes, idCliente * 194, SEEK_SET);
    cliente->id = cliente->id;
    strcpy(cliente->nome, "*");
    strcpy(cliente->endereco, "*");
    strcpy(cliente->contato, "*");

    fwrite(&cliente->id, sizeof(int), 1, arqClientes);
    fwrite(cliente->nome, sizeof(char), sizeof(cliente->nome), arqClientes);
    fwrite(cliente->endereco, sizeof(char), sizeof(cliente->endereco), arqClientes);
    fwrite(cliente->contato, sizeof(char), sizeof(cliente->contato), arqClientes);

    fflush(arqClientes);

    printf("\nCliente desabilitado com sucesso!\n");
}

void realizarPedido(TCliente *cliente, FILE *arqPedidos, FILE *arqProdutos)
{

    int idProd, opcao;

    TProduto *produto = (TProduto *)malloc(sizeof(TProduto));

    printf("Digite o ID do produto que deseja adquirir: ");
    scanf("%d", &idProd);
    getchar();
    produto = buscaSequencialProduto(idProd, arqProdutos);
    if (produto == NULL)
    {
        return;
    }

    TPedido *ped = pedido(tamanho_arquivoPedido(arqPedidos) + 1, *cliente, *produto, "Pedido confirmado", produto->preco);

    printf("\nCliente selecionado para realizar o pedido:\n");
    imprimirCliente(cliente);
    printf("\n\nProduto selecionado para realizar o pedido:\n");
    imprimirProduto(produto);

    printf("\n\nPedido realizado com sucesso!\n");

    fseek(arqPedidos, 0, SEEK_END);
    salvaPedido(ped, arqPedidos);

    printf("\nInserido na base de pedidos\n");
    imprimirBasePedido(arqPedidos);

    free(produto);
    free(ped);
}

// void efetuarPagamento(FILE *arqPedidos)
// {

//     int opcao, idPed;
//     TPedido *pedido = (TPedido *)malloc(194);

//     printf("Digite o ID do pedido: ");
//     scanf("%d", &idPed);
//     pedido = buscaSequencialPedido(idPed, arqPedidos); // tem que passar o arquivo de pedidos como parâmetro
//     if (pedido == NULL)
//     {
//         return;
//     }

//     printf("Pedido encontrado:\n");
//     imprimirPedido(pedido);

//     printf("\nConfirmar o pagamento do pedido no valor de R$ %.2f? [1] Sim [2] Não\n", pedido->valorTotal);
//     scanf("%d", &opcao);

//     if (opcao == 1)
//     {
//         strcpy(pedido->status, "Pagamento realizado");
//         printf("Pagamento efetuado com sucesso!\n");
//     }
//     else if (opcao == 2)
//     {
//         printf("Pagamento cancelado.\n");
//         return;
//     }
//     else
//     {
//         printf("Opcao invalida. Tente novamente.\n");
//         return;
//     }

//     // ## VERIFICAR SE ISSO SALVA CORRETAMENTE NO ARQUIVO
//     fseek(arqPedidos, 194, SEEK_CUR);
//     fwrite(pedido->status, sizeof(char), 30, arqPedidos);
// }

// void cancelarPedido(TPedido *ped, FILE *arqPedidos)
// {

//     int opcao;

//     printf("Pedido encontrado:\n");
//     imprimirPedido(ped);

//     printf("\nConfirmar cancelamento do pedido? [1] Sim [2] Não\n");
//     scanf("%d", &opcao);

//     if (opcao == 1)
//     {
//         strcpy(ped->status, "Pedido cancelado");
//         printf("Pedido cancelado com sucesso!\n");
//         // ## Ou deveria ser excluído do arquivo?
//     }
//     else if (opcao == 2)
//     {
//         printf("Cancelamento de pedido interrompido.\n");
//         return;
//     }
//     else
//     {
//         printf("Opcao invalida. Tente novamente.\n");
//         return;
//     }

//     // ## VERIFICAR SE ISSO SALVA CORRETAMENTE NO ARQUIVO
//     fseek(arqPedidos, -sizeof(TPedido), SEEK_CUR);
//     fwrite(ped, sizeof(TPedido), 1, arqPedidos);
// }

#endif