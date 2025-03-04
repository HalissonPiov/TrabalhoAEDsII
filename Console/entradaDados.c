#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "entradaDados.h"
#include "../Buscas/buscaSequencial.h"
#include "../Ordenacao/quicksort.h"


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

    arqClientes = fopen("C:\\Users\\halis\\Desktop\\TP-AEDsII\\halissonAtualizado\\TrabalhoAEDsII\\ArquivosDat\\cliente.dat", "rb+");
    if (arqClientes == NULL)
    {
        perror("Erro ao abrir arquivo");
    }

    fseek(arqClientes, (idCliente - 1) * tamanho_registroCliente(), SEEK_SET);
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

    fclose(arqClientes);

    // arqClientes = fopen("C:\\Users\\halis\\Desktop\\TP-AEDsII\\halissonAtualizado\\TrabalhoAEDsII\\ArquivosDat\\cliente.dat", "w+b");
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

void realizarOrdenacao(FILE *arquivoClientes)
{
    int comparacoes = 0;
    clock_t inicio = clock();
    int op;

    printf("\n\nMetodo de ordenacao em disco: QuickSort\n");
    printf("[1] Ordenar base de dados de clientes\n[2] Continuar sem ordenar\n");
    scanf("%d", &op);

    if (op == 1)
    {
        quickSort(arquivoClientes, 1, tamanho_arquivoCliente(arquivoClientes), &comparacoes);
    }
    else if (op == 2)
    {
        return;
    }
    else
    {
        printf("Opcao invalida. Tente novamente.\n");
        return;
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\nBase de dados de clientes ordenada com sucesso!\n");
    imprimirBaseCliente(arquivoClientes);

    salvarDadosQuickSort(comparacoes, tempo);

}

