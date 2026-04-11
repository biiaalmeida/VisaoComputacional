#include <stdio.h>

#define CELULA_LIVRE 0
#define CELULA_OBSTACULO -1

static int dentroDosLimites(int linhas, int colunas, int linha, int coluna) {
    return linha >= 0 && linha < linhas && coluna >= 0 && coluna < colunas;
}

int preprocessarMapaRoboQuadrado(
    int linhas,
    int colunas,
    const int mapaOriginal[linhas][colunas],
    int mapaProcessado[linhas][colunas],
    int raioRobo
) {
    if (linhas <= 0 || colunas <= 0 || raioRobo < 0) {
        return 0;
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            mapaProcessado[i][j] = mapaOriginal[i][j];
        }
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (mapaOriginal[i][j] != CELULA_OBSTACULO) {
                continue;
            }

            for (int dl = -raioRobo; dl <= raioRobo; dl++) {
                for (int dc = -raioRobo; dc <= raioRobo; dc++) {
                    const int linhaVizinha = i + dl;
                    const int colunaVizinha = j + dc;

                    if (!dentroDosLimites(linhas, colunas, linhaVizinha, colunaVizinha)) {
                        continue;
                    }

                    mapaProcessado[linhaVizinha][colunaVizinha] = CELULA_OBSTACULO;
                }
            }
        }
    }

    return 1;
}


int preprocessarMapaRoboCircularInPlace(
    int linhas,
    int colunas,
    int mapa[linhas][colunas],
    int raioRobo
) {
    if (linhas <= 0 || colunas <= 0 || raioRobo < 0) {
        return 0;
    }

    int temporario[linhas][colunas];

    if (!preprocessarMapaRoboQuadrado(linhas, colunas, mapa, temporario, raioRobo)) {
        return 0;
    }

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            mapa[i][j] = temporario[i][j];
        }
    }

    return 1;
}

int pontoLivreAposPreprocessamento(
    int linhas,
    int colunas,
    const int mapaProcessado[linhas][colunas],
    int linha,
    int coluna
) {
    if (!dentroDosLimites(linhas, colunas, linha, coluna)) {
        return 0;
    }

    return mapaProcessado[linha][coluna] != CELULA_OBSTACULO;
}

void imprimirMapaPreprocessado(
    int linhas,
    int colunas,
    const int mapa[linhas][colunas]
) {
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            printf("%3d ", mapa[i][j]);
        }
        printf("\n");
    }
}