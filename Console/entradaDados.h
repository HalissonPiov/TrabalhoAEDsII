#ifndef ENTRADADADOS_H_INCLUDED
#define ENTRADADADOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// #include "../Entidades/produto.c"
#include "../Entidades/assinaturas.h"

#define statusP "Aguardando pagamento"


void cadastrarProduto(FILE *arqProdutos);

void cadastrarCliente(FILE *arqClientes);

// void realizarPedido(TCliente cli, FILE *arqProdutos, FILE *arqPedidos); // ## Se funcionar: passar a função para o arquivo pedido.c

void realizarPedido(TCliente *cliente, FILE *arqPedidos, FILE *arqProdutos);

void efetuarPagamento(TPedido *ped, FILE *arqPedidos);

void cancelarPedido(TPedido *ped, FILE *arqPedidos);

void editarCliente(TCliente *cliente, FILE *arqClientes);

void excluirCliente(TCliente *cliente, FILE *arqClientes);

#endif