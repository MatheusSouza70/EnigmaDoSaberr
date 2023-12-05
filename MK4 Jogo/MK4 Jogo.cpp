#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <cmath>
#include <errno.h>
#include <stdio.h>
#include <string>

int tela_largura = 1366;
int tela_altura = 768;
const int RECTANGLE_WIDTH = 50;
const int RECTANGLE_HEIGHT = 50;
int player_speed = 5;
int EstadoTela;
int largura_sprite_hp;
int altura_sprite_hp;

enum estado {
  menuprincipal,
  jogando,
  dialogo,
  controles,
  creditos,
  puzzle,
  puzzle2,
  puzzle3
};

void init() {
  al_init();
  al_init_image_addon();
  al_init_ttf_addon();
  al_install_keyboard();
  al_install_mouse();
  al_init_font_addon();
}

struct objeto {
  int x;
  int y;
  int largura;
  int altura;
  ALLEGRO_BITMAP *imagem;
};

typedef struct objeto Objeto;

Objeto *player, *npc, *npc2;

typedef struct Rectangle {
  float x, y;
  float width, height;
  ALLEGRO_COLOR color;
} Rect;

Rect *rect1, *rect2;

typedef struct Obstacle {
  float x, y;
  float width, height;
  ALLEGRO_COLOR color;
} Obstacle;

Obstacle obstacles[] = {
    {100, 100, 50, 50, al_map_rgb(255, 255, 0)},    // Obstáculo 1
    {350, 200, 80, 30, al_map_rgb(0, 255, 0)},      // Obstáculo 2
    {120, 320, 70, 40, al_map_rgb(0, 0, 255)},      // Obstáculo 3
    {510, 80, 45, 70, al_map_rgb(255, 0, 255)},     // Obstáculo 4
    {210, 400, 65, 55, al_map_rgb(0, 255, 255)},    // Obstáculo 5
    {400, 360, 40, 80, al_map_rgb(255, 0, 0)},      // Obstáculo 6
    {60, 480, 95, 20, al_map_rgb(0, 255, 0)},       // Obstáculo 7
    {550, 270, 50, 50, al_map_rgb(255, 255, 0)},    // Obstáculo 8
    {330, 110, 90, 30, al_map_rgb(0, 0, 255)},      // Obstáculo 9
    {190, 40, 35, 110, al_map_rgb(255, 0, 0)},      // Obstáculo 10
    {420, 420, 80, 70, al_map_rgb(0, 255, 255)},    // Obstáculo 11
    {80, 330, 85, 25, al_map_rgb(255, 255, 0)},     // Obstáculo 12
    {260, 180, 50, 90, al_map_rgb(120, 120, 120)},  // Obstáculo 13
    {380, 150, 65, 70, al_map_rgb(200, 100, 50)},   // Obstáculo 14
    {110, 400, 60, 40, al_map_rgb(50, 200, 100)},   // Obstáculo 15
    {800, 600, 120, 40, al_map_rgb(200, 50, 150)},  // Obstáculo 16
    {1020, 300, 50, 100, al_map_rgb(100, 200, 50)}, // Obstáculo 17
    {1150, 150, 80, 80, al_map_rgb(50, 50, 200)},   // Obstáculo 18
    {1200, 450, 30, 60, al_map_rgb(200, 100, 100)}, // Obstáculo 19
    {1050, 500, 70, 50, al_map_rgb(150, 200, 50)},  // Obstáculo 20
    {670, 420, 100, 90, al_map_rgb(100, 150, 200)}, // Obstáculo 21
    {800, 250, 120, 60, al_map_rgb(200, 50, 100)},  // Obstáculo 22
    {950, 50, 40, 120, al_map_rgb(80, 120, 200)},   // Obstáculo 23
    {1150, 500, 70, 30, al_map_rgb(160, 200, 80)},  // Obstáculo 24
    {800, 200, 70, 70, al_map_rgb(100, 0, 150)},    // Obstáculo 25
    {650, 350, 90, 40, al_map_rgb(200, 100, 50)},   // Obstáculo 26
    {550, 600, 120, 30, al_map_rgb(50, 150, 200)},  // Obstáculo 27
    {100, 650, 50, 60, al_map_rgb(150, 200, 50)},   // Obstáculo 28
    {950, 250, 40, 90, al_map_rgb(100, 150, 200)},  // Obstáculo 29
    {1200, 100, 80, 80, al_map_rgb(80, 120, 200)},  // Obstáculo 30
};

const int numObstacles = sizeof(obstacles) / sizeof(obstacles[0]);

void LimpaTela() { al_clear_to_color(al_map_rgb(0, 0, 0)); }

void GameMenu() {
  LimpaTela();
  ALLEGRO_FONT *fonte = al_load_font("./font.ttf", 25, 0);
  ALLEGRO_FONT *fonte_grande = al_load_font("./font.ttf", 45, 0);
  al_draw_text(fonte_grande, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 5, ALLEGRO_ALIGN_CENTER, "ENIGMA DO SABER");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 3, ALLEGRO_ALIGN_CENTER, "1. Jogar");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 2, ALLEGRO_ALIGN_CENTER, "2. Controles");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 1.5, ALLEGRO_ALIGN_CENTER, "3. Créditos");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 1.2, ALLEGRO_ALIGN_CENTER, "ESC. Sair");
}

void Controles() {
  LimpaTela();
  ALLEGRO_FONT *fonte = al_load_font("./font.ttf", 25, 0);
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 5, ALLEGRO_ALIGN_CENTER, "Controles");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 3, ALLEGRO_ALIGN_CENTER,
               "Seta para Cima - Mover para cima");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 2.5, ALLEGRO_ALIGN_CENTER,
               "Seta para Baixo - Mover para baixo");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 2, ALLEGRO_ALIGN_CENTER,
               "Seta para Esquerda - Mover para esquerda");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 1.8, ALLEGRO_ALIGN_CENTER,
               "Seta para Direita - Mover para direita");
  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura / 1.5, ALLEGRO_ALIGN_CENTER,
               "Tecla E - Iniciar diálogo");

  al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
               tela_altura - 50, ALLEGRO_ALIGN_CENTER,
               "Pressione BACKSPACE para voltar ao menu principal");

  EstadoTela = controles;
}

void Creditos() {
    LimpaTela();
    ALLEGRO_FONT* fonte = al_load_font("./font.ttf", 25, 0);
    al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
        tela_altura / 5, ALLEGRO_ALIGN_CENTER, "Créditos");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
        tela_altura / 3.5, ALLEGRO_ALIGN_CENTER, "Desenvolvido por:");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
        tela_altura / 2.8, ALLEGRO_ALIGN_CENTER,
        "Eric Donato Pulcino dos Santos");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
        tela_altura / 2.4, ALLEGRO_ALIGN_CENTER,
        "Kaue Oliveira dos Anjos");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
        tela_altura / 2.1, ALLEGRO_ALIGN_CENTER, "Matheus Dutra Souza");
    al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
        tela_altura / 1.9, ALLEGRO_ALIGN_CENTER,
        "Vinicius Kitamukai Vazzoler");

    al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
        tela_altura - 50, ALLEGRO_ALIGN_CENTER,
        "Pressione BACKSPACE para voltar ao menu principal");

    EstadoTela = creditos;

};

  
   

int Collide(Rectangle *rect1, Rectangle *rect2) {

  if (rect1->x + RECTANGLE_WIDTH >= rect2->x &&
      rect1->x <= rect2->x + RECTANGLE_WIDTH &&
      rect1->y + RECTANGLE_HEIGHT >= rect2->y &&
      rect1->y <= rect2->y + RECTANGLE_HEIGHT) {
    return 1; // Há colisão.

    /*
    verifica se dois retângulos, representados pelos ponteiros rect1 e rect2,
    estão colidindo ou sobrepostos no plano. Ele usa condições para verificar
    se as bordas dos retângulos se cruzam. rect1->x + RECTANGLE_WIDTH >=
    rect2->x: Esta condição verifica se o lado direito do primeiro retângulo
    (rect1) está à direita ou coincide com o lado esquerdo do segundo
    retângulo (rect2). rect1->x <= rect2->x + RECTANGLE_WIDTH: Esta condição
    verifica se o lado esquerdo do primeiro retângulo (rect1) está à esquerda
    ou coincide com o lado direito do segundo retângulo (rect2). rect1->y +
    RECTANGLE_HEIGHT >= rect2->y: Esta condição verifica se a parte inferior
    do primeiro retângulo (rect1) está abaixo ou coincide com a parte
    superior do segundo retângulo (rect2). rect1->y + RECTANGLE_HEIGHT >=
    rect2->y: Esta condição verifica se a parte inferior do primeiro
     retângulo (rect1) está abaixo ou coincide com a parte superior do segundo
     retângulo (rect2).
     */
  }
  return 0; // Não há colisão.
}

bool Collide(const Rect *rect1, const Rect *rect2, const Obstacle *obstacles,
             int numObstacles) {
  // Verifica a colisão entre rect1 e rect2
  if (rect1->x + rect1->width >= rect2->x &&
      rect2->x + rect2->width >= rect1->x &&
      rect1->y + rect1->height >= rect2->y &&
      rect2->y + rect2->height >= rect1->y) {
    return true; // Há colisão entre rect1 e rect2
  }

  // Verifica a colisão entre os retângulos e os obstáculos
  for (int i = 0; i < numObstacles; ++i) {
    const Obstacle *obstacle = &obstacles[i];
    if (rect1->x + rect1->width >= obstacle->x &&
        obstacle->x + obstacle->width >= rect1->x &&
        rect1->y + rect1->height >= obstacle->y &&
        obstacle->y + obstacle->height >= rect1->y) {
      return true; // Há colisão entre rect1 e o obstáculo atual
    }

    if (rect2->x + rect2->width >= obstacle->x &&
        obstacle->x + obstacle->width >= rect2->x &&
        rect2->y + rect2->height >= obstacle->y &&
        obstacle->y + obstacle->height >= rect2->y) {
      return true; // Há colisão entre rect2 e o obstáculo atual
    }
  }

  return false; // Não há colisão entre retângulos e obstáculos
}

void DrawRectangle(Rectangle *rect) {
  al_draw_filled_rectangle(rect->x, rect->y, rect->x + RECTANGLE_WIDTH,
                           rect->y + RECTANGLE_HEIGHT, rect->color);
}

void DesenhaObstaculos(const Obstacle *obstacles, int numObstacles) {
  for (int i = 0; i < numObstacles; ++i) {
    const Obstacle *obstacle = &obstacles[i];
    al_draw_filled_rectangle(
        obstacle->x, obstacle->y, obstacle->x + obstacle->width,
        obstacle->y + obstacle->height,
        al_map_rgb(255, 255, 255)); // Defina a cor desejada
  }
}

void ResetRectangles(Rect *rect1, Rect *rect2) {
  // Defina as coordenadas iniciais dos retângulos
  rect1->x = 0;
  rect1->y = 0;
  rect2->x = 500;
  rect2->y = 500;
}

int main() {
  init();
  int pontos = 0;
  ALLEGRO_FONT *fonte = al_load_font("./font.ttf", 25, 0);
  if (!fonte) {
    fprintf(stderr, "Falha ao carregar a fonte.\n");
    return -1;
  }

  ALLEGRO_TIMER *timer = al_create_timer(1.0 / 30);
  if (!timer) {
    fprintf(stderr, "Falha ao carregar o timer.\n");
    return -1;
  }

  ALLEGRO_BITMAP *playerSprite = al_load_bitmap("./clarinho.png");
  if (!playerSprite) {
    fprintf(stderr, "Falha ao carregar o sprite do jogador.\n");
    return -1;
  }

  ALLEGRO_BITMAP *npcSprite1 = al_load_bitmap("./quintas.png");
  if (!npcSprite1) {
    fprintf(stderr, "Falha ao carregar o sprite do NPC.\n");
    return -1;
  }

  ALLEGRO_BITMAP* npcSprite2 = al_load_bitmap("./rey.png");
  if (!npcSprite2) {
      fprintf(stderr, "Falha ao carregar o sprite do NPC.\n");
      return -1;
  }

  ALLEGRO_BITMAP *background = al_load_bitmap("./background.jpeg");
  if (!background) {
    fprintf(stderr, "Falha ao carregar a imagem de fundo.\n");
    return -1;
  }

  ALLEGRO_BITMAP *menuBackground = al_load_bitmap("./mainmenu.jpeg");
  if (!menuBackground) {
    fprintf(stderr, "Falha ao carregar a imagem de fundo do menu.\n");
    return -1;
  }

  ALLEGRO_BITMAP *button_e = al_load_bitmap("./button_e.png");
  if (!button_e) {
    fprintf(stderr, "Falha ao carregar a imagem do botão E.\n");
    return -1;
  }

  const char *Dialogo[] = {
      "Olá, eu sou o Thyago Quintas!",
      "Estou muito machucado para fazer algumas coisas manuais...",
      "Posso pedir uma ajuda sua? Preciso conectar dois blocos de energia! "
      "Conto com você.",
      "Muito obrigado! Talvez seja interessante você checar aquele cara ali no "
      "moinho..."};

  const char *Dialogo_npc2[] = {
      "Oi, eu sou Guilherme Rey!", "Você pode me ajudar?",
      "Preciso resolver uma equação! Dá uma olhadinha..."
      "Conto com você.",
      "Muito obrigado!"};

  Rect rect1 = {0, 0, /* Define a posição e tamanho do primeiro retângulo */
                50,
                50, /* Aqui, usei valores arbitrários para largura e altura */
                al_map_rgb(255, 0, 0)}; // Cor vermelha
  Rect rect2 = {1300, 725, /* Define a posição e tamanho do segundo retângulo */
                50,
                50, /* Aqui, usei valores arbitrários para largura e altura */
                al_map_rgb(0, 0, 255)}; // Cor verde

  int dragging = 0; // Flag indicando se o triângulo está sendo arrastado.

  int num_linhas = sizeof(Dialogo) / sizeof(Dialogo[0]);
  int num_linhas2 = sizeof(Dialogo_npc2) / sizeof(Dialogo_npc2[0]);

  // Inicialização da Allegro
  if (!al_init()) {
    fprintf(stderr, "Falha ao inicializar a Allegro.\n");
    return -1;
  }

  // Inicialização do addon para primitivas gráficas
  if (!al_init_primitives_addon()) {
    fprintf(stderr, "Falha ao inicializar o addon de primitivas gráficas.\n");
    return -1;
  }

  ALLEGRO_DISPLAY *display = al_create_display(tela_largura, tela_altura);
  if (!display) {
    fprintf(stderr, "Falha ao criar a janela.\n");
    return -1;
  }

  ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
  if (!event_queue) {
    fprintf(stderr, "Falha ao criar a fila de eventos.\\n");
    return -1;
  }

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_mouse_event_source());
  al_register_event_source(event_queue, al_get_timer_event_source(timer));

  // stats player
  player = (Objeto *)malloc(sizeof(Objeto));
  player->imagem = al_load_bitmap("./clarinho.png");
  player->x = 0;
  player->y = 325;
  player->largura = 50;
  player->altura = 50;

  npc = (Objeto *)malloc(sizeof(Objeto));
  npc->imagem = al_load_bitmap("./quintas.png");
  npc->x = 102.5;
  npc->y = 280;
  npc->largura = 50;
  npc->altura = 50;

  npc2 = (Objeto *)malloc(sizeof(Objeto));
  npc2->imagem = al_load_bitmap("./rey.png");
  npc2->x = 975;
  npc2->y = 300;
  npc2->largura = 50;
  npc2->altura = 50;


  al_draw_filled_rectangle(npc->x, npc->y, npc->x + npc->largura,
                           npc->y + npc->altura, al_map_rgb(255, 0, 0)); // npc1
  al_draw_filled_rectangle(npc2->x, npc2->y, npc2->x + npc2->largura,
                           npc2->y + npc2->altura,
                           al_map_rgb(255, 0, 0)); // npc2
  al_draw_filled_rectangle(player->x, player->y, player->x + player->largura,
                           player->y + player->altura,
                           al_map_rgb(0, 0, 255)); // player

  estado EstadoTela = menuprincipal;
  GameMenu();
  bool sair = false;
  bool colisao_anterior = false;
  int dialogo_atual = 0;
  int dialogo_atual2 = 0;
  int dialogo_atual3 = 0;
  bool jogo_finalizado = false;
  bool jogo_finalizado2 = false;
  bool jogo_finalizado3 = false;
  int vidas = 3;
  bool key_right = false, key_left = false, key_down = false, key_up = false;
  float frame = 0.0f;
  int current_frame_y = 148;
  int spriteWidth = 105;
  int spriteHeight = 148;
  bool is_any_key_pressed = false;
  bool principal = false;
  int displayWidth = al_get_display_width(display);
  int displayHeight = al_get_display_height(display);
 
  al_start_timer(timer);

  srand(time(NULL));
  int randU = rand() % 100 + 3;
  int randI = rand() % 100 + 3;
  int btnCerto = rand() % 3 + 1;

  while (randI > randU) {
    randI = rand() % 100 + 3;
  }
  int R = randU / randI;
  bool exibir_parabens = false;
  bool exibir_gameover = false;
  while (!sair) {
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);
    // Movimentacao
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
      sair = true;
    }

    if (EstadoTela == menuprincipal) {
      al_draw_scaled_bitmap(menuBackground, 0, 0,
                            al_get_bitmap_width(menuBackground),
                            al_get_bitmap_height(menuBackground), 0, 0,
                            tela_largura / 2.5, tela_altura / 1.25, 0);
      principal = true;
      if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode == ALLEGRO_KEY_1 && principal == true) {
          EstadoTela = jogando;
          principal = false;
        } else if (event.keyboard.keycode == ALLEGRO_KEY_2 &&
                   principal == true) {
          Controles();
          principal == false;
        } else if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
          GameMenu();
          principal == true;
        } else if (event.keyboard.keycode == ALLEGRO_KEY_3 &&
                   principal == true) {
          Creditos();
          principal == false;
        } else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE &&
                   EstadoTela == menuprincipal) {
          sair = true;
        }
      }
    }

    else if (EstadoTela == jogando) {
      bool tecla_e_pressionada = false;
      if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
          key_up = true;
          break;
        case ALLEGRO_KEY_DOWN:
          key_down = true;
          break;
        case ALLEGRO_KEY_LEFT:
          key_left = true;
          break;
        case ALLEGRO_KEY_RIGHT:
          key_right = true;
          break;
        case ALLEGRO_KEY_E:
          tecla_e_pressionada = true;
          break;
        }

      } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
        switch (event.keyboard.keycode) {
        case ALLEGRO_KEY_UP:
          key_up = false;
          break;
        case ALLEGRO_KEY_DOWN:
          key_down = false;
          break;
        case ALLEGRO_KEY_LEFT:
          key_left = false;
          break;
        case ALLEGRO_KEY_RIGHT:
          key_right = false;
          break;
        }
      }

      if (key_up) {
        player->y -= player_speed;
        current_frame_y = 0;
      }
      if (key_down) {
        player->y += player_speed;
        current_frame_y = 148 * 2;
      }
      if (key_left) {
        player->x -= player_speed;
        current_frame_y = 148 * 3;
      }
      if (key_right) {
        player->x += player_speed;
        current_frame_y = 148;
      }

      // quantidade de frames
      frame += 0.3f;
      if (frame > 4) {
        frame -= 4;
      }

      if (player->x < 0) {
        player->x = 0; // Não permita que o sprite saia pela esquerda
      }

      else if (player->x > tela_largura - spriteWidth) {
        player->x = tela_largura -
                    spriteWidth; // Não permita que o sprite saia pela direita
      }

      if (player->y < 0) {
        player->y = 0; // Não permita que o sprite saia por cima
      } else if (player->y > tela_altura - spriteHeight) {
        player->y = tela_altura -
                    spriteHeight; // Não permita que o sprite saia por baixo
      }

      // Verificação de colisão entre o jogador e npc
      bool colisao = (player->x < npc->x + npc->largura &&
                      player->x + player->largura > npc->x &&
                      player->y < npc->y + npc->altura &&
                      player->y + player->altura > npc->y);

      // Verificação de colisão entre o jogador e npc2
      bool colisao_npc2 = (player->x < npc2->x + npc2->largura &&
                           player->x + player->largura > npc2->x &&
                           player->y < npc2->y + npc2->altura &&
                           player->y + player->altura > npc2->y);

      // Limpa a tela
      al_clear_to_color(al_map_rgb(0, 0, 0));
      al_draw_scaled_bitmap(background, 0, 0, al_get_bitmap_width(background),
                            al_get_bitmap_height(background), 0, 0,
                            tela_largura, tela_altura, 0);

      al_draw_scaled_bitmap(npcSprite1, 0, 0, al_get_bitmap_width(npcSprite1),
                            al_get_bitmap_height(npcSprite1), npc->x, npc->y, 45,
                            55, 0);

      al_draw_scaled_bitmap(npcSprite2, 0, 0, al_get_bitmap_width(npcSprite2),
                            al_get_bitmap_height(npcSprite2), npc2->x, npc2->y,
                            40, 50, 0);

      al_draw_scaled_bitmap(playerSprite, 105 * (int)frame, current_frame_y,
                            spriteWidth, spriteHeight, player->x, player->y, 45,
                            55, 0);

      if (colisao && EstadoTela == jogando) {
        if (tecla_e_pressionada) {
          // Se a tecla E foi pressionada, exibe o próximo trecho do diálogo
          if (dialogo_atual < 3) {
            al_draw_filled_rectangle(0, tela_altura / 1.3, tela_largura,
                                     tela_altura, al_map_rgb(0, 0, 200));
            al_draw_filled_rectangle(3, tela_altura / 1.3 + 3, tela_largura - 3,
                                     tela_altura - 3, al_map_rgb(0, 0, 0));
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 13,
                         tela_altura / 1.3 + 13, ALLEGRO_ALIGN_LEFT,
                         Dialogo[dialogo_atual]);
            al_flip_display();
            dialogo_atual++;
            al_rest(1.5); // Espera por 2 segundos entre cada parte do diálogo
            tecla_e_pressionada = false;
          }

          else if (!jogo_finalizado) {
            dialogo_atual = 0; // Reseta o diálogo quando terminar
            EstadoTela = puzzle;
          }

          else if (dialogo_atual < num_linhas && jogo_finalizado == true) {
            al_draw_filled_rectangle(0, tela_altura / 1.3, tela_largura,
                                     tela_altura, al_map_rgb(0, 0, 200));
            al_draw_filled_rectangle(3, tela_altura / 1.3 + 3, tela_largura - 3,
                                     tela_altura - 3, al_map_rgb(0, 0, 0));
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 13,
                         tela_altura / 1.3 + 13, ALLEGRO_ALIGN_LEFT,
                         Dialogo[dialogo_atual]);
          }
        } else {
          // Desenha o botao
          al_draw_bitmap(button_e, player->x, player->y - 40, NULL);
        }

        colisao_anterior = colisao;
      }

      if (colisao_npc2 && EstadoTela == jogando) {
        if (tecla_e_pressionada) {
          // Se a tecla E foi pressionada, exibe o próximo trecho do diálogo
          if (dialogo_atual2 < 3) {
            al_draw_filled_rectangle(0, tela_altura / 1.3, tela_largura,
                                     tela_altura, al_map_rgb(0, 0, 200));
            al_draw_filled_rectangle(3, tela_altura / 1.3 + 3, tela_largura - 3,
                                     tela_altura - 3, al_map_rgb(0, 0, 0));
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 13,
                         tela_altura / 1.3 + 13, ALLEGRO_ALIGN_LEFT,
                         Dialogo_npc2[dialogo_atual2]);
            al_flip_display();
            dialogo_atual2++;
            al_rest(1.5); // Espera por 2 segundos entre cada parte do diálogo
            tecla_e_pressionada = false;
          }

          else if (!jogo_finalizado2) {
            // Reseta o diálogo quando terminar e muda o estado do jogo
            dialogo_atual2 = 0;
            EstadoTela = puzzle2;
          }

          else if (dialogo_atual2 < num_linhas2 && jogo_finalizado2 == true) {
            al_draw_filled_rectangle(0, tela_altura / 1.3, tela_largura,
                                     tela_altura, al_map_rgb(0, 0, 200));
            al_draw_filled_rectangle(3, tela_altura / 1.3 + 3, tela_largura - 3,
                                     tela_altura - 3, al_map_rgb(0, 0, 0));
            al_draw_text(fonte, al_map_rgb(255, 255, 255), 13,
                         tela_altura / 1.3 + 13, ALLEGRO_ALIGN_LEFT,
                         Dialogo_npc2[dialogo_atual2]);
          }
        } else {
          // Desenha o botao
          al_draw_bitmap(button_e, player->x, player->y - 40, NULL);
        }

        colisao_anterior = colisao_npc2;
      }

    }

    else if (EstadoTela == controles) {
   
    } else if (EstadoTela == creditos) {
      
    } else if (EstadoTela == puzzle) {
      // Código para mostrar informações de puzzle
      if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

        if (event.mouse.button == 1) { // clique esquerdo.
          if (event.mouse.x >= rect1.x &&
              event.mouse.x <= rect1.x + RECTANGLE_WIDTH &&
              event.mouse.y >= rect1.y &&
              event.mouse.y <= rect1.y + RECTANGLE_HEIGHT) {
            dragging = 1; // esta arrastando rect1.
          } else if (event.mouse.x >= rect2.x &&
                     event.mouse.x <= rect2.x + RECTANGLE_WIDTH &&
                     event.mouse.y >= rect2.y &&
                     event.mouse.y <= rect2.y + RECTANGLE_HEIGHT) {
            dragging = 2; // está arrastando rect2.
          }
        }
      } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
        // event.
        if (event.mouse.button == 1) { // clique esquerdo.
          dragging = 0;                // Parou de arrastar.
        }
      } else if (event.type ==
                 ALLEGRO_EVENT_MOUSE_AXES) { // Pedi ajuda pra um amigo, mouse
        // axes é uma biblioteca de mov de
        // mouse.
        if (dragging == 1) { // arrastando rect1.
          rect1.x = event.mouse.x - RECTANGLE_WIDTH / 2;
          rect1.y = event.mouse.y - RECTANGLE_HEIGHT / 2;

        } else if (dragging == 2) { // arrastando rect2.
          rect2.x = event.mouse.x - RECTANGLE_WIDTH / 2;
          rect2.y = event.mouse.y - RECTANGLE_HEIGHT / 2;
        }
      }

      // Checa a colisão entre os retângulos.
      if (Collide(&rect1, &rect2)) {
        rect1.color = al_map_rgb(0, 255, 0); // verde para o rect1.
        rect2.color = al_map_rgb(0, 255, 0); // verde para o rect2.

        if (!jogo_finalizado) {
          al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
                       tela_altura / 2, ALLEGRO_ALIGN_CENTER,
                       "VOCÊ COMPLETOU O PUZZLE!");
          al_flip_display();
          jogo_finalizado = true;
          dragging = 0;
          pontos++;

          al_rest(2.0);
          EstadoTela = jogando;
        }

        if (jogo_finalizado) {
            EstadoTela = jogando;
        }

      } else if (Collide(&rect1, &rect2, obstacles, numObstacles)) {
        al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2,
                     tela_altura / 2, ALLEGRO_ALIGN_CENTER, "VOCÊ FALHOU...");
        al_flip_display();
        al_rest(2.0);
        ResetRectangles(&rect1, &rect2);
        EstadoTela = jogando;
        vidas = vidas - 1;
      }

      al_clear_to_color(al_map_rgb(0, 0, 0));     // Limpa o display.
      DrawRectangle(&rect1);                      // desenha o rect1.
      DrawRectangle(&rect2);                      // desenha o rect2.
      DesenhaObstaculos(obstacles, numObstacles); // Desenha os obstaculos
      al_flip_display();                          // Flippando o display.
    }

    else if (EstadoTela == puzzle2) {

      // desenhando a caixa de dialogo
      al_clear_to_color(al_map_rgb(247, 255, 239));
      al_draw_rectangle(450, 50, 750, 200, al_map_rgb(0, 0, 0), 5);
      al_draw_filled_rectangle(445, 70, 455, 100, al_map_rgb(247, 255, 239));
      al_draw_line(440, 70, 460, 70, al_map_rgb(0, 0, 0), 5);
      al_draw_line(435, 100, 465, 100, al_map_rgb(0, 0, 0), 5);
      al_draw_triangle(50, 10, 10, 70, 70, 70, al_map_rgb(0, 0, 0), 5);
      al_draw_textf(fonte, al_map_rgb(0, 0, 0), 300, 50, NULL, "U = %d", randU);
      al_draw_textf(fonte, al_map_rgb(0, 0, 0), 500, 10, NULL, "I = %d", randI);
      al_draw_textf(fonte, al_map_rgb(0, 0, 0), 770, 50, NULL, "R = %d", btnCerto);

      switch(btnCerto){
          case 1:
              al_draw_textf(fonte, al_map_rgb(0, 0, 0), 350, 220, NULL, "1: %d", R);
              al_draw_textf(fonte, al_map_rgb(0, 0, 0), 490, 220, NULL, "2: %d", R + 5);
              al_draw_textf(fonte, al_map_rgb(0, 0, 0), 630, 220, NULL, "3: %d", R * 2);
             

              break;
          case 2:
              al_draw_textf(fonte, al_map_rgb(0, 0, 0), 350, 220, NULL, "1: %d", R + 5);
              al_draw_textf(fonte, al_map_rgb(0, 0, 0), 490, 220, NULL, "2: %d", R);
              al_draw_textf(fonte, al_map_rgb(0, 0, 0), 630, 220, NULL, "3: %d", R * 2);
			  
              break;
		  case 3:
  			  al_draw_textf(fonte, al_map_rgb(0, 0, 0), 350, 220, NULL, "1: %d", R * 2);
			  al_draw_textf(fonte, al_map_rgb(0, 0, 0), 490, 220, NULL, "2: %d", R + 5);
			  al_draw_textf(fonte, al_map_rgb(0, 0, 0), 630, 220, NULL, "3: %d", R);
			  
			  break;
      }
    

      if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
          switch (event.keyboard.keycode) {
          case ALLEGRO_KEY_1:
              if (btnCerto == 1) {
                  al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                      tela_altura / 2, ALLEGRO_ALIGN_CENTER,
                      "VOCÊ COMPLETOU O PUZZLE!");
                  al_flip_display();
                  jogo_finalizado2 = true;
                  dragging = 0;
                  pontos++;
                  al_rest(1.0);
                  EstadoTela = jogando;
              }
              else if (btnCerto != 1) {
                  al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                      tela_altura / 2, ALLEGRO_ALIGN_CENTER,
                      "VOCÊ ERROU!");
                  vidas = vidas - 1;
              }
              break;

          case ALLEGRO_KEY_2:
              if (btnCerto == 2) {
                  al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                  tela_altura / 2, ALLEGRO_ALIGN_CENTER,
                  "VOCÊ COMPLETOU O PUZZLE!");
              al_flip_display();
              jogo_finalizado2 = true;
              dragging = 0;
              pontos++;
              al_rest(1.0);
              EstadoTela = jogando;
              }
              else if (btnCerto != 2) {
                  al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                      tela_altura / 2, ALLEGRO_ALIGN_CENTER,
                      "VOCÊ ERROU!");
                  vidas = vidas - 1;
              }
              break;

          case ALLEGRO_KEY_3:
              if (btnCerto == 3) {
                  al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                      tela_altura / 2, ALLEGRO_ALIGN_CENTER,
                      "VOCÊ COMPLETOU O PUZZLE!");
                  al_flip_display();
                  jogo_finalizado2 = true;
                  dragging = 0;
                  pontos++;
                  al_rest(1.0);
                  EstadoTela = jogando;
              }
              else if (btnCerto != 3) {
                  al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                      tela_altura / 2, ALLEGRO_ALIGN_CENTER,
                      "VOCÊ ERROU!");
                  vidas = vidas - 1;
              }
              break;
          }
      }

          if (jogo_finalizado2) {
              
              EstadoTela = jogando;

          }
            
          if(pontos == 2)
          {
              al_flip_display();
              exibir_parabens = true;
              al_rest(3.0);
              
          } else if (vidas == 1) {
              al_flip_display();
              exibir_gameover = true;
              al_rest(3.0);
          }



          if (exibir_parabens) {
              al_clear_to_color(al_map_rgb(247, 255, 239)); // Limpa a tela com a cor de fundo
              al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                  tela_altura / 2, ALLEGRO_ALIGN_CENTER,
                  "Obrigado por jogar!");
              al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                  tela_altura / 3, ALLEGRO_ALIGN_CENTER,
                  "PARABÉNS! VOCÊ FINALIZOU O JOGO!");
              al_flip_display();

              al_rest(3.0); // Aguarde um pouco antes de retornar ao jogo

              // Volte ao estado inicial ou carregue a próxima fase do jogo
              // Altere o estado do jogo conforme necessário aqui

              sair = true; // Redefina exibir_parabens para evitar a exibição contínua
          }

          if (exibir_gameover) {
              al_clear_to_color(al_map_rgb(247, 255, 239)); // Limpa a tela com a cor de fundo
              al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                  tela_altura / 2, ALLEGRO_ALIGN_CENTER,
                  "Talvez você deva tentar novamente...!");
              al_draw_text(fonte, al_map_rgb(0, 0, 0), tela_largura / 2,
                  tela_altura / 3, ALLEGRO_ALIGN_CENTER,
                  "VOCê PERDEU!");
              al_flip_display();

              al_rest(3.0); // Aguarde um pouco antes de retornar ao jogo

              // Volte ao estado inicial ou carregue a próxima fase do jogo
              // Altere o estado do jogo conforme necessário aqui

              sair = true; // Redefina exibir_parabens para evitar a exibição contínua
          }
    }

    al_flip_display();
  }

  return 0;
}