#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "classificacao.h"


void salvarDadosClassificacao(int comparacoes, double tempoExecucao)
{
    FILE *arquivo = fopen("ArquivosLog/dadosClassificacao.txt", "a");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    // ----------------
    fprintf(arquivo, "Comparações: %d\n", comparacoes);
    fprintf(arquivo, "Tempo de execução: %.2f segundos\n", tempoExecucao);
    fclose(arquivo);
    // ----------------
}

// Função auxiliar para trocar dois elementos na memória
void trocar(TCliente *a, TCliente *b)
{
    TCliente temp = *a;
    *a = *b;
    *b = temp;
}

// Função para manter a propriedade do heap (memória) mínima, deixando o menor elemento na raiz
void memMinima(TCliente memoria[], int n, int i)
{
    int menor = i; // Inicializar o nó atual como sendo o menor (i)
    int esq = 2 * i + 1; //  Índice do filho esquerdo de i
    int dir = 2 * i + 2; // Índice do filho esquerdo de i

    // Verifica se o filho esquerdo existe e é menor que o nó atual, ele vira o menor
    if (esq < n && memoria[esq].id < memoria[menor].id)
        menor = esq;

    // Verifica se o filho direito existe e é menor que o menor encontrado até agora, ele vira o menor
    if (dir < n && memoria[dir].id < memoria[menor].id)
        menor = dir;

    // Verifica se algum dos filhos for menor, trocamos com o menor e chamamos recursivamente para corrigir
    if (menor != i)
    {
        trocar(&memoria[i], &memoria[menor]); // Troca o nó atual com o menor filho encontrado
        memMinima(memoria, n, menor); // Recursivamente corrige a estrutura da memoria
    }
}

// Função para extrair o menor elemento da memoria, que estará no topo
TCliente extrairMin(TCliente memoria[], int *n)
{
    TCliente min = memoria[0];
    memoria[0] = memoria[(*n) - 1];
    (*n)--;
    memMinima(memoria, *n, 0);
    return min;
}

// Função para inserir um elemento na memoria
void inserirMemoria(TCliente memoria[], int *n, TCliente novo)
{
    int i = (*n);
    memoria[i] = novo;
    (*n)++;

    // Corrigir a propriedade da memoria subindo o elemento
    while (i > 0 && memoria[(i - 1) / 2].id > memoria[i].id)
    {
        trocar(&memoria[i], &memoria[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Implementação do algoritmo de Seleção Natural
int selecaoNatural(FILE *entrada, int M)
{

    entrada = fopen("C:\\Users\\halis\\Desktop\\TP-AEDsII\\halissonAtualizado\\TrabalhoAEDsII\\ArquivosDat\\cliente.dat", "rb");
    if (!entrada)
    {
        printf("Erro ao abrir arquivo de entrada!\n");
        return -1;
    }

    int comparacoes = 0;
    clock_t inicio = clock();
    TCliente memoria[M];
    TCliente reservatorio[M];
    int tamanhoMemoria = 0, tamanhoReservatorio = 0;
    int fimDeArquivo = 0;

    char nomeArquivo[50];
    int numParticao = 1;
    FILE *saida;

    // Criar o primeiro arquivo de partição
    sprintf(nomeArquivo, "Ordenacao/Particoes/particao_%d.dat", numParticao);
    saida = fopen(nomeArquivo, "wb");
    if (!saida)
    {
        printf("Erro ao criar arquivo de particao!\n");
        fclose(entrada);
        return -1;
    }

    printf("Iniciando Selecao Natural\n");

    // Passo 1: Carregar M registros do arquivo para a memória (heap)
    for (int i = 0; i < M; i++)
    {
        TCliente *cliente = leCliente(entrada);
        if (cliente)
        {
            memoria[tamanhoMemoria++] = *cliente;
            printf("Carregado na memoria: ID = %d\n", cliente->id);
            free(cliente);
        }
        else
        {
            fimDeArquivo = 1;
            printf("Fim do arquivo encontrado ao carregar memoria.\n");
            break;
        }
    }

    // Construir memória inicial antes de iniciar extrações
    for (int i = tamanhoMemoria / 2 - 1; i >= 0; i--)
    {
        memMinima(memoria, tamanhoMemoria, i);
    }

    printf("Memoria inicializada com %d registros.\n", tamanhoMemoria);

    // Extrair o menor elemento ANTES de ler um novo registro do arquivo
    while (!fimDeArquivo || tamanhoMemoria > 0)
    {
        // Passo 2: Extrai o menor registro da memória e salva na partição
        if (tamanhoMemoria > 0)
        {
            TCliente menor = extrairMin(memoria, &tamanhoMemoria);
            salvaCliente(&menor, saida);
            printf("Menor registro extraído e salvo na partição: ID = %d\n", menor.id);

            // Passo 3: Ler um novo registro do arquivo
            TCliente *novoRegistro = leCliente(entrada);
            if (novoRegistro)
            {
                // Se for maior ou igual ao último salvo, insere no memoria
                if (novoRegistro->id >= menor.id)
                {
                    comparacoes++;
                    inserirMemoria(memoria, &tamanhoMemoria, *novoRegistro);
                    printf("Novo registro inserido na memoria: ID = %d\n", novoRegistro->id);
                }
                else
                {
                    comparacoes++;
                    // Caso contrário, vai para o reservatório
                    reservatorio[tamanhoReservatorio++] = *novoRegistro;
                    printf("Registro movido para reservatório: ID = %d\n", novoRegistro->id);
                }
                free(novoRegistro);
            }
            else
            {
                fimDeArquivo = 1;
            }
        }

        // Se a memoria estiver vazio, criar uma nova partição e recarregar a memoria
        if (tamanhoMemoria == 0 && tamanhoReservatorio > 0)
        {
            fclose(saida);
            printf("Partição %d finalizada.\n", numParticao);

            numParticao++;
            sprintf(nomeArquivo, "Ordenacao/Particoes/particao_%d.dat", numParticao);
            saida = fopen(nomeArquivo, "wb");

            if (!saida)
            {
                printf("Erro ao criar novo arquivo de particao!\n");
                fclose(entrada);
                return -1;
            }

            // Reinserir registros do reservatório na memória para a próxima partição
            for (int i = 0; i < tamanhoReservatorio; i++)
            {
                inserirMemoria(memoria, &tamanhoMemoria, reservatorio[i]);
            }
            tamanhoReservatorio = 0;
        }
    }

    fclose(saida);
    printf("Foram geradas %d particoes ordenadas.\n", numParticao);

    fclose(entrada);
    
    clock_t fim = clock();
    double tempoExecucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    salvarDadosClassificacao(comparacoes, tempoExecucao);

    return numParticao;
}

void verificaParticoes(int numParticoes)
{

    char nomeArquivo[50];

    for (int i = 1; i <= numParticoes; i++)
    {
        sprintf(nomeArquivo, "Ordenacao/Particoes/particao_%d.dat", i);
        FILE *arquivo = fopen(nomeArquivo, "rb");

        if (!arquivo)
        {
            printf("Erro ao abrir a partição %d\n", i);
            continue;
        }

        printf("\nConteúdo da Partição %d:\n", i);
        TCliente *cliente;
        int anterior = -1;
        int ordenado = 1;

        while ((cliente = leCliente(arquivo)))
        {
            printf("ID = %d\n", cliente->id);
            if (anterior > cliente->id)
            {
                ordenado = 0; // Se encontrar um valor fora de ordem, marca como erro
            }
            anterior = cliente->id;
            free(cliente);
        }

        fclose(arquivo);

        if (ordenado)
        {
            printf("✅ Partição %d está ordenada corretamente.\n", i);
        }
        else
        {
            printf("❌ ERRO: Partição %d não está ordenada corretamente!\n", i);
        }
    }
}