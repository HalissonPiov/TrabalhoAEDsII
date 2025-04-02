#ifndef TABELAHASH_H_INCLUDED
#define TABELAHASH_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../Entidades/assinaturas.h"
#include "../Console/entradaDados.h"

#define m 7 // definição do tamanho da tabela hash

typedef struct {
    TCliente cliente;
    bool ocupado;
    int prox;
} Entidade;

void inicializarTabelaHashVazia();

int funcaoHash(int chave);

void inserirClienteHash(TCliente cliente);

TCliente *buscarClienteHash(int chave);

void removerClienteHash(int chave);

void exibirTabelaHash();

void exibirArquivoDados();

void reinicializarArquivosHash();

#endif // TABELAHASH_H_INCLUDED