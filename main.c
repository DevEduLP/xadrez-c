#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "tabuleiro.h"
#include "regras.h"
#include "gui.h"

static bool peca_do_turno(char peca, bool turno_brancas) {
    if (peca == '.') return false;
    return turno_brancas ? isupper((unsigned char)peca) : islower((unsigned char)peca);
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;

    char tabuleiro[TAM][TAM];
    inicializar_tabuleiro(tabuleiro);

    InterfaceGUI gui;
    if (!gui_iniciar(&gui, "pecas.webp")) {
        printf("Erro ao inicializar SDL2: %s\n", SDL_GetError());
        return 1;
    }

    bool rodando = true;
    bool fim_de_jogo = false;
    bool turno_brancas = true;
    bool em_xeque = false;
    SDL_Event evento;

    while (rodando) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) {
                rodando = false;
            } else if (!fim_de_jogo && evento.type == SDL_MOUSEBUTTONDOWN && evento.button.button == SDL_BUTTON_LEFT) {
                int c = evento.button.x / TAM_CASA;
                int l = evento.button.y / TAM_CASA;

                if (l < 0 || l >= TAM || c < 0 || c >= TAM) continue;

                char p_clicada = tabuleiro[l][c];

                if (gui.sel_l == -1) {
                    if (peca_do_turno(p_clicada, turno_brancas)) {
                        gui.sel_l = l;
                        gui.sel_c = c;
                        gui_atualizar_validos(&gui, tabuleiro, turno_brancas);
                    }
                } else {
                    if (peca_do_turno(p_clicada, turno_brancas)) {
                        gui.sel_l = l;
                        gui.sel_c = c;
                        gui_atualizar_validos(&gui, tabuleiro, turno_brancas);
                    } else {
                        if (validar_movimento(tabuleiro, gui.sel_l, gui.sel_c, l, c, turno_brancas)) {
                            mover_peca(tabuleiro, gui.sel_l, gui.sel_c, l, c);

                            gui.ult_l1 = gui.sel_l;
                            gui.ult_c1 = gui.sel_c;
                            gui.ult_l2 = l;
                            gui.ult_c2 = c;

                            turno_brancas = !turno_brancas;
                            em_xeque = rei_em_xeque(tabuleiro, turno_brancas);

                            if (!tem_movimentos_legais(tabuleiro, turno_brancas)) {
                                fim_de_jogo = true;
                                if (em_xeque) {
                                    SDL_SetWindowTitle(gui.janela, turno_brancas
                                        ? "XEQUE-MATE! Pretas venceram!"
                                        : "XEQUE-MATE! Brancas venceram!");
                                } else {
                                    SDL_SetWindowTitle(gui.janela, "EMPATE por Afogamento!");
                                }
                            } else {
                                if (em_xeque) {
                                    SDL_SetWindowTitle(gui.janela, turno_brancas
                                        ? "Vez das Brancas - XEQUE!"
                                        : "Vez das Pretas - XEQUE!");
                                } else {
                                    SDL_SetWindowTitle(gui.janela, turno_brancas
                                        ? "Xadrez em C - Vez das Brancas"
                                        : "Xadrez em C - Vez das Pretas");
                                }
                            }
                        }
                        gui.sel_l = -1;
                        gui.sel_c = -1;
                        gui_atualizar_validos(&gui, tabuleiro, turno_brancas);
                    }
                }
            }
        }

        gui_desenhar(&gui, tabuleiro, turno_brancas, em_xeque);
        SDL_Delay(16);
    }

    gui_encerrar(&gui);
    return 0;
}
