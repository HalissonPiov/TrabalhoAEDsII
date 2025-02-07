#ifndef ENTRADA_DADOS_H
#define ENTRADA_DADOS_H

// ## Criar uma const que define o tamanho total de ID para pedidos

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include "../Entidades/produto.c"

#include "entradaDados.h"
#include "../Buscas/buscaSequencial.h"

void cadastrarProduto(FILE *arqProdutos)
{

    int id;
    char nome[50];
    char categoria[20];
    double preco;
    int estoque;

    // arqProdutos = fopen("ArquivosDat/produto.dat", "a+b");

    printf("Digite o ID do produto: ");
    scanf("%d", &id);
    getchar();

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
    getchar();

    printf("Digite o estoque do produto: ");
    scanf("%d", &estoque);
    getchar();

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

void editarCliente(TCliente *cliente, FILE *arqClientes)
{

    while (getchar() != '\n')
    {
    };
    printf("\nInsira os novos dados do cliente\n");
    printf("Digite o nome do cliente: ");
    fgets(cliente->nome, 50, stdin);
    cliente->nome[strcspn(cliente->nome, "\n")] = '\0';

    printf("Digite o endereco do cliente: ");
    fgets(cliente->endereco, 100, stdin);
    cliente->endereco[strcspn(cliente->endereco, "\n")] = '\0';

    printf("Digite o contato do cliente: ");
    fgets(cliente->contato, 40, stdin);
    cliente->contato[strcspn(cliente->contato, "\n")] = '\0';

    int posicao = buscaSequencialClienteEditar(cliente->id, arqClientes);

    fseek(arqClientes, posicao * sizeof(TCliente), SEEK_CUR);
    salvaCliente(cliente, arqClientes);

    printf("\nCliente editado com sucesso!\n");
}

void excluirCliente(TCliente *cliente, FILE *arqClientes)
{

    fseek(arqClientes, -1 * sizeof(TCliente), SEEK_CUR);
    cliente->id = -1;
    strcpy(cliente->nome, "*");
    strcpy(cliente->endereco, "*");
    strcpy(cliente->contato, "*");
    salvaCliente(cliente, arqClientes);
}

void realizarPedido(TCliente *cliente, FILE *arqPedidos, FILE *arqProdutos)
{

    // int tamanho = tamanho_arquivoPedido(arqPedidos) + 1;

    int idProd, opcao;
    // int vet[tamanho];

    TProduto *produto = (TProduto *)malloc(sizeof(TProduto));

    // Identificação: coletar ID do produto na base de dados

    printf("Digite o ID do produto que deseja adquirir: ");
    scanf("%d", &idProd);
    getchar();
    produto = buscaSequencialProduto(idProd, arqProdutos);
    if (produto == NULL)
    {
        return;
    }

    // for (int i = 0; i < 100; i++)
    // {
    //     vet[i] = i + 1;
    // }

    TPedido *ped = pedido(tamanho_arquivoPedido(arqPedidos) + 1, *cliente, *produto, "Aguardando pagamento", produto->preco);

    imprimirProduto(produto);
    imprimirCliente(cliente);

    imprimirPedido(ped);

    fseek(arqPedidos, 0, SEEK_END);
    salvaPedido(ped, arqPedidos);

    // TPedido *pedido = (TPedido *)malloc(sizeof(TPedido));
    // pedido->id = tamanho_arquivoPedido(arqPedidos) + 1;
    // pedido->cliente = *cliente;
    // pedido->produto = *produto;
    // strcpy(pedido->status, "Aguardando pagamento");
    // pedido->valorTotal = produto->preco;

    // fseek(arqPedidos, 0, SEEK_END);
    // fwrite(pedido, sizeof(TPedido), 1, arqPedidos);

    printf("\nPedido realizado com sucesso!\n");

    imprimirBasePedido(arqPedidos);

    // printf("Cliente: %s\n", cliente->nome);
    // printf("Produto: %s\n", produto->nome);;
    // printf("Valor Total: %.2f\n", produto->preco);

    free(produto);
    free(ped);
}






// void realizarPedido(TCliente cli, FILE *arqProdutos, FILE *arqPedidos) // ## Se funcionar: passar a função para o arquivo pedido.c
// {

//     int idProd, opcao, quantProdutos = 1;
//     double valorTotalPedido;
//     TProduto *prod;
//     TPedido *ped;

//     // Identificação: coletar ID do produto na base de dados
//     while (1) //    ## VERIFICAR ESSE WHILE
//     {
//         printf("Digite o ID do produto que deseja adquirir: ");
//         scanf("%d", &idProd);
//         prod = buscaSequencial(idProd, arqProdutos); // tem que passar o arquivo de clientes como parâmetro
//         if (prod == NULL)
//         {
//             return;
//         }

//         valorTotalPedido += prod->preco;

//         ped = {// talvez precise de ser ponteiro
//                   ped->id = 1,    // ## Depois tem que mudar para gerar ID automaticamente
//                   ped->cliente = cli,
//                   ped->qtdProdutos = quantProdutos,
//                   ped->status = "Aguardando pagamento",
//                   ped->valorTotal = valorTotalPedido};

//         // prod = {
//         //     produtos->id,
//         //     produtos->nome,
//         //     produtos->categoria,
//         //     produtos->preco,
//         //     produtos->estoque};

//         fwrite(ped, sizeof(TPedido), 1, arqPedidos);                  // Escreve o pedido no arquivo
//         fwrite(prod, sizeof(TProduto), ped->qtdProdutos, arqPedidos); // Escreve os produtos

//         printf("Produto adicionado ao pedido!\n");
//         quantProdutos++;
//         fclose(arqPedidos); // ## VERIFICAR SE DEVE SER FECHADO AQUI

//         arqPedidos = fopen("pedidos.dat", "rb"); // "rb" para leitura

//         if (!arqPedidos)   // ## EXCLUIR ESSA PARTE DEPOIS
//         {
//             printf("Erro ao abrir o arquivo!\n");
//             return 1;
//         }

//         lerPedido(arqPedidos);

//         printf("Deseja adicionar mais produtos ao pedido? [1] Sim [2] Não\n");
//         scanf("%d", &opcao);

//         if (opcao == 1)
//         {
//             continue; // ## VERIFICAR SE FUNCIONA
//         }
//         else if (opcao == 2)
//         {
//             printf("Pedido realizado com sucesso!\n");
//             return;
//         }
//         else
//         {
//             printf("Opcao invalida. Tente novamente.\n");
//             printf("Processo de pedido cancelado.\n");
//             return;
//         }
//     }
// }

void efetuarPagamento(TPedido *ped, FILE *arqPedidos)
{

    int opcao;

    printf("Pedido encontrado:\n");
    imprimirPedido(ped);

    printf("\nConfirmar o pagamento do pedido no valor de R$ %.2f? [1] Sim [2] Não\n", ped->valorTotal);
    scanf("%d", &opcao);

    if (opcao == 1 && strcmp(ped->status, "Pagamento efetuado") != 0)
    {
        strcpy(ped->status, "Pagamento efetuado");
        printf("Pagamento efetuado com sucesso!\n");
    }
    else if (opcao == 2)
    {
        printf("Pagamento cancelado.\n");
        return;
    }
    else
    {
        printf("Opcao invalida. Tente novamente.\n");
        return;
    }

    // ## VERIFICAR SE ISSO SALVA CORRETAMENTE NO ARQUIVO
    fseek(arqPedidos, sizeof(TPedido), SEEK_CUR);
    fwrite(ped, sizeof(TPedido), 1, arqPedidos);
}

void cancelarPedido(TPedido *ped, FILE *arqPedidos)
{

    int opcao;

    printf("Pedido encontrado:\n");
    imprimirPedido(ped);

    printf("\nConfirmar cancelamento do pedido? [1] Sim [2] Não\n");
    scanf("%d", &opcao);

    if (opcao == 1)
    {
        strcpy(ped->status, "Pedido cancelado");
        printf("Pedido cancelado com sucesso!\n");
        // ## Ou deveria ser excluído do arquivo?
    }
    else if (opcao == 2)
    {
        printf("Cancelamento de pedido interrompido.\n");
        return;
    }
    else
    {
        printf("Opcao invalida. Tente novamente.\n");
        return;
    }

    // ## VERIFICAR SE ISSO SALVA CORRETAMENTE NO ARQUIVO
    fseek(arqPedidos, -sizeof(TPedido), SEEK_CUR);
    fwrite(ped, sizeof(TPedido), 1, arqPedidos);
}

#endif