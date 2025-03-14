#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "intercalacao.h"


void intercalacaoOtima(int numParticoes, int F)
{
    char listaArquivos[numParticoes][50]; // Lista de nomes dos arquivos de entrada

    // Gerando dinamicamente os nomes dos arquivos de entrada
    for (int i = 0; i < numParticoes; i++)
    {
        sprintf(listaArquivos[i], "Ordenacao/Particoes/particao_%d.dat", i + 1);
    }

    int numArquivos = numParticoes; // Inicia com a quantidade de partições criadas
    int rodada = 1;                 // Contador para os arquivos temporários

    while (numArquivos > 1)
    {
        int novoNumArquivos = 0;                 // Contador de novos arquivos gerados
        char novaListaArquivos[numArquivos][50]; // Lista temporária para os novos arquivos gerados

        for (int i = 0; i < numArquivos; i += F) // Pegamos até F arquivos por vez
        {
            // Define os arquivos a serem intercalados
            int fim = (i + F < numArquivos) ? (i + F) : numArquivos; // Pega até F arquivos

            char arquivoSaida[50];
            if (fim == numArquivos && numArquivos <= F) // Se for a última rodada
            {
                sprintf(arquivoSaida, "Ordenacao/arquivo_final.dat");
            }
            else
            {
                sprintf(arquivoSaida, "Ordenacao/Particoes/temp_intercalado_%d.dat", rodada++);
            }

            // Chamada da função que faz a intercalação de até F arquivos
            intercalarArquivos(&listaArquivos[i], fim - i, arquivoSaida);

            // Atualiza a lista com o novo arquivo gerado
            strcpy(novaListaArquivos[novoNumArquivos], arquivoSaida);
            novoNumArquivos++;
        }

        // Copia os novos arquivos gerados para a lista original
        memcpy(listaArquivos, novaListaArquivos, novoNumArquivos * 50);

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

    ElementoMemoria memoria[numArquivos];
    int tamanhoMemoria = 0;

    // Inicializa a memoria com o primeiro registro de cada arquivo
    for (int i = 0; i < numArquivos; i++)
    {
        fseek(entradas[i], 0, SEEK_SET);
        TCliente *cliente = leCliente(entradas[i]);
        if (cliente)
        {
            printf("Lido de %s: ID %d\n", arquivos[i], cliente->id);
            memoria[tamanhoMemoria].cliente = *cliente;
            memoria[tamanhoMemoria].origem = i;
            tamanhoMemoria++;
            free(cliente);
        }
        else
        {
            printf("Nenhum registro lido de %s\n", arquivos[i]);
        }
    }

    // Constrói o memoria mínimo com os primeiros elementos
    for (int i = tamanhoMemoria / 2 - 1; i >= 0; i--)
    {
        memMinimaElementos(memoria, tamanhoMemoria, i);
    }

    // Intercalação dos arquivos
    while (tamanhoMemoria > 0)
    {
        // Extrai o menor elemento da memoria
        ElementoMemoria menor = memoria[0];
        salvaCliente(&menor.cliente, saida);
        printf("Salvando ID %d no arquivo %s\n", menor.cliente.id, arquivoSaida);

        // Lê um novo registro do mesmo arquivo de onde veio o menor
        TCliente *novoRegistro = leCliente(entradas[menor.origem]);
        if (novoRegistro)
        {
            memoria[0].cliente = *novoRegistro;
            memoria[0].origem = menor.origem;
            free(novoRegistro);
        }
        else
        {
            // Remove o último elemento do memoria e diminui o tamanho
            memoria[0] = memoria[tamanhoMemoria - 1];
            tamanhoMemoria--;
        }

        // Restaura a propriedade do memoria mínimo
        if (tamanhoMemoria > 0)
        {
            memMinimaElementos(memoria, tamanhoMemoria, 0);
        }
    }

    // Fecha todos os arquivos
    for (int i = 0; i < numArquivos; i++)
    {
        fclose(entradas[i]);
    }

    fclose(saida);
}

void memMinimaElementos(ElementoMemoria memoria[], int n, int i)
{
    int menor = i;
    int esq = 2 * i + 1;
    int dir = 2 * i + 2;

    if (esq < n && memoria[esq].cliente.id < memoria[menor].cliente.id)
        menor = esq;
    if (dir < n && memoria[dir].cliente.id < memoria[menor].cliente.id)
        menor = dir;

    if (menor != i)
    {
        ElementoMemoria temp = memoria[i];
        memoria[i] = memoria[menor];
        memoria[menor] = temp;
        memMinimaElementos(memoria, n, menor);
    }
}