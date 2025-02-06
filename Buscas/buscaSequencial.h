#ifndef BUSCASEQUENCIAL_H_INCLUDED
#define BUSCASEQUENCIAL_H_INCLUDED
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Entidades/assinaturas.h"

// typedef struct Cliente
// {
//     int cod;
//     char nome[50];
//     char cpf[15];
//     char data_nascimento[11];
//     double salario;
// } TCliente;


void salvarDadosSequencial(int comparacoes, double tempoExecucao);

TCliente *buscaSequencialCliente(int chave, FILE *in);

TCliente *buscaSequencialClienteEditar(int chave, FILE *in);

TProduto *buscaSequencialProduto(int chave, FILE *in);

TPedido *buscaSequencialPedido(int chave, FILE *in);




#endif // BUSCASEQUENCIAL_H_INCLUDED
