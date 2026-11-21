#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "regras.h"

static bool eh_branca(char peca) {
    return isupper((unsigned char)peca);
}

static bool eh_preta(char peca) {
    return islower((unsigned char)peca);
}

static bool caminho_livre(const char tab[TAM][TAM], int l1, int c1, int l2, int c2) {
    int dir_l = (l2 > l1) ? 1 : (l2 < l1 ? -1 : 0);
    int dir_c = (c2 > c1) ? 1 : (c2 < c1 ? -1 : 0);

    int l = l1 + dir_l;
    int c = c1 + dir_c;

    while (l != l2 || c != c2) {
        if (tab[l][c] != '.') return false;
        l += dir_l;
        c += dir_c;
    }
    return true;
}

static bool movimento_pseudo_legal(const char tab[TAM][TAM], int l1, int c1, int l2, int c2, bool turno_brancas) {
    if (l1 < 0 || l1 >= TAM || c1 < 0 || c1 >= TAM ||
        l2 < 0 || l2 >= TAM || c2 < 0 || c2 >= TAM) {
        return false;
    }

    if (l1 == l2 && c1 == c2) return false;

    char peca = tab[l1][c1];
    char destino = tab[l2][c2];

    if (peca == '.') return false;

    if (turno_brancas && !eh_branca(peca)) return false;
    if (!turno_brancas && !eh_preta(peca)) return false;

    if (destino != '.') {
        if (turno_brancas && eh_branca(destino)) return false;
        if (!turno_brancas && eh_preta(destino)) return false;
    }

    int dl = l2 - l1;
    int dc = c2 - c1;
    int abs_dl = abs(dl);
    int abs_dc = abs(dc);

    switch (toupper((unsigned char)peca)) {
        case 'P': {
            int direcao = eh_branca(peca) ? -1 : 1;
            int linha_inicial = eh_branca(peca) ? 6 : 1;

            if (dc == 0 && dl == direcao && destino == '.') return true;
            if (dc == 0 && l1 == linha_inicial && dl == 2 * direcao &&
                destino == '.' && tab[l1 + direcao][c1] == '.') return true;
            if (abs_dc == 1 && dl == direcao && destino != '.') return true;
            return false;
        }
        case 'T':
            if (l1 != l2 && c1 != c2) return false;
            return caminho_livre(tab, l1, c1, l2, c2);
        case 'C':
            return (abs_dl == 2 && abs_dc == 1) || (abs_dl == 1 && abs_dc == 2);
        case 'B':
            if (abs_dl != abs_dc) return false;
            return caminho_livre(tab, l1, c1, l2, c2);
        case 'D':
            if (l1 != l2 && c1 != c2 && abs_dl != abs_dc) return false;
            return caminho_livre(tab, l1, c1, l2, c2);
        case 'R':
            return abs_dl <= 1 && abs_dc <= 1;
    }

    return false;
}

bool rei_em_xeque(const char tab[TAM][TAM], bool turno_brancas) {
    char rei_alvo = turno_brancas ? 'R' : 'r';
    int rei_l = -1, rei_c = -1;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tab[i][j] == rei_alvo) {
                rei_l = i;
                rei_c = j;
                break;
            }
        }
        if (rei_l != -1) break;
    }

    if (rei_l == -1) return true;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (tab[i][j] != '.') {
                bool peca_oponente = turno_brancas ? eh_preta(tab[i][j]) : eh_branca(tab[i][j]);
                if (peca_oponente && movimento_pseudo_legal(tab, i, j, rei_l, rei_c, !turno_brancas)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool validar_movimento(const char tab[TAM][TAM], int l1, int c1, int l2, int c2, bool turno_brancas) {
    if (!movimento_pseudo_legal(tab, l1, c1, l2, c2, turno_brancas)) {
        return false;
    }

    char copia[TAM][TAM];
    memcpy(copia, tab, sizeof(copia));
    mover_peca(copia, l1, c1, l2, c2);

    return !rei_em_xeque(copia, turno_brancas);
}

bool tem_movimentos_legais(const char tab[TAM][TAM], bool turno_brancas) {
    for (int l1 = 0; l1 < TAM; l1++) {
        for (int c1 = 0; c1 < TAM; c1++) {
            char p = tab[l1][c1];
            if (p == '.') continue;
            if (turno_brancas && !eh_branca(p)) continue;
            if (!turno_brancas && !eh_preta(p)) continue;

            for (int l2 = 0; l2 < TAM; l2++) {
                for (int c2 = 0; c2 < TAM; c2++) {
                    if (validar_movimento(tab, l1, c1, l2, c2, turno_brancas)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void mover_peca(char tab[TAM][TAM], int l1, int c1, int l2, int c2) {
    char peca = tab[l1][c1];
    tab[l2][c2] = peca;
    tab[l1][c1] = '.';

    if (peca == 'P' && l2 == 0) tab[l2][c2] = 'D';
    if (peca == 'p' && l2 == 7) tab[l2][c2] = 'd';
}
