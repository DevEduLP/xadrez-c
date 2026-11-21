#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "gui.h"
#include "regras.h"

bool gui_iniciar(InterfaceGUI *gui, const char *caminho_sprite) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) return false;

    int flags = IMG_INIT_PNG | IMG_INIT_WEBP;
    if ((IMG_Init(flags) & flags) == 0) {
        printf("Erro IMG_Init: %s\n", IMG_GetError());
        return false;
    }

    gui->janela = SDL_CreateWindow(
        "Xadrez em C - Vez das Brancas",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LARGURA_JANELA, ALTURA_JANELA,
        SDL_WINDOW_SHOWN
    );
    if (!gui->janela) return false;

    gui->render = SDL_CreateRenderer(gui->janela, -1, SDL_RENDERER_ACCELERATED);
    if (!gui->render) return false;

    SDL_SetRenderDrawBlendMode(gui->render, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    gui->textura_pecas = IMG_LoadTexture(gui->render, caminho_sprite);
    if (!gui->textura_pecas) {
        printf("Erro ao carregar '%s': %s\n", caminho_sprite, IMG_GetError());
        return false;
    }

    int img_w, img_h;
    SDL_QueryTexture(gui->textura_pecas, NULL, NULL, &img_w, &img_h);
    gui->sprite_w = img_w / 6;
    gui->sprite_h = img_h / 2;

    gui->sel_l = -1;
    gui->sel_c = -1;
    gui->ult_l1 = -1;
    gui->ult_c1 = -1;
    gui->ult_l2 = -1;
    gui->ult_c2 = -1;
    memset(gui->validos, 0, sizeof(gui->validos));

    return true;
}

void gui_atualizar_validos(InterfaceGUI *gui, const char tab[TAM][TAM], bool turno_brancas) {
    memset(gui->validos, 0, sizeof(gui->validos));
    if (gui->sel_l == -1 || gui->sel_c == -1) return;

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            if (validar_movimento(tab, gui->sel_l, gui->sel_c, i, j, turno_brancas)) {
                gui->validos[i][j] = true;
            }
        }
    }
}

static void desenhar_peca(InterfaceGUI *gui, char peca, int l, int c) {
    if (peca == '.') return;

    int linha_sprite = isupper((unsigned char)peca) ? 0 : 1;
    int coluna_sprite = 0;

    switch (toupper((unsigned char)peca)) {
        case 'R': coluna_sprite = 0; break;
        case 'D': coluna_sprite = 1; break;
        case 'B': coluna_sprite = 2; break;
        case 'C': coluna_sprite = 3; break;
        case 'T': coluna_sprite = 4; break;
        case 'P': coluna_sprite = 5; break;
        default: return;
    }

    SDL_Rect src = {
        coluna_sprite * gui->sprite_w,
        linha_sprite * gui->sprite_h,
        gui->sprite_w,
        gui->sprite_h
    };

    int margem = 6;
    SDL_Rect dst = {
        c * TAM_CASA + margem,
        l * TAM_CASA + margem,
        TAM_CASA - (2 * margem),
        TAM_CASA - (2 * margem)
    };

    SDL_RenderCopy(gui->render, gui->textura_pecas, &src, &dst);
}

void gui_desenhar(InterfaceGUI *gui, const char tab[TAM][TAM], bool turno_brancas, bool em_xeque) {
    char rei_turno = turno_brancas ? 'R' : 'r';

    for (int i = 0; i < TAM; i++) {
        for (int j = 0; j < TAM; j++) {
            SDL_Rect casa = { j * TAM_CASA, i * TAM_CASA, TAM_CASA, TAM_CASA };

            if ((i + j) % 2 == 0) {
                SDL_SetRenderDrawColor(gui->render, 238, 238, 210, 255);
            } else {
                SDL_SetRenderDrawColor(gui->render, 118, 150, 86, 255);
            }
            SDL_RenderFillRect(gui->render, &casa);

            if ((i == gui->ult_l1 && j == gui->ult_c1) ||
                (i == gui->ult_l2 && j == gui->ult_c2)) {
                SDL_SetRenderDrawColor(gui->render, 246, 246, 105, 130);
                SDL_RenderFillRect(gui->render, &casa);
            }

            if (i == gui->sel_l && j == gui->sel_c) {
                SDL_SetRenderDrawColor(gui->render, 246, 246, 105, 210);
                SDL_RenderFillRect(gui->render, &casa);
            }

            if (em_xeque && tab[i][j] == rei_turno) {
                SDL_SetRenderDrawColor(gui->render, 220, 45, 45, 200);
                SDL_RenderFillRect(gui->render, &casa);
            }

            desenhar_peca(gui, tab[i][j], i, j);

            if (gui->validos[i][j]) {
                if (tab[i][j] == '.') {
                    SDL_SetRenderDrawColor(gui->render, 30, 30, 30, 90);
                    SDL_Rect ponto = {
                        j * TAM_CASA + (TAM_CASA / 2) - 10,
                        i * TAM_CASA + (TAM_CASA / 2) - 10,
                        20, 20
                    };
                    SDL_RenderFillRect(gui->render, &ponto);
                } else {
                    SDL_SetRenderDrawColor(gui->render, 210, 40, 40, 200);
                    for (int b = 0; b < 4; b++) {
                        SDL_Rect borda = {
                            j * TAM_CASA + b,
                            i * TAM_CASA + b,
                            TAM_CASA - 2 * b,
                            TAM_CASA - 2 * b
                        };
                        SDL_RenderDrawRect(gui->render, &borda);
                    }
                }
            }
        }
    }
    SDL_RenderPresent(gui->render);
}

void gui_encerrar(InterfaceGUI *gui) {
    if (gui->textura_pecas) SDL_DestroyTexture(gui->textura_pecas);
    if (gui->render) SDL_DestroyRenderer(gui->render);
    if (gui->janela) SDL_DestroyWindow(gui->janela);
    IMG_Quit();
    SDL_Quit();
}
