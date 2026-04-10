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
   Os quadrantes são calculados dinamicamente a partir de ROW e COL.
   Isso garante que quando a matriz crescer, o teste continua funcionando sem ajustes manuais.
*/

void obterLimitesQuadrante(int quadrante, int *linMin, int *linMax, int *colMin, int *colMax) {
    int metadeLinhas = ROW / 2;
    int metadeColunas = COL / 2;

    int linhaInicialSuperior = 0;
    int linhaFinalSuperior = metadeLinhas - 1;
    int linhaInicialInferior = metadeLinhas;
    int linhaFinalInferior = ROW - 1;

    int colunaInicialEsquerda = 0;
    int colunaFinalEsquerda = metadeColunas - 1;
    int colunaInicialDireita = metadeColunas;
    int colunaFinalDireita = COL - 1;

    switch (quadrante) {
        case 1:
            *linMin = linhaInicialSuperior;
            *linMax = linhaFinalSuperior;
            *colMin = colunaInicialEsquerda;
            *colMax = colunaFinalEsquerda;
            break;

        case 2:
            *linMin = linhaInicialSuperior;
            *linMax = linhaFinalSuperior;
            *colMin = colunaInicialDireita;
            *colMax = colunaFinalDireita;
            break;

        case 3:
            *linMin = linhaInicialInferior;
            *linMax = linhaFinalInferior;
            *colMin = colunaInicialEsquerda;
            *colMax = colunaFinalEsquerda;
            break;

        case 4:
            *linMin = linhaInicialInferior;
            *linMax = linhaFinalInferior;
            *colMin = colunaInicialDireita;
            *colMax = colunaFinalDireita;
            break;

        default:
            *linMin = linhaInicialSuperior;
            *linMax = linhaFinalSuperior;
            *colMin = colunaInicialEsquerda;
            *colMax = colunaFinalEsquerda;
    }
}

void gerarPontoQuadrante(int quadrante, int *linha, int *coluna) {
    int linMin, linMax, colMin, colMax;

    obterLimitesQuadrante(quadrante, &linMin, &linMax, &colMin, &colMax);

    if (linMin > linMax || colMin > colMax) {
        linMin = 0;
        linMax = ROW - 1;
        colMin = 0;
        colMax = COL - 1;
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