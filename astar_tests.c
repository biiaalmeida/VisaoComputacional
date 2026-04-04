#include <stdio.h>

#define ASTAR_NO_MAIN
#include "astar.c"

typedef struct {
    int linha;
    int coluna;
} Ponto;

typedef struct {
    const char *nome;
    int dados[ROW][COL];
} MatrizTeste;

static void carregarMatrizNoGrid(const int matriz[ROW][COL]) {
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            grid[i][j] = matriz[i][j];
        }
    }
}

static int ehDentro(int linha, int coluna) {
    return linha >= 0 && linha < ROW && coluna >= 0 && coluna < COL;
}

static int bfsReferencia(int origemLinha, int origemColuna, int destinoLinha, int destinoColuna, int *distancia) {
    int distRef[ROW][COL];
    int filaLinha[ROW * COL], filaColuna[ROW * COL];
    int inicio = 0, fim = 0;

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            distRef[i][j] = -1;
        }
    }

    distRef[origemLinha][origemColuna] = 0;
    filaLinha[fim] = origemLinha;
    filaColuna[fim] = origemColuna;
    fim++;

    while (inicio < fim) {
        int linhaAtual = filaLinha[inicio];
        int colunaAtual = filaColuna[inicio];
        inicio++;

        for (int i = 0; i < 8; i++) {
            int novaLinha = linhaAtual + movimentoLinha[i];
            int novaColuna = colunaAtual + movimentoColuna[i];

            if (!ehDentro(novaLinha, novaColuna)) {
                continue;
            }

            if (grid[novaLinha][novaColuna] == -1) {
                continue;
            }

            if (movimentoLinha[i] != 0 && movimentoColuna[i] != 0) {
                if (grid[linhaAtual][novaColuna] == -1 || grid[novaLinha][colunaAtual] == -1) {
                    continue;
                }
            }

            if (distRef[novaLinha][novaColuna] != -1) {
                continue;
            }

            distRef[novaLinha][novaColuna] = distRef[linhaAtual][colunaAtual] + 1;
            filaLinha[fim] = novaLinha;
            filaColuna[fim] = novaColuna;
            fim++;
        }
    }

    *distancia = distRef[destinoLinha][destinoColuna];
    return *distancia != -1;
}

static int testarCombinacao(int origemLinha, int origemColuna, int destinoLinha, int destinoColuna) {
    int esperadoTemCaminho;
    int distanciaEsperada;

    linhaOrigem = origemLinha;
    colunaOrigem = origemColuna;
    linhaDestino = destinoLinha;
    colunaDestino = destinoColuna;

    if (!validarPontos()) {
        return 0;
    }

    esperadoTemCaminho = bfsReferencia(origemLinha, origemColuna, destinoLinha, destinoColuna, &distanciaEsperada);

    prepararBusca();

    if (propagar(origemLinha, origemColuna) != esperadoTemCaminho) {
        return 0;
    }

    if (esperadoTemCaminho && dist[destinoLinha][destinoColuna] != distanciaEsperada) {
        return 0;
    }

    return 1;
}

static int executarTesteExaustivoMatriz(const MatrizTeste *matrizTeste, int *totalAcumulado, int *aprovadosAcumulado) {
    Ponto livres[ROW * COL];
    int quantidadeLivres = 0;
    int total = 0;
    int aprovados = 0;

    carregarMatrizNoGrid(matrizTeste->dados);

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (grid[i][j] == 0) {
                livres[quantidadeLivres].linha = i;
                livres[quantidadeLivres].coluna = j;
                quantidadeLivres++;
            }
        }
    }

    for (int i = 0; i < quantidadeLivres; i++) {
        for (int j = 0; j < quantidadeLivres; j++) {
            int origemLinha = livres[i].linha;
            int origemColuna = livres[i].coluna;
            int destinoLinha = livres[j].linha;
            int destinoColuna = livres[j].coluna;

            if (origemLinha == destinoLinha && origemColuna == destinoColuna) {
                continue;
            }

            total++;

            if (testarCombinacao(origemLinha, origemColuna, destinoLinha, destinoColuna)) {
                aprovados++;
            } else {
                printf("[FALHOU] %s | Origem (%d,%d) -> Destino (%d,%d)\n", matrizTeste->nome, origemLinha, origemColuna, destinoLinha, destinoColuna);
            }
        }
    }

    *totalAcumulado += total;
    *aprovadosAcumulado += aprovados;

    printf("\nMatriz: %s\n", matrizTeste->nome);
    printf("Total de celulas livres: %d\n", quantidadeLivres);
    printf("Total de combinacoes testadas: %d\n", total);
    printf("Resultado: %d/%d combinacoes aprovadas.\n", aprovados, total);

    return aprovados == total;
}

int main() {
    MatrizTeste matrizes[] = {
        {
            "Matriz original",
            {
                {0, 0, 0, 0, 0},
                {0, -1, -1, 0, 0},
                {0, 0, 0, -1, 0},
                {0, -1, 0, 0, 0},
                {0, -1, 0, -1, 0},
                {0, 0, 0, -1, 0},
                {-1, -1, 0, 0, 0},
                {0, 0, 0, -1, 0},
                {0, -1, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        {
            "Matriz com corredor",
            {
                {0, 0, 0, 0, 0},
                {0, -1, -1, -1, 0},
                {0, 0, 0, -1, 0},
                {0, -1, 0, -1, 0},
                {0, -1, 0, 0, 0},
                {0, -1, -1, -1, 0},
                {0, 0, 0, 0, 0},
                {0, -1, -1, -1, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0}
            }
        },
        {
            "Matriz mais bloqueada",
            {
                {0, -1, 0, -1, 0},
                {0, -1, 0, -1, 0},
                {0, 0, 0, -1, 0},
                {-1, -1, 0, -1, 0},
                {0, 0, 0, 0, 0},
                {0, -1, -1, -1, 0},
                {0, 0, 0, 0, 0},
                {0, -1, 0, -1, 0},
                {0, -1, 0, -1, 0},
                {0, 0, 0, 0, 0}
            }
        }
    };

    int quantidadeMatrizes = (int)(sizeof(matrizes) / sizeof(matrizes[0]));
    int totalGeral = 0;
    int aprovadosGeral = 0;
    int matrizesAprovadas = 0;

    for (int i = 0; i < quantidadeMatrizes; i++) {
        if (executarTesteExaustivoMatriz(&matrizes[i], &totalGeral, &aprovadosGeral)) {
            matrizesAprovadas++;
        }
    }

    printf("\n===== RESUMO GERAL =====\n");
    printf("Matrizes aprovadas: %d/%d\n", matrizesAprovadas, quantidadeMatrizes);
    printf("Combinacoes aprovadas: %d/%d\n", aprovadosGeral, totalGeral);

    return (aprovadosGeral == totalGeral) ? 0 : 1;
}