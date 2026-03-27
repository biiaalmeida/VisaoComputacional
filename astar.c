#include <stdio.h>

#define ROW 10
#define COL 5

int grid[ROW][COL] = {
    {1, 0, 0, 0, 0},
    {0, -1, -1, 0, 0},
    {0, 0, 0, -1, 0},
    {0, -1, 0, 0, 0},
    {0, -1, 0, -1, 0},
    {0, 0, 0, -1, 0},
    {-1, -1, 0, 0, 0},
    {0, 0, 0, -1, 0},
    {0, -1, 0, 0, 0},
    {0, 0, 0, 0, 0}
};

// destino 
int linhaDestino = 9;
int colunaDestino = 4;

// movimentos (considerando as diagonais)
int movimentoLinha[]  = {-1, 1, 0, 0, -1, -1, 1, 1};
int movimentoColuna[] = {0, 0, -1, 1, -1, 1, -1, 1};

// encontra início
void encontrarInicio(int *linhaInicial, int *colunaInicial) {

    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (grid[i][j] == 1) {
                *linhaInicial = i;
                *colunaInicial = j;
            }
        }
    }
}

// propagar valores
void propagar(int linhaInicial, int colunaInicial) {

    int filaLinha[100], filaColuna[100]; // filas para armazenar as posições a serem processadas
    int inicioFila = 0, fimFila = 0;// índices para controle da fila

    filaLinha[fimFila] = linhaInicial; // adiciona posição inicial à fila
    filaColuna[fimFila] = colunaInicial;
    fimFila++; // incrementa fim da fila

    while (inicioFila < fimFila) { 

        int linhaAtual = filaLinha[inicioFila]; // obtém posição atual da fila
        int colunaAtual = filaColuna[inicioFila];
        inicioFila++; // incrementa início da fila

        for (int i = 0; i < 8; i++) { // para cada movimento possível

            int novaLinha = linhaAtual + movimentoLinha[i]; // calcula nova posição
            int novaColuna = colunaAtual + movimentoColuna[i];

            if (novaLinha >= 0 && novaLinha < ROW && novaColuna >= 0 && novaColuna < COL) { // verifica se nova posição está dentro dos limites da matriz

                // impedir diagonal atravessando obstáculo (se for movimento diagonal, verifica se há obstáculos nas posições adjacentes)
                if (movimentoLinha[i] != 0 && movimentoColuna[i] != 0) { 
                    if (grid[linhaAtual][novaColuna] == -1 || 
                        grid[novaLinha][colunaAtual] == -1) {
                        continue;
                    }
                }

                // se a nova posicao for destino, marca o caminho e termina a propagação
                if (novaLinha == linhaDestino && novaColuna == colunaDestino) { 

                    grid[novaLinha][novaColuna] = grid[linhaAtual][colunaAtual] + 1; 

                    return;
                }

                // se for caminho válido (valor 0), marca a nova posição com o valor do caminho e adiciona à fila para continuar a propagação
                if (grid[novaLinha][novaColuna] == 0) {

                    grid[novaLinha][novaColuna] = grid[linhaAtual][colunaAtual] + 1;

                    filaLinha[fimFila] = novaLinha; 
                    filaColuna[fimFila] = novaColuna;
                    fimFila++;
                }
            }
        }
    }
}

// reconstruir caminho
void reconstruir() {

    // começa do destino e vai para o início, escolhendo a posição com o menor valor a cada passo
    int linhaAtual = linhaDestino; 
    int colunaAtual = colunaDestino;

    printf("\nCaminho encontrado:\n");

    // enquanto não chegar ao início (valor 1), continua procurando o próximo passo
    while (grid[linhaAtual][colunaAtual] != 1) { 

        printf("(%d, %d)\n", linhaAtual, colunaAtual);

        int menorValor = grid[linhaAtual][colunaAtual];
        int melhorLinha = linhaAtual; 
        int melhorColuna = colunaAtual;

        int encontrou = 0; 

        // verifica os 8 movimentos possíveis para encontrar o próximo passo com o menor valor
        for (int i = 0; i < 8; i++) { 

            int novaLinha = linhaAtual + movimentoLinha[i];
            int novaColuna = colunaAtual + movimentoColuna[i];

            if (novaLinha >= 0 && novaLinha < ROW && novaColuna >= 0 && novaColuna < COL) {

                // mesma regra da diagonal
                if (movimentoLinha[i] != 0 && movimentoColuna[i] != 0) {
                    if (grid[linhaAtual][novaColuna] == -1 ||
                        grid[novaLinha][colunaAtual] == -1) {
                        continue;
                    }
                }
                
                // se encontrar um valor menor, atualiza o melhor caminho
                if (grid[novaLinha][novaColuna] > 0 && grid[novaLinha][novaColuna] < menorValor) {

                    menorValor = grid[novaLinha][novaColuna];
                    melhorLinha = novaLinha;
                    melhorColuna = novaColuna;
                    encontrou = 1;
                }
            }
        }

        if (!encontrou) {
            printf("Erro: caminho não encontrado!\n");
            return;
        }

        linhaAtual = melhorLinha;
        colunaAtual = melhorColuna;
    }

    printf("(%d, %d)\n", linhaAtual, colunaAtual);
}

// imprimir matriz
void imprimirGrid() {

    printf("\nMatriz:\n");

    for (int i = 0; i < ROW; i++) { 
        for (int j = 0; j < COL; j++) { 
            printf("%3d ", grid[i][j]);
        }
        printf("\n");
    }
}

int main() {

    int linhaInicial, colunaInicial;

    encontrarInicio(&linhaInicial, &colunaInicial);

    propagar(linhaInicial, colunaInicial);

    imprimirGrid();

    reconstruir();

    return 0;
}