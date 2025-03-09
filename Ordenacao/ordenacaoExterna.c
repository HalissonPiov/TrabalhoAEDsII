#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ordenacaoExterna.h"

// // Função para realizar o heapify (reajustar o heap mínimo)
// void heapify(TCliente **heap, int n, int i)
// {
//     int menor = i;
//     int esq = 2 * i + 1;
//     int dir = 2 * i + 2;

//     if (esq < n && heap[esq]->id < heap[menor]->id)
//         menor = esq;

//     if (dir < n && heap[dir]->id < heap[menor]->id)
//         menor = dir;

//     if (menor != i)
//     {
//         TCliente *temp = heap[i];
//         heap[i] = heap[menor];
//         heap[menor] = temp;
//         heapify(heap, n, menor);
//     }
// }

// // Função para construir o heap mínimo inicial
// void construirHeap(TCliente **heap, int n)
// {
//     for (int i = n / 2 - 1; i >= 0; i--)
//         heapify(heap, n, i);
// }

// // Implementação da Seleção Natural
// void selecaoNatural(FILE *entrada, FILE *saida, int M)
// {

//     TCliente *heap[M]; // Array que funciona como um heap mínimo
//     int tamanho_heap = 0;

//     // Ler os primeiros M registros para o heap
//     for (int i = 0; i < M; i++)
//     {
//         heap[i] = leCliente(entrada);
//         if (heap[i] == NULL)
//             break;
//         tamanho_heap++;
//     }

//     // Construir o heap mínimo inicial
//     construirHeap(heap, tamanho_heap);

//     while (tamanho_heap > 0)
//     {
//         // Escrever o menor registro na saída
//         salvaCliente(heap[0], saida);

//         // Substituir o menor elemento por um novo do arquivo
//         TCliente *novo = leCliente(entrada);
//         if (novo)
//         {
//             heap[0] = novo; // Substituir pelo novo
//         }
//         else
//         {
//             // Se não há mais registros, reduzir o tamanho do heap
//             heap[0] = heap[tamanho_heap - 1];
//             tamanho_heap--;
//         }

//         // Ajustar o heap mínimo
//         heapify(heap, tamanho_heap, 0);
//     }

//     fflush(saida);
// }

// -----------------------------------------------------------------------------------------------

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

    // Passo 1: Carregar M registros do arquivo para a memória
    for (int i = 0; i < M; i++)
    {
        TCliente *cliente = leCliente(entrada);
        if (cliente)
        {
            // inserirHeap(heap, &tamanhoHeap, *cliente);
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

    printf("Heap inicializado com %d registros.\n", tamanhoHeap);

    while (!fimDeArquivo || tamanhoHeap > 0)
    {
        TCliente menor = extrairMin(heap, &tamanhoHeap);
        salvaCliente(&menor, saida);
        printf("Menor registro extraído e salvo na partição: ID = %d\n", menor.id);

        TCliente *novoRegistro = leCliente(entrada);
        if (novoRegistro)
        {
            if (novoRegistro->id >= menor.id)
            {
                inserirHeap(heap, &tamanhoHeap, *novoRegistro);
                printf("Novo registro inserido no heap: ID = %d\n", novoRegistro->id);
            }
            else
            {
                reservatorio[tamanhoReservatorio++] = *novoRegistro;

                if (tamanhoReservatorio == M || fimDeArquivo)
                {
                    fclose(saida);
                    printf("Partição %d finalizada.\n", numParticao);

                    numParticao++;
                    sprintf(nomeArquivo, "Ordenacao/Particoes/particao_%d.dat", numParticao);
                    saida = fopen(nomeArquivo, "wb");

                    if (!saida)
                    {
                        printf("Erro ao criar novo arquivo de particao!\n");
                        return -1;
                    }

                    for (int i = 0; i < tamanhoReservatorio; i++)
                    {
                        inserirHeap(heap, &tamanhoHeap, reservatorio[i]);
                    }
                    tamanhoReservatorio = 0;
                }
            }
            free(novoRegistro);
        }
        else
        {
            fimDeArquivo = 1;
        }
    }

    while (tamanhoHeap > 0)
    {
        TCliente menor = extrairMin(heap, &tamanhoHeap);
        salvaCliente(&menor, saida);
    }

    fclose(saida);
    printf("Foram geradas %d particoes ordenadas.\n", numParticao);

    entrada = fopen("C:\\Users\\halis\\Desktop\\TP-AEDsII\\halissonAtualizado\\TrabalhoAEDsII\\ArquivosDat\\cliente.dat", "w+b");
    if (!entrada)
    {
        printf("Erro ao abrir arquivo de entrada!\n");
        return -1;
    }

    return numParticao;
}

// void criaParticoesOrdenadas(FILE *arquivoEntrada, int M) {
//     FILE *particoes[M];
//     char nomeArquivo[20];
//     TCliente heap[M];
//     int numParticao = 0;
//     int i, heapSize = 0;
//     bool fimArquivo = false;

//     // Criar nome da partição inicial
//     sprintf(nomeArquivo, "particao_%d.dat", numParticao);
//     particoes[numParticao] = fopen(nomeArquivo, "wb");
//     if (!particoes[numParticao]) {
//         printf("Erro ao criar arquivo de partição!\n");
//         return;
//     }

//     // Preencher heap inicial
//     for (i = 0; i < M; i++) {
//         if (fread(&heap[i], sizeof(TCliente), 1, arquivoEntrada) == 1) {
//             heapSize++;
//         } else {
//             fimArquivo = true;
//             break;
//         }
//     }

//     while (heapSize > 0) {
//         // Ordenar heap
//         for (i = 0; i < heapSize - 1; i++) {
//             for (int j = i + 1; j < heapSize; j++) {
//                 if (heap[i].id > heap[j].id) {
//                     TCliente temp = heap[i];
//                     heap[i] = heap[j];
//                     heap[j] = temp;
//                 }
//             }
//         }

//         // Escrever o menor elemento na partição
//         fwrite(&heap[0], sizeof(TCliente), 1, particoes[numParticao]);
//         TCliente ultimoEscrito = heap[0];

//         // Substituir elemento removido por um novo do arquivo
//         if (!fimArquivo) {
//             TCliente novoRegistro;
//             if (fread(&novoRegistro, sizeof(TCliente), 1, arquivoEntrada) == 1) {
//                 if (novoRegistro.id >= ultimoEscrito.id) {
//                     heap[0] = novoRegistro; // Substituir pelo novo
//                 } else {
//                     // Novo elemento é menor -> iniciar nova partição
//                     sprintf(nomeArquivo, "particao_%d.dat", ++numParticao);
//                     particoes[numParticao] = fopen(nomeArquivo, "wb");
//                     if (!particoes[numParticao]) {
//                         printf("Erro ao criar arquivo de partição!\n");
//                         return;
//                     }
//                     fwrite(&novoRegistro, sizeof(TCliente), 1, particoes[numParticao]);
//                 }
//             } else {
//                 fimArquivo = true;
//                 heapSize--;
//                 for (int j = 0; j < heapSize; j++) {
//                     heap[j] = heap[j + 1];
//                 }
//             }
//         } else {
//             heapSize--;
//             for (int j = 0; j < heapSize; j++) {
//                 heap[j] = heap[j + 1];
//             }
//         }
//     }

//     // Fechar todas as partições
//     for (i = 0; i <= numParticao; i++) {
//         fclose(particoes[i]);
//     }
// }