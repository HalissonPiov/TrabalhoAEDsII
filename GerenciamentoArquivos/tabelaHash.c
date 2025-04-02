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
            printf("Erro ao abrir/criar tabela hash!\n");
            return;
        }
        inicializarTabelaHashVazia(); // Se for novo arquivo, inicializa
    }

    FILE *arqDados = fopen("GerenciamentoArquivos/entidade.dat", "rb+");
    if (!arqDados)
    {
        arqDados = fopen("GerenciamentoArquivos/entidade.dat", "wb+");
        if (!arqDados)
        {
            printf("Erro ao abrir/criar arquivo de dados!\n");
            fclose(arqHash);
            return;
        }
    }

    Entidade novaEntidade;
    novaEntidade.cliente = cliente;
    novaEntidade.ocupado = 1; // Marca como ocupado
    novaEntidade.prox = -1;

    int posicaoHash = funcaoHash(cliente.id);
    int compartimento;

    // Lê a posição inicial na tabela hash
    fseek(arqHash, posicaoHash * sizeof(int), SEEK_SET);
    fread(&compartimento, sizeof(int), 1, arqHash);

    // Passo 1: Buscar uma posição livre no arquivo de dados
    int posicaoLivre = -1;
    Entidade aux;
    fseek(arqDados, 0, SEEK_SET);
    int indice = 0;

    while (fread(&aux, sizeof(Entidade), 1, arqDados) == 1)
    {
        if (aux.ocupado == 0) // Verifica se o registro está desocupado
        {
            posicaoLivre = indice;
            novaEntidade.cliente.id = aux.cliente.id; // Reutiliza o ID do registro desocupado
            break;
        }
        indice++;
    }

    // Passo 2: Reutilizar espaço livre (se encontrado)
    if (posicaoLivre != -1)
    {
        printf("Reutilizando posição %d para cliente ID %d\n", posicaoLivre, cliente.id);

        // Grava o novo registro na posição livre
        fseek(arqDados, posicaoLivre * sizeof(Entidade), SEEK_SET);
        fwrite(&novaEntidade, sizeof(Entidade), 1, arqDados);
        fflush(arqDados);

        // Atualiza a tabela hash se ele for o primeiro registro da lista encadeada
        if (compartimento == -1)
        {
            fseek(arqHash, posicaoHash * sizeof(int), SEEK_SET);
            fwrite(&posicaoLivre, sizeof(int), 1, arqHash);
        }
        else
        {
            // Adiciona ao final da lista encadeada
            int atual = compartimento;

            while (1)
            {
                fseek(arqDados, atual * sizeof(Entidade), SEEK_SET);
                fread(&aux, sizeof(Entidade), 1, arqDados);

                if (aux.prox == -1)
                {
                    aux.prox = posicaoLivre;
                    fseek(arqDados, atual * sizeof(Entidade), SEEK_SET);
                    fwrite(&aux, sizeof(Entidade), 1, arqDados);
                    break;
                }
                atual = aux.prox;
            }
        }
    }
    else
    {
        // Passo 3: Se não há espaço livre, insere no final
        fseek(arqDados, 0, SEEK_END);
        int novaPosicao = ftell(arqDados) / sizeof(Entidade);
        fwrite(&novaEntidade, sizeof(Entidade), 1, arqDados);

        // Atualiza a tabela hash
        if (compartimento == -1)
        {
            fseek(arqHash, posicaoHash * sizeof(int), SEEK_SET);
            fwrite(&novaPosicao, sizeof(int), 1, arqHash);
        }
        else
        {
            // Adiciona ao final da lista encadeada
            int atual = compartimento;
            while (1)
            {
                fseek(arqDados, atual * sizeof(Entidade), SEEK_SET);
                fread(&aux, sizeof(Entidade), 1, arqDados);

                if (aux.prox == -1)
                {
                    aux.prox = novaPosicao;
                    fseek(arqDados, atual * sizeof(Entidade), SEEK_SET);
                    fwrite(&aux, sizeof(Entidade), 1, arqDados);
                    break;
                }
                atual = aux.prox;
            }
        }
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
        printf("Erro ao abrir o arquivo da tabela hash!\n");
        return;
    }

    FILE *arqDados = fopen("GerenciamentoArquivos/entidade.dat", "rb+");
    if (!arqDados)
    {
        printf("Erro ao abrir o arquivo de dados!\n");
        fclose(arqHash);
        return;
    }

    int pos = funcaoHash(id); // Calcula a posição na tabela hash
    int compartimento;
    Entidade aux;

    fseek(arqHash, pos * sizeof(int), SEEK_SET);
    fread(&compartimento, sizeof(int), 1, arqHash);

    int anterior = -1; // Para rastrear o registro anterior na lista encadeada

    while (compartimento != -1)
    {
        fseek(arqDados, compartimento * sizeof(Entidade), SEEK_SET);
        fread(&aux, sizeof(Entidade), 1, arqDados);

        if (aux.ocupado && aux.cliente.id == id)
        {
            aux.ocupado = 0; // Marca o registro como desocupado

            if (anterior != -1)
            {
                // Atualiza o ponteiro `prox` do registro anterior
                Entidade anteriorEntidade;
                fseek(arqDados, anterior * sizeof(Entidade), SEEK_SET);
                fread(&anteriorEntidade, sizeof(Entidade), 1, arqDados);

                anteriorEntidade.prox = aux.prox; // Ignora o registro removido
                fseek(arqDados, anterior * sizeof(Entidade), SEEK_SET);
                fwrite(&anteriorEntidade, sizeof(Entidade), 1, arqDados);
            }
            else
            {
                // Atualiza a tabela hash para apontar para o próximo registro
                fseek(arqHash, pos * sizeof(int), SEEK_SET);
                fwrite(&aux.prox, sizeof(int), 1, arqHash);
            }

            // Atualiza o registro no arquivo de dados
            fseek(arqDados, compartimento * sizeof(Entidade), SEEK_SET);
            fwrite(&aux, sizeof(Entidade), 1, arqDados);

            printf("Cliente com ID %d removido com sucesso.\n", id);

            fclose(arqHash);
            fclose(arqDados);
            return;
        }

        anterior = compartimento; // Atualiza o registro anterior
        compartimento = aux.prox; // Avança para o próximo registro
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
    FILE *arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "rb");
    if (!arqHash)
    {
        printf("Erro ao abrir o arquivo da tabela hash!\n");
        return;
    }

    FILE *arqDados = fopen("GerenciamentoArquivos/entidade.dat", "rb");
    if (!arqDados)
    {
        printf("Erro ao abrir o arquivo de dados!\n");
        fclose(arqHash);
        return;
    }

    printf("\nArquivo de Dados e Listas Encadeadas:\n");
    printf("-------------------------------------------------------------\n");
    printf("Compartimento | Posicao | ID  | Nome                 | Prox | Ocupado \n");
    printf("-------------------------------------------------------------\n");

    int compartimento;
    Entidade entidade;
    for (int i = 0; i < m; i++)
    {
        // Lê o valor do compartimento na tabela hash
        fseek(arqHash, i * sizeof(int), SEEK_SET);
        fread(&compartimento, sizeof(int), 1, arqHash);

        if (compartimento == -1)
        {
            // Compartimento vazio
            printf("      %d        |   -   |   -   | %-20s |   -   |   -\n", i, "Nenhum registro");
            continue;
        }

        // Percorre a lista encadeada associada ao compartimento
        int posicao = compartimento;
        while (posicao != -1)
        {
            fseek(arqDados, posicao * sizeof(Entidade), SEEK_SET);
            fread(&entidade, sizeof(Entidade), 1, arqDados);

            printf("      %d        |   %d   |  %d  | %-20s |   %d   |   %d\n",
                   i, posicao, entidade.cliente.id, entidade.cliente.nome, entidade.prox, entidade.ocupado);

            posicao = entidade.prox; // Avança para o próximo registro na lista encadeada
        }
    }

    printf("-------------------------------------------------------------\n");

    fclose(arqHash);
    fclose(arqDados);
}

void reinicializarArquivosHash()
{
    // Reinicializa o arquivo tabelaHash.dat
    FILE *arqHash = fopen("GerenciamentoArquivos/tabelaHash.dat", "wb");
    if (!arqHash)
    {
        printf("Erro ao criar o arquivo tabelaHash.dat!\n");
        return;
    }

    int vazio = -1;
    for (int i = 0; i < m; i++)
    {
        fwrite(&vazio, sizeof(int), 1, arqHash);
    }
    fclose(arqHash);

    // Reinicializa o arquivo entidade.dat
    FILE *arqDados = fopen("GerenciamentoArquivos/entidade.dat", "wb");
    if (!arqDados)
    {
        printf("Erro ao criar o arquivo entidade.dat!\n");
        return;
    }
    fclose(arqDados);

    printf("Arquivos da tabela hash reinicializados com sucesso!\n");
}