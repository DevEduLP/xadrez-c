#include <stdio.h>
#include "tabuleiro.h"

void inicializar_tabuleiro(char tab[TAM][TAM]) {
    char inicial[TAM][TAM] = {
        {'t', 'c', 'b', 'd', 'r', 'b', 'c', 't'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'T', 'C', 'B', 'D', 'R', 'B', 'C', 'T'}
    };

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            tab[i][j] = inicial[i][j];
        }
    }
}

void imprimir_tabuleiro(const char tab[TAM][TAM]) {
    printf("\n    a b c d e f g h\n");
    printf("  +-----------------+\n");
    for (int i = 0; i < TAM; i++) {
        printf("%d | ", 8 - i);
        for (int j = 0; j < TAM; j++) {
            printf("%c ", tab[i][j]);
        }
        printf("| %d\n", 8 - i);
    }
    printf("  +-----------------+\n");
    printf("    a b c d e f g h\n\n");
}

bool converter_coordenada(const char *str, int *linha, int *coluna) {
    if (str[0] < 'a' || str[0] > 'h' || str[1] < '1' || str[1] > '8' || str[2] != '\0') {
        return false;
    }
    *coluna = str[0] - 'a';
    *linha = 8 - (str[1] - '0');
    return true;
}
