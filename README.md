# ♟️ Xadrez em C com SDL2

Jogo de Xadrez desenvolvido em **Linguagem C** pura com interface gráfica (GUI) utilizando as bibliotecas **SDL2** e **SDL2_image**. 

Projeto criado com objetivo de estudo para aprofundar conceitos de programação modular, manipulação de matrizes bidimensionais, simulação de estados em memória e renderização gráfica acelerada por hardware sem o uso de *game engines* prontas.

---

## Funcionalidades Implementadas

* **Motor Completo de Regras (`regras.c`):**
  * Validação geométrica e de trajetória para todas as peças (Peão, Torre, Cavalo, Bispo, Dama e Rei).
  * Promoção automática de peões ao alcançarem a última fileira.
  * Prevenção de auto-xeque através da simulação de jogadas em cópia do tabuleiro.
  * Detecção automática de **Xeque**, **Xeque-Mate** e **Empate por Afogamento**.
* **Interface Gráfica Interativa (`gui.c`):**
  * Renderização do tabuleiro 8x8 e recorte dinâmico de *sprite sheet* (`.png` / `.webp`).
  * Indicação visual de todas as casas válidas ao selecionar uma peça (pontos centrais para movimento livre e bordas vermelhas para captura).
  * Destaque da última jogada realizada (origem e destino).
  * Alerta visual no Rei quando em situação de xeque e atualização dinâmica do status na barra de título da janela.

---

##  Arquitetura do Projeto

O código foi dividido em módulos independentes para separar a lógica de negócio da camada de apresentação:

```text
├── main.c          # Loop principal, controle de turnos e eventos de mouse
├── tabuleiro.h     # Definições da matriz 8x8 e assinaturas do tabuleiro
├── tabuleiro.c     # Inicialização do estado inicial e conversão de coordenadas
├── regras.h        # Assinaturas de validação de lances, xeque e xeque-mate
├── regras.c        # Lógica das peças, caminho livre e simulação de estado
├── gui.h           # Estruturas da janela, renderizador e mapa de casas válidas
├── gui.c           # Renderização SDL2, recorte de sprites e feedbacks visuais
└── pecas.webp      # Sprite sheet das peças (grade 6x2)
