#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "entradaDados.h"
#include "../Buscas/buscaSequencial.h"
#include "../Ordenacao/quicksort.h"
#include "../GerenciamentoArquivos/tabelaHash.h"

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

    int id, posicao = 0;
    char nome[50];
    char endereco[100];
    char contato[40];

    getchar();

    printf("Digite o nome do cliente: ");
    fgets(nome, 50, stdin);
    nome[strcspn(nome, "\n")] = 0;

    printf("Digite o endereco do cliente: ");
    fgets(endereco, 100, stdin);
    endereco[strcspn(endereco, "\n")] = 0;

    printf("Digite o contato do cliente: ");
    fgets(contato, 40, stdin);
    contato[strcspn(contato, "\n")] = 0;

    id = tamanho_arquivoCliente(arqClientes) + 1;

    TCliente *c = cliente(id, nome, endereco, contato);

    printf("[1] Cadastrar cliente na tabela hash\n[2] Cadastrar cliente na base de dados\n");
    int opcao;
    scanf("%d", &opcao);

    if (opcao == 1)
    {
        inserirClienteHash(*c);
    }
    else if (opcao == 2)
    {

        rewind(arqClientes);
        TCliente *cli = leCliente(arqClientes);

        // Verificar se existe um cliente desabilitado e salvar no lugar dele
        while (cli != NULL)
        {
            posicao++;
            if (strcmp(cli->nome, "*") == 0 && strcmp(cli->endereco, "*") == 0 && strcmp(cli->contato, "*") == 0)
            {
                fseek(arqClientes, (posicao - 1) * tamanho_registroCliente(), SEEK_SET);
                // fseek(arqClientes, posicao * sizeof(TCliente), SEEK_SET);
                c->id = cli->id;
                salvaCliente(c, arqClientes);
                // fflush(arqClientes);
                // inserirClienteHash(*c);

                printf("\nCliente cadastrado com sucesso!\n");
                imprimirCliente(c);

                free(c);
                free(cli);
                return;
            }
            free(cli);
            cli = leCliente(arqClientes);
        }

        fseek(arqClientes, 0, SEEK_END);
        salvaCliente(c, arqClientes);
    }
    else
    {
        printf("Opcao invalida. Tente novamente.\n");
        free(c);
        return;
    }

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

void excluirCliente(int id)
{
    int posicao = 0;
    TCliente *cli;

    FILE *arqClientes = fopen("ArquivosDat/cliente.dat", "r+b");
    if (arqClientes == NULL)
    {
        printf("Erro ao abrir o arquivo de clientes.\n");
        return;
    }

    rewind(arqClientes);

    while ((cli = leCliente(arqClientes)) != NULL)
    {
        posicao++;
        if (cli->id == id)
        {
            // Marca o cliente como excluído na base de dados
            strcpy(cli->nome, "*");
            strcpy(cli->endereco, "*");
            strcpy(cli->contato, "*");

            fseek(arqClientes, (posicao - 1) * tamanho_registroCliente(), SEEK_SET);
            salvaCliente(cli, arqClientes);

            printf("Cliente com ID %d excluído na base de dados.\n", cli->id);

            free(cli);
            fclose(arqClientes);
            return;
        }
        free(cli);
    }

    printf("Cliente com ID %d não encontrado na base de dados.\n", id);

    fclose(arqClientes);
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

void realizarOrdenacaoCliente(FILE *arquivoClientes)
{
    int comparacoes = 0;
    clock_t inicio = clock();
    int op;

    quickSortCliente(arquivoClientes, 1, tamanho_arquivoCliente(arquivoClientes), &comparacoes);

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\nBase de dados de clientes ordenada com sucesso!\n");
    imprimirBaseCliente(arquivoClientes);

    salvarDadosQuickSort(comparacoes, tempo);
}

void realizarOrdenacaoProduto(FILE *arquivoProdutos)
{
    int comparacoes = 0;
    clock_t inicio = clock();
    int op;

    quickSortProduto(arquivoProdutos, 1, tamanho_arquivoProduto(arquivoProdutos), &comparacoes);

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("\nBase de dados de produtos ordenada com sucesso!\n");
    imprimirBaseProduto(arquivoProdutos);

    salvarDadosQuickSort(comparacoes, tempo);
}

void realizarGerenciamentoBaseClientes(FILE *arqClientes)
{

    inicializarTabelaHashVazia();

    printf("Tabela hash inicializada com sucesso!\n");
    exibirTabelaHash();

    rewind(arqClientes);
    TCliente *c = leCliente(arqClientes);

    if (c == NULL)
    {
        printf("Base de dados vazia\n");
        return;
    }

    while (c != NULL)
    {
        inserirClienteHash(*c);
        c = leCliente(arqClientes);
    }

    free(c);

    printf("Base de dados de clientes inserida com sucesso na tabela hash para ser gerenciada!\n");
}
