#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <math.h>

#include "quicksort.h"
#include "../Entidades/assinaturas.h"


// void QuickSort(FILE *A, int p, int r){

//     int p, r, q;

//     if (p < r){
//         q = particao(A, p, r);
//         QuickSort(A, p, q - 1);
//         QuickSort(A, q + 1, r);
//     }
// }


// int particao(FILE *A, int p, int r){

//     FILE x;
//     FILE Aux;
//     x = A[r];

//     int i = p - 1;
//     for(int j = p; j < r; j++){
//         if (A[j].Chave <= x.Chave){
//             i = i + 1;
//             Aux = A[i];
//             A[i] = A[j];
//             A[j] = Aux;
//         }
//     }
//     Aux = A[i + 1];
//     A[i + 1] = A[r];
//     A[r] = Aux;
//     return i + 1;
// }





// - tamanho_registro() retorna sizeof(TCliente)
// - TCliente *le(FILE *arq) lê um registro (da posição atual) e retorna um ponteiro alocado
// - void salva(TCliente *item, FILE *arq) escreve o registro (na posição atual) no arquivo


void quickSort(FILE *arq, int p, int r) {
    if (p < r) {
        int q = particao(arq, p, r);
        quickSort(arq, p, q - 1);
        quickSort(arq, q + 1, r);
    }
}

int particao(FILE *arq, int p, int r) {
    
    TCliente x;  // pivô
    TCliente *cliente = (TCliente *)malloc(sizeof(TCliente));

    int i = p - 1;

    // Lê o pivô: registro na posição r
    fseek(arq, (r - 1) * tamanho_registroCliente(), SEEK_SET);
    cliente = leCliente(arq);
    x = *cliente;  // copia o registro para 'x'
    free(cliente);

    for (int j = p; j < r; j++) {
        TCliente *aj = (TCliente *)malloc(sizeof(TCliente));
        // Lê o registro j
        fseek(arq, (j - 1) * tamanho_registroCliente(), SEEK_SET);
        aj = leCliente(arq);
        // Compara pelo campo Chave (ou 'id' se for TCliente)
        if (aj->id <= x.id) {
            i++;
            // Realiza a troca entre os registros nas posições i e j

            // Lê o registro na posição i
            fseek(arq, (i - 1) * tamanho_registroCliente(), SEEK_SET);
            TCliente *cliente_i = leCliente(arq);

            // Escreve o registro j (aj) na posição i
            fseek(arq, (i - 1) * tamanho_registroCliente(), SEEK_SET);
            salvaCliente(aj, arq);

            // Escreve o registro que estava na posição i (cliente_i) na posição j
            fseek(arq, (j - 1) * tamanho_registroCliente(), SEEK_SET);
            salvaCliente(cliente_i, arq);

            free(cliente_i);
        }
        free(aj);
    }

    // Troca o registro da posição i+1 com o pivô (registro r)
    fseek(arq, (i + 1 - 1) * tamanho_registroCliente(), SEEK_SET);
    TCliente *temp_ip1 = leCliente(arq);

    // Escreve o pivô (x) na posição i+1
    fseek(arq, (i + 1 - 1) * tamanho_registroCliente(), SEEK_SET);
    salvaCliente(&x, arq);

    // Escreve o registro que estava em i+1 (temp_ip1) na posição r
    fseek(arq, (r - 1) * tamanho_registroCliente(), SEEK_SET);
    salvaCliente(temp_ip1, arq);

    free(temp_ip1);

    return i + 1;
}

// // Função particao adaptada para arquivo
// int particao(FILE *arq, int p, int r) {

//     // TCliente x = *(TCliente *)malloc(sizeof(194));  // pivô
//     TCliente x;
//     TCliente *cliente = (TCliente *)malloc(sizeof(194));

//     // TCliente x;
//     // TCliente *cliente;

//     int i = p - 1;

//     // Lê o pivô: registro na posição r
//     fseek(arq, (r - 1) * tamanho_registroCliente(), SEEK_SET);
//     cliente = leCliente(arq);
//     x = *cliente;  // copia o registro para 'x'
//     free(cliente);

//     for (int j = p; j < r; j++) {
//         // TCliente *aj;
//         TCliente *aj = (TCliente *)malloc(sizeof(194));
//         // Lê o registro j
//         fseek(arq, (j - 1) * tamanho_registroCliente(), SEEK_SET);
//         aj = leCliente(arq);
//         // Compara pelo campo Chave (ou 'id' se for TCliente)
//         if (aj->id <= x.id) {
//             i++;
//             // Realiza a troca entre os registros nas posições i e j

//             // Lê o registro na posição i
//             fseek(arq, (i - 1) * tamanho_registroCliente(), SEEK_SET);
//             // TCliente *cliente_i = leCliente(arq);
//             TCliente *cliente_i = (TCliente *)malloc(sizeof(194));
//             cliente_i = leCliente(arq);
//             // Lê novamente o registro j (já temos aj, mas para garantir a escrita correta, pode ler novamente ou usar aj)
//             // Aqui, usaremos aj mesmo, pois ele contém o registro j

//             // Escreve o registro j (aj) na posição i
//             fseek(arq, (i - 1) * tamanho_registroCliente(), SEEK_SET);
//             salvaCliente(aj, arq);
//             // Escreve o registro que estava na posição i (cliente_i) na posição j
//             fseek(arq, (j - 1) * tamanho_registroCliente(), SEEK_SET);
//             salvaCliente(cliente_i, arq);
//             free(cliente_i);
//         }
//         free(aj);
//     }
    
//     // Troca o registro da posição i+1 com o pivô (registro r)
//     fseek(arq, (i + 1 - 1) * tamanho_registroCliente(), SEEK_SET);
//     // TCliente *temp_ip1 = leCliente(arq);
//     TCliente *temp_ip1 = (TCliente *)malloc(sizeof(194));
//     temp_ip1 = leCliente(arq);
    
//     // Escreve o pivô (x) na posição i+1
//     fseek(arq, (i + 1 - 1) * tamanho_registroCliente(), SEEK_SET);
//     salvaCliente(&x, arq);
    
//     // Escreve o registro que estava em i+1 (temp_ip1) na posição r
//     fseek(arq, (r - 1) * tamanho_registroCliente(), SEEK_SET);
//     salvaCliente(temp_ip1, arq);
//     free(temp_ip1);
    
//     return i + 1;
// }