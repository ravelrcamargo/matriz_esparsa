#include <stdio.h>
#include <stdlib.h>
#include "matriz.h"

int main()
{
    //começando a declara as matrizes A e B

    matriz matA, matB, matC;

    printf("Lendo matrizA:\n ");

    le_matriz(&matA);
    imprime_matriz(&matA);

    printf("\n");

    printf("Lendo matrizB:\n ");

    le_matriz(&matB);
    imprime_matriz(&matB);

     printf("\n");

    printf("somando matrizes:\n ");

    soma_matriz(&matA, &matB, &matC);
    imprime_matriz(&matC);
    apaga_matriz(&matC);

     printf("\n");

    printf("multiplicando matrizes:\n ");

    multiplica_matriz(&matA, &matB, &matC);
    imprime_matriz(&matC);
    apaga_matriz(&matC);

     printf("\n");

    apaga_matriz(&matA);
    apaga_matriz(&matB);

    return 0;
}
