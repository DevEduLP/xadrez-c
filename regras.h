#ifndef REGRAS_H
#define REGRAS_H

#include <stdbool.h>
#include "tabuleiro.h"

bool validar_movimento(const char tab[TAM][TAM], int l1, int c1, int l2, int c2, bool turno_brancas);
void mover_peca(char tab[TAM][TAM], int l1, int c1, int l2, int c2);
bool rei_em_xeque(const char tab[TAM][TAM], bool turno_brancas);
bool tem_movimentos_legais(const char tab[TAM][TAM], bool turno_brancas);

#endif
