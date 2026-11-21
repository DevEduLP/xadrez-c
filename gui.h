#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include "tabuleiro.h"

#define TAM_CASA 80
#define LARGURA_JANELA (TAM * TAM_CASA)
#define ALTURA_JANELA  (TAM * TAM_CASA)

typedef struct {
    SDL_Window *janela;
    SDL_Renderer *render;
    SDL_Texture *textura_pecas;
    int sprite_w;
    int sprite_h;
    int sel_l;
    int sel_c;
    int ult_l1;
    int ult_c1;
    int ult_l2;
    int ult_c2;
    bool validos[TAM][TAM];
} InterfaceGUI;

bool gui_iniciar(InterfaceGUI *gui, const char *caminho_sprite);
void gui_atualizar_validos(InterfaceGUI *gui, const char tab[TAM][TAM], bool turno_brancas);
void gui_desenhar(InterfaceGUI *gui, const char tab[TAM][TAM], bool turno_brancas, bool em_xeque);
void gui_encerrar(InterfaceGUI *gui);

#endif
