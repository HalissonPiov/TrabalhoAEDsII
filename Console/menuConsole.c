#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include "entradaDados.c"
// #include "../Buscas/buscaSequencial.c"
// #include "../Buscas/buscaBinaria.c"

#include "../Entidades/assinaturas.h"
#include "../Buscas/buscaSequencial.h"
#include "../Buscas/buscaBinaria.h"
#include "entradaDados.h"
// #include "../Entidades/cliente.c"
// #include "../Entidades/produto.c"
// #include "../Entidades/pedido.c"

// void abreArquivos(FILE **arqClientes, FILE **arqProdutos, FILE **arqPedidos)
// {

//     *arqClientes = fopen("cliente.dat", "w+b");
//     *arqProdutos = fopen("produto.dat", "w+b");
//     *arqPedidos = fopen("pedido.dat", "w+b");

//     if (*arqClientes == NULL || *arqProdutos == NULL || *arqPedidos == NULL)
//     {
//         printf("Erro ao abrir um dos arquivos\n");
//         exit(1);
//     }
// }

void exibirMenuPrincipal(FILE *arqClientes, FILE *arqProdutos, FILE *arqPedidos)
{

    int opcao, subOpcao, escolha;
    int idC, idPro, idPed, quantProdutos = 0;
    // TCliente *cli;
    // TPedido *ped;
    // TProduto *prod;
    TCliente *cli = (TCliente *)malloc(sizeof(TCliente));
    TPedido *ped = (TPedido *)malloc(sizeof(TPedido));
    TProduto *prod = (TProduto *)malloc(sizeof(TProduto));

    while (1)
    {

        printf("\nSelecione uma opcao:\n[1] Realizar um pedido\n[2] Operacoes\n[3] Gerenciar pedido\n[4] Realizar pesquisa na base de dados\n[5] Imprimir bases de dados\n[6] Gerar relatorio de vendas\n[7] Fechar programa\n");
        scanf("%d", &opcao);

        switch (opcao)
        {
        case 1:
            printf("Selecione qual cliente vai realizar o pedido: \n");
            printf("[1] Selecionar cliente por ID\n[2] Cadastrar um novo cliente\n[3] Voltar\n");
            scanf("%d", &subOpcao);

            if (subOpcao == 1)
            {
                printf("Digite o ID do cliente: ");
                scanf("%d", &idC);
                cli = buscaSequencialCliente(idC, arqClientes);
                if (cli == NULL)
                {
                    break;
                }

                realizarPedido(cli, arqPedidos, arqProdutos);
                break;
            }
            else if (subOpcao == 2)
            {
                cadastrarCliente(arqClientes);
                realizarPedido(cli, arqProdutos, arqPedidos);
                break;
            }
            else if (subOpcao == 3)
            {
                break;
            }
            else
            {
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }

        case 2:
            subOpcao = 0;
            printf("[1] Inserir novo cliente\n[2] Inserir novo produto\n[3] Editar cliente\n[4] Excluir cliente\n[5] Sair\n");
            scanf("%d", &subOpcao);

            switch (subOpcao)
            {
            case 1:
                cadastrarCliente(arqClientes);
                break;
            case 2:
                cadastrarProduto(arqProdutos);
                break;
            case 3:{
                printf("Digite o ID do cliente: ");
                scanf("%d", &idC);
                cli = buscaSequencialCliente(idC, arqClientes);
                if (cli == NULL)
                {
                    break;
                }

                editarCliente(cli, arqClientes);

                break;
                }
            case 4:

                printf("Digite o ID do cliente: ");
                scanf("%d", &idC);
                cli = buscaSequencialCliente(idC, arqClientes);
                if (cli == NULL)
                {
                    break;
                }

                excluirCliente(cli, arqClientes);

                break;
            case 5:

                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }

            break;

        case 3:
            escolha = 0;
            printf("[1] Efetuar pagamento do pedido\n[2] Cancelar pedido\n[3] Consultar estado do pedido por ID\n[4] Voltar\n");
            scanf("%d", &escolha);

            switch (escolha)
            {
            case 1:

                printf("Digite o ID do pedido: ");
                scanf("%d", &idPed);
                ped = buscaSequencialPedido(idPed, arqPedidos); // tem que passar o arquivo de pedidos como parâmetro
                if (ped == NULL)
                {
                    break;
                }

                efetuarPagamento(ped, arqPedidos); // tem que passar o arquivo de pedidos como parâmetro  ## TEM QUE IMPLEMENTAR

                break;
            case 2:

                printf("Digite o ID do pedido: ");
                scanf("%d", &idPed);
                ped = buscaSequencialPedido(idPed, arqPedidos); // tem que passar o arquivo de pedidos como parâmetro
                if (ped == NULL)
                {
                    break;
                }

                cancelarPedido(ped, arqPedidos); // tem que passar o arquivo de pedidos como parâmetro ## TEM QUE IMPLEMENTAR

                break;

            case 3:

                printf("Digite o ID do pedido: ");
                scanf("%d", &idPed);
                ped = buscaSequencialPedido(idPed, arqPedidos); // tem que passar o arquivo de pedidos como parâmetro ## TEM QUE IMPLEMENTAR
                if (ped == NULL)
                {
                    break;
                }

                printf("Status do pedido: %s\n", ped->status);

                break;

            case 4:
                break;

            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }

            break;
        case 4:

            opcao = 0;
            printf("[1] Realizar busca sequencial\n[2] Realizar busca binaria\n[3] Voltar\n");
            scanf("%d", &opcao);
            // printf("[1] Realizar busca por ID\n[2] Realizar busca Sequencial\n[3] Realizar busca binaria\n[4] Voltar\n"); // Adicionar opção de pesquisa por status do pedido

            if (opcao == 1)
            {
                escolha = 0;
                printf("Busca Sequencial: \n");
                printf("[1] Realizar busca por cliente\n[2] Realizar busca por produto\n[3] Realizar busca por pedido\n[4] Voltar\n");
                scanf("%d", &escolha);

                switch (escolha)
                {
                case 1:

                    printf("Digite o ID do cliente: ");
                    scanf("%d", &idC);
                    cli = buscaSequencialCliente(idC, arqClientes);
                    if (cli == NULL)
                    {
                        break;
                    }

                    imprimirCliente(cli);

                    break;
                case 2:

                    printf("Digite o ID do produto: ");
                    scanf("%d", &idPro);
                    prod = buscaSequencialProduto(idPro, arqProdutos);
                    if (prod == NULL)
                    {
                        break; // ## VERIFICAR se deve ser um return mesmo
                    }

                    imprimirProduto(prod);

                    break;
                case 3:

                    printf("Digite o ID do pedido: ");
                    scanf("%d", &idPed);
                    ped = buscaSequencialPedido(idPed, arqPedidos);
                    if (ped == NULL)
                    {
                        break;
                    }

                    imprimirPedido(ped);

                    break;

                case 4:
                    break;

                default:
                    printf("Opcao invalida. Tente novamente.\n");
                    break;
                }
            }
            else if (opcao == 2)
            {
                subOpcao = 0;
                printf("Busca Binaria: \n");
                printf("[1] Realizar busca por cliente\n[2] Realizar busca por produto\n[3] Realizar busca por pedido\n[4] Voltar\n");
                scanf("%d", &subOpcao);

                switch (subOpcao)
                {
                case 1:

                    printf("Digite o ID do cliente: ");
                    scanf("%d", &idC);
                    cli = buscaBinariaCliente(arqClientes, idC);
                    if (cli == NULL)
                    {
                        break;
                    }

                    imprimirCliente(cli);

                    break;
                case 2:

                    printf("Digite o ID do produto: ");
                    scanf("%d", &idPro);
                    prod = buscaBinariaProduto(arqProdutos, idPro);
                    if (prod == NULL)
                    {
                        break;
                    }

                    imprimirProduto(prod);

                    break;
                case 3:

                    printf("Digite o ID do pedido: ");
                    scanf("%d", &idPed);
                    ped = buscaBinariaPedido(arqPedidos, idC);
                    if (ped == NULL)
                    {
                        break;
                    }

                    imprimirPedido(ped);

                    break;

                case 4:
                    break;

                default:
                    printf("Opcao invalida. Tente novamente.\n");
                    break;
                }
            }
            else
            {
                break;
            }

            break;
        case 5:
            subOpcao = 0;
            printf("[1] Imprimir base de dados de clientes\n[2] Imprimir base de dados de produtos\n[3] Imprimir base de dados de pedidos\n[4] Voltar\n");
            scanf("%d", &subOpcao);
            switch (subOpcao)
            {
            case 1:
                imprimirBaseCliente(arqClientes);
                break;
            case 2:
                imprimirBaseProduto(arqProdutos);
                break;
            case 3:
                imprimirBasePedido(arqPedidos);
                break;
            case 4:
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
            }
            break;

        case 6:

            relatorioVendas(arqPedidos);

            break;
        case 7:
            printf("Fechando programa...\n");
            return;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            break;
        }
    }
}