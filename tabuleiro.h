#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <stdbool.h>

#define TAM 8

void inicializar_tabuleiro(char tab[TAM][TAM]);
void imprimir_tabuleiro(const char tab[TAM][TAM]);
bool converter_coordenada(const char *str, int *linha, int *coluna);

#endif
