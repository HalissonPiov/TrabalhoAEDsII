#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tabelaHash.h"


void inicializarTabelaHashVazia()
{
    FILE *arquivo = fopen("GerenciamentoArquivos/tabelaHash.dat", "wb");
    if (!arquivo)
    {
        printf("Erro ao criar o arquivo da tabela hash!\n");
        return;
    }

    int vazio = -1;

    rewind(arquivo);

    for (int i = 0; i < m; i++)
    {
        fwrite(&vazio, sizeof(int), 1, arquivo);
        // fwrite(&clienteVazio, sizeof(TCliente), 1, arquivo);
    }
    fclose(arquivo);

    // inicializa todos as posições do compartimento com -1
    // vai ter apenas valores int
}

int funcaoHash(int id)
{
    return id % m;
}

void inserirClienteHash(TCliente cliente)
{
    FILE *arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "rb+");
    if (!arqHash)
    {
        arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "wb+");
        if (!arqHash)
        {
            printf("Erro ao criar o arquivo da tabela hash!\n");
            return;
        }
    }

    FILE *arqDados = fopen("GerenciamentoArquivos/entidade.dat", "rb+");
    if (!arqDados)
    {
        arqDados = fopen("GerenciamentoArquivos/entidade.dat", "wb+");
        if (!arqDados)
        {
            printf("Erro ao criar o arquivo de dados!\n");
            fclose(arqHash);
            return;
        }
    }

    Entidade novaEntidade;
    novaEntidade.cliente = cliente;
    novaEntidade.ocupado = 1;
    novaEntidade.prox = -1;

    int posicao = funcaoHash(cliente.id);
    int compartimento;

    fseek(arqHash, posicao * sizeof(int), SEEK_SET); // posição correspondente ao registro na tabela hash
    fread(&compartimento, sizeof(int), 1, arqHash);  // lê o valor da posição

    if (compartimento == -1) // indica que não há registros nessa posição no arquivo de dados (lista vazia)
    {

        fseek(arqDados, 0, SEEK_END); // move para o final do arquivo de dados

        int compartimentoDados = ftell(arqDados) / sizeof(Entidade); // calcula a posição em que será inserido o registro no arquivo de dados
        fwrite(&novaEntidade, sizeof(Entidade), 1, arqDados);

        fseek(arqHash, posicao * sizeof(int), SEEK_SET);
        fwrite(&compartimentoDados, sizeof(int), 1, arqHash); // tabela hash aponta para o registro no arquivo de dados (que é o primeiro lá)
        fflush(arqHash);
    }
    else
    {
        // Tratamento de colisão
        Entidade aux;
        while (compartimento != -1)
        {

            fseek(arqDados, compartimento * sizeof(Entidade), SEEK_SET); // posição correspondente ao registro no arquivo de dados
            fread(&aux, sizeof(Entidade), 1, arqDados);

            if (!aux.ocupado) // se encontrar um registro com ocupado = 0, insere o novo registro nessa posição
            {
                novaEntidade.prox = aux.prox; // Mantém a estrutura da lista encadeada
                fseek(arqDados, compartimento * sizeof(Entidade), SEEK_SET);
                fwrite(&novaEntidade, sizeof(Entidade), 1, arqDados); // Sobrescreve o registro excluído
                
                // Atualiza a tabela hash caso seja o primeiro registro na lista
                fseek(arqHash, posicao * sizeof(int), SEEK_SET);
                fwrite(&compartimento, sizeof(int), 1, arqHash);  // Mantém referência correta
            
                fclose(arqHash);
                fclose(arqDados);
                return;
            }
            if (aux.prox == -1)
            { // verifica se é o último registro da lista de dados
                break;
            }
            compartimento = aux.prox;
        }

        // Cliente sendo inserido no final da lista encadeada
        fseek(arqDados, 0, SEEK_END);
        int novoCompartimento = ftell(arqDados) / sizeof(Entidade);
        aux.prox = novoCompartimento; // Atualiza o ponteiro do antigo último da lista

        // Grava o antigo último registro atualizado
        fseek(arqDados, compartimento * sizeof(Entidade), SEEK_SET);
        fwrite(&aux, sizeof(Entidade), 1, arqDados);

        // Atualiza o novo registro corretamente
        novaEntidade.prox = -1; // O último cliente sempre deve apontar para -1
        fseek(arqDados, 0, SEEK_END);
        fwrite(&novaEntidade, sizeof(Entidade), 1, arqDados);
    }
    fclose(arqHash);
    fclose(arqDados);
}

TCliente *buscarClienteHash(int id)
{
    FILE *arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "rb+");
    if (!arqHash)
    {
        arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "wb+");
        if (!arqHash)
        {
            printf("Erro ao criar o arquivo da tabela hash!\n");
            return NULL;
        }
    }

    FILE *arqDados = fopen("GerenciamentoArquivos/entidade.dat", "rb+");
    if (!arqDados)
    {
        arqDados = fopen("GerenciamentoArquivos/entidade.dat", "wb+");
        if (!arqDados)
        {
            printf("Erro ao criar o arquivo de dados!\n");
            fclose(arqHash);
            return NULL;
        }
    }

    int pos = funcaoHash(id);
    int compartimento;
    fseek(arqHash, pos * sizeof(int), SEEK_SET);    // posição correspondente ao registro na tabela hash
    fread(&compartimento, sizeof(int), 1, arqHash); // lê o valor da posição na tabela hash

    while (compartimento != -1) // percorre a lista encadeada de registros
    {
        Entidade aux;
        fseek(arqDados, compartimento * sizeof(Entidade), SEEK_SET);
        fread(&aux, sizeof(Entidade), 1, arqDados); // lê o registro no arquivo de dados e salva na aux

        if (aux.ocupado && aux.cliente.id == id)
        {
            fclose(arqHash);
            fclose(arqDados);
            TCliente *clienteEncontrado = (TCliente *)malloc(sizeof(TCliente));
            *clienteEncontrado = aux.cliente;

            printf("Cliente com ID %d encontrado com sucesso.\n", id);

            return clienteEncontrado;
        }
        compartimento = aux.prox;
    }

    printf("Cliente com ID %d nao encontrado.\n", id);

    fclose(arqHash);
    fclose(arqDados);
    return NULL;
}

void removerClienteHash(int id)
{
    FILE *arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "rb+");
    if (!arqHash)
    {
        arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "wb+");
        if (!arqHash)
        {
            printf("Erro ao criar o arquivo da tabela hash!\n");
            return;
        }
    }

    FILE *arqDados = fopen("GerenciamentoArquivos/entidade.dat", "rb+");
    if (!arqDados)
    {
        arqDados = fopen("GerenciamentoArquivos/entidade.dat", "wb+");
        if (!arqDados)
        {
            printf("Erro ao criar o arquivo de dados!\n");
            fclose(arqHash);
            return;
        }
    }

    int pos = funcaoHash(id);
    int compartimento;

    fseek(arqHash, pos * sizeof(int), SEEK_SET);
    fread(&compartimento, sizeof(int), 1, arqHash);

    while (compartimento != -1)
    {
        Entidade aux;
        fseek(arqDados, compartimento * sizeof(Entidade), SEEK_SET);
        fread(&aux, sizeof(Entidade), 1, arqDados);

        if (aux.ocupado && aux.cliente.id == id)
        {
            aux.ocupado = 0;
            fseek(arqDados, compartimento * sizeof(Entidade), SEEK_SET);
            fwrite(&aux, sizeof(Entidade), 1, arqDados); // altera o campo ocupado para 0 no arquivo de dados

            excluirCliente(&aux.cliente); // remove o cliente também da base de dados de clientes

            fclose(arqHash);
            fclose(arqDados);

            printf("Cliente com ID %d removido.\n", id);

            return;
        }
        compartimento = aux.prox;
    }

    printf("Cliente com ID %d não encontrado para ser removido.\n", id);

    fclose(arqHash);
    fclose(arqDados);
}

// -----------------------------------------------------------------------------

void exibirTabelaHash()
{

    FILE *arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "rb");
    if (!arqHash)
    {
        printf("Erro ao abrir o arquivo da tabela hash!\n");
        return;
    }

    printf("\nTabela Hash:\n");
    printf("---------------------------------------\n");
    printf("Compartimento | Posicao do registro\n");
    printf("---------------------------------------\n");

    int compartimento;
    for (int i = 0; i < m; i++)
    {
        fread(&compartimento, sizeof(int), 1, arqHash);
        printf("            %d            |            %d            \n", i, compartimento);
    }
    printf("-----------------\n");

    fclose(arqHash);
}

void exibirArquivoDados()
{

    FILE *arqDados = fopen("GerenciamentoArquivos/entidade.dat", "rb");
    if (!arqDados)
    {
        printf("Erro ao abrir o arquivo de dados!\n");
        return;
    }

    printf("\nArquivo de Dados: \n");
    printf("-------------------------------------------------------------\n");
    printf("Posicao | ID  | Nome                 | Prox | Ocupado \n");
    printf("-------------------------------------------------------------\n");

    Entidade entidade;
    int posicao = 0;

    while (fread(&entidade, sizeof(Entidade), 1, arqDados))
    {
        printf("   %d    |  %d  |  %-20s  |   %d   |  %d\n",
               posicao, entidade.cliente.id, entidade.cliente.nome, entidade.prox, entidade.ocupado);
        posicao++;
    }
    printf("-------------------------------------------------------------\n");

    fclose(arqDados);
}

void reinicializarArquivosHash() {
    // Reinicializa o arquivo tabelaHash.dat
    FILE *arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "wb");
    if (!arqHash) {
        printf("Erro ao criar o arquivo tabelaHash.dat!\n");
        return;
    }

    int vazio = -1;
    for (int i = 0; i < m; i++) {
        fwrite(&vazio, sizeof(int), 1, arqHash);
    }
    fclose(arqHash);

    // Reinicializa o arquivo entidade.dat
    FILE *arqDados = fopen("GerenciamentoArquivos/entidade.dat", "wb");
    if (!arqDados) {
        printf("Erro ao criar o arquivo entidade.dat!\n");
        return;
    }
    fclose(arqDados);

    printf("Arquivos da tabela hash reinicializados com sucesso!\n");
}