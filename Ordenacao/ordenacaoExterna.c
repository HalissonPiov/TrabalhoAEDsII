#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ordenacaoExterna.h"

// Função auxiliar para trocar dois elementos no heap
void swap(TCliente *a, TCliente *b)
{
    TCliente temp = *a;
    *a = *b;
    *b = temp;
}

// Função para manter a propriedade do heap mínimo
void minHeapify(TCliente heap[], int n, int i)
{
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && heap[esq].id < heap[menor].id)
        menor = esq;
    if (dir < n && heap[dir].id < heap[menor].id)
        menor = dir;

    if (menor != i)
    {
        swap(&heap[i], &heap[menor]);
        minHeapify(heap, n, menor);
    }
}

// Função para extrair o menor elemento do heap
TCliente extrairMin(TCliente heap[], int *n)
{
    TCliente min = heap[0];
    heap[0] = heap[(*n) - 1];
    (*n)--;
    minHeapify(heap, *n, 0);
    return min;
}

// Função para inserir um elemento no heap
void inserirHeap(TCliente heap[], int *n, TCliente novo)
{
    int i = (*n);
    heap[i] = novo;
    (*n)++;

    // Corrigir a propriedade do heap subindo o elemento
    while (i > 0 && heap[(i - 1) / 2].id > heap[i].id)
    {
        swap(&heap[i], &heap[(i - 1) / 2]);
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

    TCliente heap[M];
    TCliente reservatorio[M];
    int tamanhoHeap = 0, tamanhoReservatorio = 0;
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
            heap[tamanhoHeap++] = *cliente;
            printf("Carregado no heap: ID = %d\n", cliente->id);
            free(cliente);
        }
        else
        {
            fimDeArquivo = 1;
            printf("Fim do arquivo encontrado ao carregar heap.\n");
            break;
        }
    }

    // Construir heap inicial antes de iniciar extrações
    for (int i = tamanhoHeap / 2 - 1; i >= 0; i--)
    {
        minHeapify(heap, tamanhoHeap, i);
    }

    printf("Heap inicializado com %d registros.\n", tamanhoHeap);

    // Extrair o menor elemento ANTES de ler um novo registro do arquivo
    while (!fimDeArquivo || tamanhoHeap > 0)
    {
        // Passo 2: Extrai o menor registro do heap e salva na partição
        if (tamanhoHeap > 0)
        {
            TCliente menor = extrairMin(heap, &tamanhoHeap);
            salvaCliente(&menor, saida);
            printf("Menor registro extraído e salvo na partição: ID = %d\n", menor.id);

            // Passo 3: Ler um novo registro do arquivo
            TCliente *novoRegistro = leCliente(entrada);
            if (novoRegistro)
            {
                // Se for maior ou igual ao último salvo, insere no heap
                if (novoRegistro->id >= menor.id)
                {
                    inserirHeap(heap, &tamanhoHeap, *novoRegistro);
                    printf("Novo registro inserido no heap: ID = %d\n", novoRegistro->id);
                }
                else
                {
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

        // Se o heap estiver vazio, criar uma nova partição e recarregar o heap
        if (tamanhoHeap == 0 && tamanhoReservatorio > 0)
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

            // Reinserir registros do reservatório no heap para a próxima partição
            for (int i = 0; i < tamanhoReservatorio; i++)
            {
                inserirHeap(heap, &tamanhoHeap, reservatorio[i]);
            }
            tamanhoReservatorio = 0;
        }
    }

    fclose(saida);
    printf("Foram geradas %d particoes ordenadas.\n", numParticao);

    fclose(entrada);
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