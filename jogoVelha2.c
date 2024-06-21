/*
gcc -o jogoVelha2 jogoVelha2.c -lpthread
./jogoVelha2
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define SIZE 4

char tabuleiro[SIZE][SIZE];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void exibirTabuleiro() {
    printf("-----------------------------\n");
    printf("  | A | B | C | D |\n");
    printf("-----------------------------\n");

    for (int i = 0; i < SIZE; ++i) {
        printf("%d |", i + 1);
        for (int j = 0; j < SIZE; ++j) {
            printf("| %c ", tabuleiro[i][j]);
        }
        printf("|\n");
        printf("-----------------------------\n");
    }
}

int jogadaValida(int linha, int coluna) {
    if (linha < 0 || linha >= SIZE || coluna < 0 || coluna >= SIZE) {
        return 0;
    }
    if (tabuleiro[linha][coluna] != ' ') {
        return 0;
    }
    return 1;
}

int verificarVencedor(char jogador) {
    for (int i = 0; i < SIZE; ++i) {
        if (tabuleiro[i][0] == jogador && tabuleiro[i][1] == jogador &&
            tabuleiro[i][2] == jogador && tabuleiro[i][3] == jogador) {
            return 1;
        }
        if (tabuleiro[0][i] == jogador && tabuleiro[1][i] == jogador &&
            tabuleiro[2][i] == jogador && tabuleiro[3][i] == jogador) {
            return 1;
        }
    }
    if (tabuleiro[0][0] == jogador && tabuleiro[1][1] == jogador &&
        tabuleiro[2][2] == jogador && tabuleiro[3][3] == jogador) {
        return 1;
    }
    if (tabuleiro[0][3] == jogador && tabuleiro[1][2] == jogador &&
        tabuleiro[2][1] == jogador && tabuleiro[3][0] == jogador) {
        return 1;
    }
    return 0;
}

void jogadaMaquina() {
    int linha, coluna;
    
    for (linha = 0; linha < SIZE; ++linha) {
        for (coluna = 0; coluna < SIZE; ++coluna) {
            if (jogadaValida(linha, coluna)) {
                tabuleiro[linha][coluna] = 'O';
                if (verificarVencedor('O')) {
                    return;
                }
                tabuleiro[linha][coluna] = 'X';
                if (verificarVencedor('X')) {
                    tabuleiro[linha][coluna] = 'O';
                    return;
                }
                tabuleiro[linha][coluna] = ' ';
            }
        }
    }

    do {
        linha = rand() % SIZE;
        coluna = rand() % SIZE;
    } while (!jogadaValida(linha, coluna));

    tabuleiro[linha][coluna] = 'O';
}

int main() {
    int linha, coluna;
    char jogador = 'X';

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            tabuleiro[i][j] = ' ';
        }
    }

    srand(time(NULL));

    while (1) {
        exibirTabuleiro();

        if (verificarVencedor(jogador)) {
            printf("***TIME '%c' VENCEU!***\n", jogador);
            break;
        }

        int movimentosPossiveis = 0;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (tabuleiro[i][j] == ' ') {
                    movimentosPossiveis = 1;
                    break;
                }
            }
            if (movimentosPossiveis) break;
        }
        if (!movimentosPossiveis) {
            printf("***EMPATE***\n");
            break;
        }

        if (jogador == 'X') {
            printf("JOGUE! (Seu time: %c) (exemplo de entrada: D4): \n", jogador);
            char colunaChar;
            scanf(" %c%d", &colunaChar, &linha);

            linha--;
            coluna = colunaChar - 'A';

            if (!jogadaValida(linha, coluna)) {
                printf("Posicao INEXISTENTE ou OCUPADA! Jogue de novo.\n");
                continue;
            }

            pthread_mutex_lock(&mutex);
            tabuleiro[linha][coluna] = jogador;
            pthread_mutex_unlock(&mutex);

            jogador = 'O';
        } else {
            jogadaMaquina();

            if (verificarVencedor('O')) {
                exibirTabuleiro();
                printf("***TIME O VENCEU!***\n");
                break;
            }

            jogador = 'X';
        }
    }

    exibirTabuleiro();

    printf("Pressione ENTER para sair...\n");
    getchar();
    getchar();

    return 0;
}