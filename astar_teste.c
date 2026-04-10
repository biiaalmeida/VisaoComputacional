#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
   Importa todo o algoritmo já existente.
   O arquivo original deve estar no mesmo diretório.
*/
#define ASTAR_NO_MAIN
#include "astar.c"

/*
   Quadrantes:
   Q1 = linhas 0-4, colunas 0-4
   Q2 = linhas 0-4, colunas 5-9
   Q3 = linhas 5-9, colunas 0-4
   Q4 = linhas 5-9, colunas 5-9
*/

void gerarPontoQuadrante(int quadrante, int *linha, int *coluna) {
    int linMin, linMax, colMin, colMax;

    switch (quadrante) {
        case 1:
            linMin = 0; linMax = 4;
            colMin = 0; colMax = 4;
            break;

        case 2:
            linMin = 0; linMax = 4;
            colMin = 5; colMax = 9;
            break;

        case 3:
            linMin = 5; linMax = 9;
            colMin = 0; colMax = 4;
            break;

        case 4:
            linMin = 5; linMax = 9;
            colMin = 5; colMax = 9;
            break;

        default:
            linMin = 0; linMax = 4;
            colMin = 0; colMax = 4;
    }

    do {
        *linha = linMin + rand() % (linMax - linMin + 1);
        *coluna = colMin + rand() % (colMax - colMin + 1);
    } while (grid[*linha][*coluna] == -1);
}

void executarTesteQuadrantes() {
    int totalTestes = 0;
    int sucessos = 0;

    for (int qOrigem = 1; qOrigem <= 4; qOrigem++) {
        for (int qDestino = 1; qDestino <= 4; qDestino++) {

            if (qOrigem == qDestino)
                continue;

            totalTestes++;

            gerarPontoQuadrante(qOrigem, &linhaOrigem, &colunaOrigem);
            gerarPontoQuadrante(qDestino, &linhaDestino, &colunaDestino);

            printf("\n=====================================\n");
            printf("TESTE %d\n", totalTestes);
            printf("Quadrante %d ---> Quadrante %d\n", qOrigem, qDestino);
            printf("Origem : (%d,%d)\n", linhaOrigem, colunaOrigem);
            printf("Destino: (%d,%d)\n", linhaDestino, colunaDestino);
            printf("=====================================\n");

            int resultado = executarBusca();

            if (resultado == 0) {
                sucessos++;
                printf("Resultado: CAMINHO ENCONTRADO\n");
            } else {
                printf("Resultado: SEM CAMINHO\n");
            }
        }
    }

    printf("\n=====================================\n");
    printf("RESUMO FINAL DOS TESTES\n");
    printf("Total de testes: %d\n", totalTestes);
    printf("Sucessos: %d\n", sucessos);
    printf("Falhas: %d\n", totalTestes - sucessos);
    printf("=====================================\n");
}

int main() {
    srand(time(NULL));

    executarTesteQuadrantes();

    return 0;
}