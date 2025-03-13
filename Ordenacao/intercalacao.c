#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intercalacao.h"

// void intercalacaoOtima(int numArquivos, int F) {

//     while (numArquivos > 1) {
//         int numNovosArquivos = 0;
//         char *novosArquivos[numArquivos / (F - 1) + 1];

//         for (int i = 0; i < numArquivos; i += (F - 1)) {
//             int qtdIntercalar = (i + F - 1 < numArquivos) ? (F - 1) : (numArquivos - i);
//             char arquivoSaida[50];
//             sprintf(arquivoSaida, "Ordenacao/Intercalados/intercalado_%d.dat", numNovosArquivos + 1);

//             intercalaArquivos(&arquivos[i], qtdIntercalar, arquivoSaida);

//             novosArquivos[numNovosArquivos++] = strdup(arquivoSaida);
//         }

//         for (int i = 0; i < numArquivos; i++) {
//             remove(arquivos[i]); // Remove arquivos intermediários após intercalação
//         }

//         memcpy(arquivos, novosArquivos, numNovosArquivos * sizeof(char *));
//         numArquivos = numNovosArquivos;
//     }

//     // Renomeia o último arquivo para ser o arquivo final
//     rename(arquivos[0], "Ordenacao/clientes.dat");
// }

void intercalacaoOtima(int numParticoes, int F)
{
    char listaArquivos[numParticoes][50]; // Lista de nomes dos arquivos de entrada

    // Gerando dinamicamente os nomes dos arquivos de entrada
    for (int i = 1; i <= numParticoes; i++)
    {
        sprintf(listaArquivos[i - 1], "Ordenacao/Particoes/particao_%d.dat", i);
    }

    int numArquivos = numParticoes; // Inicia com a quantidade de partições criadas

    while (numArquivos > 1)
    {
        int novoNumArquivos = 0; // Contador de novos arquivos gerados
        char novaListaArquivos[numArquivos][50]; // Lista temporária para os novos arquivos gerados

        for (int i = 0; i < numArquivos; i += F - 1) // Pegamos F-1 arquivos por vez
        {
            // Define os arquivos a serem intercalados
            int fim = (i + (F - 1) < numArquivos) ? (i + (F - 1)) : numArquivos; // Verifica se ainda há F-1 arquivos para intercalar
            char arquivoSaida[50];

            if (numArquivos - i <= F - 1) // Última intercalação gera o arquivo final
            {
                sprintf(arquivoSaida, "Ordenacao/arquivo_final.dat");
            }
            else
            {
                sprintf(arquivoSaida, "Ordenacao/Particoes/temp_intercalado_%d.dat", novoNumArquivos + 1);
            }

            // Chamada da função que faz a intercalação de F-1 arquivos
            intercalarArquivos(&listaArquivos[i], fim - i, arquivoSaida);

            // Atualiza a lista com o novo arquivo gerado
            strcpy(novaListaArquivos[novoNumArquivos], arquivoSaida);
            novoNumArquivos++;
        }

        // Copia os novos arquivos gerados para a lista original
        memcpy(listaArquivos, novaListaArquivos, sizeof(novaListaArquivos));

        numArquivos = novoNumArquivos; // Atualiza o número total de arquivos para a próxima rodada
    }

    printf("Intercalação finalizada. Arquivo gerado: Ordenacao/arquivo_final.dat\n");

    printf("Lendo registros do arquivo final...\n");
    FILE *teste = fopen("Ordenacao/arquivo_final.dat", "rb");
    TCliente *c;
    while ((c = leCliente(teste)) != NULL)
    {
        printf("ID: %d\n", c->id);
        free(c);
    }
    fclose(teste);

    FILE *arqClientesOrdenado = fopen("Ordenacao/arquivo_final.dat", "r+b");

    if (arqClientesOrdenado == NULL)
    {
        printf("Erro ao abrir arquivo de clientes ordenado\n");
        return;
    }

    imprimirBaseCliente(arqClientesOrdenado);
    fclose(arqClientesOrdenado);
}

void intercalarArquivos(char arquivos[][50], int numArquivos, char *arquivoSaida)
{
    FILE *entradas[numArquivos];
    FILE *saida = fopen(arquivoSaida, "wb");

    if (!saida)
    {
        printf("Erro ao criar arquivo de saída!\n");
        return;
    }

    // Abre os arquivos de entrada
    for (int i = 0; i < numArquivos; i++)
    {
        entradas[i] = fopen(arquivos[i], "rb");
        if (!entradas[i])
        {
            printf("Erro ao abrir %s\n", arquivos[i]);
            fclose(saida);
            return;
        }
        else
        {
            printf("Arquivo aberto: %s\n", arquivos[i]);
        }
    }

    ElementoMemoria heap[numArquivos];
    int tamanhoHeap = 0;

    // Inicializa o heap com o primeiro registro de cada arquivo
    for (int i = 0; i < numArquivos; i++)
    {
        fseek(entradas[i], 0, SEEK_SET);
        TCliente *cliente = leCliente(entradas[i]);
        if (cliente)
        {
            printf("Lido de %s: ID %d\n", arquivos[i], cliente->id);
            heap[tamanhoHeap].cliente = *cliente;
            heap[tamanhoHeap].origem = i;
            tamanhoHeap++;
            free(cliente);
        }
        else
        {
            printf("Nenhum registro lido de %s\n", arquivos[i]);
        }
    }

    // Constrói o heap mínimo com os primeiros elementos
    for (int i = tamanhoHeap / 2 - 1; i >= 0; i--)
    {
        minHeapifyElementos(heap, tamanhoHeap, i);
    }

    // Intercalação dos arquivos
    while (tamanhoHeap > 0)
    {
        // Extrai o menor elemento do heap
        ElementoMemoria menor = heap[0];
        salvaCliente(&menor.cliente, saida);
        printf("Salvando ID %d no arquivo %s\n", menor.cliente.id, arquivoSaida);

        // Lê um novo registro do mesmo arquivo de onde veio o menor
        TCliente *novoRegistro = leCliente(entradas[menor.origem]);
        if (novoRegistro)
        {
            heap[0].cliente = *novoRegistro;
            heap[0].origem = menor.origem;
            free(novoRegistro);
        }
        else
        {
            // Remove o último elemento do heap e diminui o tamanho
            heap[0] = heap[tamanhoHeap - 1];
            tamanhoHeap--;
        }

        // Restaura a propriedade do heap mínimo
        if (tamanhoHeap > 0)
        {
            minHeapifyElementos(heap, tamanhoHeap, 0);
        }
    }

    // Fecha todos os arquivos
    for (int i = 0; i < numArquivos; i++)
    {
        fclose(entradas[i]);
    }

    fclose(saida);
}

void minHeapifyElementos(ElementoMemoria heap[], int n, int i)
{
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && heap[esq].cliente.id < heap[menor].cliente.id)
        menor = esq;
    if (dir < n && heap[dir].cliente.id < heap[menor].cliente.id)
        menor = dir;

    if (menor != i)
    {
        ElementoMemoria temp = heap[i];
        heap[i] = heap[menor];
        heap[menor] = temp;
        minHeapifyElementos(heap, n, menor);
    }
}

// void intercalaArquivos(char *arquivos[], int qtdIntercalar, char *arquivoSaida)
// {
//     FILE *arqEntrada[qtdIntercalar];
//     FILE *arqSaida = fopen(arquivoSaida, "wb");

//     TCliente *clientes[qtdIntercalar];
//     TCliente *menorCliente;
//     int posicaoMenorCliente;

//     for (int i = 0; i < qtdIntercalar; i++)
//     {
//         arqEntrada[i] = fopen(arquivos[i], "rb");
//         clientes[i] = leCliente(arqEntrada[i]);
//     }

//     while (1)
//     {
//         menorCliente = NULL;
//         for (int i = 0; i < qtdIntercalar; i++)
//         {
//             if (clientes[i] != NULL)
//             {
//                 if (menorCliente == NULL || clientes[i]->id < menorCliente->id)
//                 {
//                     menorCliente = clientes[i];
//                     posicaoMenorCliente = i;
//                 }
//             }
//         }

//         if (menorCliente == NULL)
//         {
//             break;
//         }

//         salvaCliente(menorCliente, arqSaida);
//         clientes[posicaoMenorCliente] = leCliente(arqEntrada[posicaoMenorCliente]);
//     }

//     for (int i = 0; i < qtdIntercalar; i++)
//     {
//         fclose(arqEntrada[i]);
//     }

//     fclose(arqSaida); // Acho que não deveria fechar
// }

// // Outra versão gerada no copilot
// void intercalarArquivos(char arquivos[][50], int numArquivos, char *arquivoSaida)
// {
//     FILE *entradas[numArquivos];
//     FILE *saida = fopen(arquivoSaida, "wb");

//     if (!saida)
//     {
//         printf("Erro ao criar arquivo de saída!\n");
//         return;
//     }

//     // Abre os arquivos de entrada
//     for (int i = 0; i < numArquivos; i++)
//     {
//         entradas[i] = fopen(arquivos[i], "rb");
//         if (!entradas[i])
//         {
//             printf("Erro ao abrir %s\n", arquivos[i]);
//             fclose(saida);
//             return;
//         }
//     }

//     ElementoMemoria heap[numArquivos];
//     int tamanhoHeap = 0;

//     // Inicializa o heap com o primeiro registro de cada arquivo
//     for (int i = 0; i < numArquivos; i++)
//     {
//         TCliente *cliente = leCliente(entradas[i]);
//         if (cliente)
//         {
//             heap[tamanhoHeap].cliente = *cliente;
//             heap[tamanhoHeap].origem = i;
//             tamanhoHeap++;
//             free(cliente);
//         }
//     }

//     // Constrói o heap mínimo com os primeiros elementos
//     for (int i = tamanhoHeap / 2 - 1; i >= 0; i--)
//     {
//         minHeapifyElementos(heap, tamanhoHeap, i);
//     }

//     // Intercalação dos arquivos
//     while (tamanhoHeap > 0)
//     {
//         // Extrai o menor elemento do heap
//         ElementoMemoria menor = heap[0];
//         salvaCliente(&menor.cliente, saida);

//         // Lê um novo registro do mesmo arquivo de onde veio o menor
//         TCliente *novoRegistro = leCliente(entradas[menor.origem]);
//         if (novoRegistro)
//         {
//             heap[0].cliente = *novoRegistro;
//             heap[0].origem = menor.origem;
//             free(novoRegistro);
//         }
//         else
//         {
//             // Remove o último elemento do heap e diminui o tamanho
//             heap[0] = heap[tamanhoHeap - 1];
//             tamanhoHeap--;
//         }

//         // Restaura a propriedade do heap mínimo
//         minHeapifyElementos(heap, tamanhoHeap, 0);
//     }

//     // Fecha todos os arquivos
//     for (int i = 0; i < numArquivos; i++)
//     {
//         fclose(entradas[i]);
//     }

//     fclose(saida);
// }