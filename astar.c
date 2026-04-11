#include <stdio.h>

#define ROW 10
#define COL 10
#define TAM_FILA (ROW * COL)
#define RAIO_ROBO 1

int preprocessarMapaRoboQuadrado(
    int linhas,
    int colunas,
    const int mapaOriginal[linhas][colunas],
    int mapaProcessado[linhas][colunas],
    int raioRobo
);

int grid[ROW][COL] = {
    {  0,  0,  0,  0,  0,  0,  0,  0,  0, -1},
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  0,  0,  0, -1, -1,  0,  0,  0,  0 },
    {  0,  0,  0,  0, -1, -1,  0,  0,  0,  0 },
    {  0,  0,  0,  0,  0, -1,  0,  0,  0,  0 },
    {  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 },
    {  0,  0,  0,  0,  0, -1,  0,  0,  0,  0 },
    { -1,  0,  0,  0,  0,  0,  0,  0,  0,  0 }
};

int gridProcessado[ROW][COL];
int (*mapaBusca)[COL] = grid;

// pontos de origem e destino
int linhaOrigem = 0;
int colunaOrigem = 0;
int linhaDestino = 9;
int colunaDestino = 9;

int dist[ROW][COL];
int paiLinha[ROW][COL];
int paiColuna[ROW][COL];

// movimentos (considerando as diagonais)
int movimentoLinha[]  = {-1, 1, 0, 0, -1, -1, 1, 1};
int movimentoColuna[] = {0, 0, -1, 1, -1, 1, -1, 1};

// valida se origem e destino estão dentro da matriz e não são obstáculos
int validarPontos() {

    if (linhaOrigem < 0 || linhaOrigem >= ROW || colunaOrigem < 0 || colunaOrigem >= COL) {
        printf("Erro: origem fora dos limites da matriz.\n");
        return 0;
    }

    if (linhaDestino < 0 || linhaDestino >= ROW || colunaDestino < 0 || colunaDestino >= COL) {
        printf("Erro: destino fora dos limites da matriz.\n");
        return 0;
    }

    if (mapaBusca[linhaOrigem][colunaOrigem] == -1) {
        printf("Erro: origem está sobre um obstáculo.\n");
        return 0;
    }

    if (mapaBusca[linhaDestino][colunaDestino] == -1) {
        printf("Erro: destino está sobre um obstáculo.\n");
        return 0;
    }

    if (linhaOrigem == linhaDestino && colunaOrigem == colunaDestino) {
        printf("Erro: origem e destino não podem ser o mesmo ponto.\n");
        return 0;
    }

    return 1;
}

// prepara estruturas de busca
void prepararBusca() {

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            dist[i][j] = -1;
            paiLinha[i][j] = -1;
            paiColuna[i][j] = -1;
        }
    }

    dist[linhaOrigem][colunaOrigem] = 0;
}

// encontra início
void encontrarInicio(int *linhaInicial, int *colunaInicial) {

    *linhaInicial = linhaOrigem;
    *colunaInicial = colunaOrigem;
}

// propagar valores
int propagar(int linhaInicial, int colunaInicial) {

    int filaLinha[TAM_FILA], filaColuna[TAM_FILA];
    int inicioFila = 0, fimFila = 0;

    filaLinha[fimFila] = linhaInicial; 
    filaColuna[fimFila] = colunaInicial;
    fimFila++;

    while (inicioFila < fimFila) { 

        int linhaAtual = filaLinha[inicioFila];
        int colunaAtual = filaColuna[inicioFila];
        inicioFila++;

        for (int i = 0; i < 8; i++) {

            int novaLinha = linhaAtual + movimentoLinha[i];
            int novaColuna = colunaAtual + movimentoColuna[i];

            if (novaLinha >= 0 && novaLinha < ROW && novaColuna >= 0 && novaColuna < COL) { // verifica se nova posição está dentro dos limites da matriz

                // impedir diagonal atravessando obstáculo (se for movimento diagonal, verifica se há obstáculos nas posições adjacentes)
                if (movimentoLinha[i] != 0 && movimentoColuna[i] != 0) { 
                    if (mapaBusca[linhaAtual][novaColuna] == -1 || 
                        mapaBusca[novaLinha][colunaAtual] == -1) {
                        continue;
                    }
                }

                // atualiza apenas células livres ainda não visitadas
                if (mapaBusca[novaLinha][novaColuna] == 0 && dist[novaLinha][novaColuna] == -1) {

                    dist[novaLinha][novaColuna] = dist[linhaAtual][colunaAtual] + 1;
                    paiLinha[novaLinha][novaColuna] = linhaAtual;
                    paiColuna[novaLinha][novaColuna] = colunaAtual;

                    filaLinha[fimFila] = novaLinha; 
                    filaColuna[fimFila] = novaColuna;
                    fimFila++;
                }
            }
        }
    }

    return dist[linhaDestino][colunaDestino] != -1;
}

// reconstruir caminho
void reconstruir() {

    int linhaAtual = linhaDestino; 
    int colunaAtual = colunaDestino;

    printf("\nCaminho encontrado:\n");

    while (!(linhaAtual == linhaOrigem && colunaAtual == colunaOrigem)) {

        printf("(%d, %d)\n", linhaAtual, colunaAtual);

        int proxLinha = paiLinha[linhaAtual][colunaAtual];
        int proxColuna = paiColuna[linhaAtual][colunaAtual];

        if (proxLinha == -1 || proxColuna == -1) {
            printf("Erro: caminho não encontrado!\n");
            return;
        }

        linhaAtual = proxLinha;
        colunaAtual = proxColuna;
    }

    printf("(%d, %d)\n", linhaAtual, colunaAtual);
}

// imprimir matriz
void imprimirGrid() {

    printf("\nMatriz:\n");

    for (int i = 0; i < ROW; i++) { 
        for (int j = 0; j < COL; j++) { 
            if (mapaBusca[i][j] == -1) {
                printf("%3d ", -1);
            } else {
                printf("%3d ", dist[i][j]);
            }
        }
        printf("\n");
    }
}

void imprimirMapaProcessado() {

    printf("\nMapa preprocessado (0 = livre, -1 = obstaculo):\n");

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            printf("%3d ", mapaBusca[i][j]);
        }
        printf("\n");
    }
}

int executarBusca() {
    int linhaInicial, colunaInicial;

    if (!preprocessarMapaRoboQuadrado(ROW, COL, grid, gridProcessado, RAIO_ROBO)) {
        printf("Erro ao pré-processar o mapa.\n");
        return 1;
    }

    mapaBusca = gridProcessado;

    imprimirMapaProcessado();

    if (!validarPontos()) {
        return 1;
    }

    prepararBusca();

    encontrarInicio(&linhaInicial, &colunaInicial);

    if (!propagar(linhaInicial, colunaInicial)) {
        imprimirGrid();
        printf("\nNao existe caminho entre origem e destino.\n");
        return 1;
    }

    imprimirGrid();

    reconstruir();

    return 0;
}

#ifndef ASTAR_NO_MAIN
int main() {
    return executarBusca();
}
#endif