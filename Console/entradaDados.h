#ifndef ENTRADADADOS_H_INCLUDED
#define ENTRADADADOS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#include "../Entidades/assinaturas.h"

#define statusP "Aguardando pagamento"


void cadastrarProduto(FILE *arqProdutos);

void cadastrarCliente(FILE *arqClientes);

void realizarPedido(TCliente *cliente, FILE *arqPedidos, FILE *arqProdutos);

void editarCliente(TCliente *cliente, FILE *arqClientes);

void excluirCliente(TCliente *cliente, FILE *arqClientes);

void realizarOrdenacao(FILE *arquivoClientes);

#endif