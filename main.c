// #include "Entidades/assinaturas.h"
#include "Console/entradaDados.h"
#include "Console/menuConsole.h"

#include "Console/menuConsole.c"
#include "Entidades/pedido.c"
#include "Entidades/produto.c"
#include "Entidades/cliente.c"
#include "Buscas/buscaSequencial.c"
#include "Buscas/buscaBinaria.c"
#include "Console/entradaDados.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // Abrir os arquivos de entidades
    FILE *arqClientes = fopen("ArquivosDat/cliente.dat", "w+b");
    FILE *arqProdutos = fopen("ArquivosDat/produto.dat", "w+b");
    FILE *arqPedidos = fopen("ArquivosDat/pedido.dat", "w+b");

    if (arqClientes == NULL || arqProdutos == NULL || arqPedidos == NULL)
    {
        printf("Erro ao abrir um dos arquivos\n");
        exit(1);
    }

    // abreArquivos(arqClientes, arqProdutos, arqPedidos);
    int op;

    printf("\n----------------------------------------------------------------\n");
    printf("-- Sistema de Controle de Pedidos: Plataforma de E-Commerce --\n");
    printf("----------------------------------------------------------------\n");

    printf("\nSelecione uma opcao: \n[1] Criar base ordenada\n[2] Criar base desordenada\n");
    scanf("%d", &op);

    if (op == 1)
    {
        criarBaseClienteOrdenada(arqClientes, 10);
        criarBaseProdutoOrdenada(arqProdutos, 12);
        imprimirBaseCliente(arqClientes);
        imprimirBaseProduto(arqProdutos);
    }
    else if (op == 2)
    {
        criarBaseCliente(arqClientes, 14);
        criarBaseProduto(arqProdutos, 17);
        imprimirBaseCliente(arqClientes);
        imprimirBaseProduto(arqProdutos);
    }

    else
    {
        printf("Opcao invalida. Tente novamente.\n");
        exit(1);
    }

    exibirMenuPrincipal(arqClientes, arqProdutos, arqPedidos);

    fclose(arqClientes);
    fclose(arqProdutos);
    fclose(arqPedidos);

    return 0;
}
