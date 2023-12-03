#include <stdio.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <cmath>
#include <string>
#include <errno.h>

const int tela_altura = 800;
const int tela_largura = 600;
const int player_speed = 5;
int EstadoTela;
int Menu;


#define MAX_LINHAS 100
#define TAMANHO_MAXIMO 100



enum estado {
	telainicio,
    jogando,
    dialogo,
    puzzle
};

enum Menu {
	principal,
	controles,
	creditos
};



typedef struct {
    int x_player;
    int y_player;
    int largura_player;
    int altura_player;

}player;

typedef struct {
    int x_npc;
    int y_npc;
    int largura_npc;
    int altura_npc;

}npc;

void init() {
    al_init();
    al_init_ttf_addon();
    al_install_keyboard();
   // al_install_mouse();
    al_install_keyboard();
    al_install_mouse();
    al_init_font_addon();

}

void LimpaTela(){
	al_clear_to_color(al_map_rgb(0,0,0));

}
void GameMenu(){
	LimpaTela();
	ALLEGRO_FONT* fonte = al_create_builtin_font();
	al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2, tela_altura / 3, ALLEGRO_ALIGN_CENTER, "1.Jogar");
	al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2, tela_altura / 2, ALLEGRO_ALIGN_CENTER, "2.Controles");
	al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2, tela_altura / 1.5, ALLEGRO_ALIGN_CENTER, "3.Creditos <3");
	al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2, tela_altura / 1.2, ALLEGRO_ALIGN_CENTER, "ESC.Sair");

	Menu = principal;



}

void Controles(){
	LimpaTela();
	ALLEGRO_FONT* fonte = al_create_builtin_font();
	al_draw_text(fonte, al_map_rgb(255, 255, 255), tela_largura / 2, tela_altura / 2, ALLEGRO_ALIGN_CENTER, "lorem ipsum, LOREM IPSUM KKKKKKKKKKHAHAHAHAAH");
	Menu = controles;

}

void Creditos(){
	LimpaTela();
	ALLEGRO_FONT* fonte = al_create_builtin_font();
	al_draw_text(fonte,al_map_rgb(255,255,255),tela_largura/2,tela_altura/3,ALLEGRO_ALIGN_CENTER,"LOREM FODENDO ISPUM HAAAAAAAAA");
	Menu = creditos;
}

int main() {
    init();
    ALLEGRO_DISPLAY* display = al_create_display(tela_altura, tela_largura);;
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
	GameMenu();
	EstadoTela = telainicio; // Definindo o estado inicial como menu
	bool sair = false;
	bool jogando = false;
	

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

    if (!display) {
        fprintf(stderr, "Falha ao criar a janela.\n");
        return -1;
    }

    display = al_create_display(tela_altura, tela_altura);
    if (!display) {
        fprintf(stderr, "Falha ao criar o display.\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "Falha ao criar a fila de eventos.\\n");
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
   // al_register_event_source(event_queue, al_get_mouse_event_source());

    player player = { 0,0, 20,20 };
    npc square = { 200,200, 20,20 };

	FILE* thyagoQuintas;
	char linhas[MAX_LINHAS][TAMANHO_MAXIMO];

	errno_t aberto = fopen_s(&thyagoQuintas, "textThyago.txt", "r");

	//conferindo se houve algum erro 
	if (aberto != 0) {
		printf("Erro ao abrir o arquivo\n");
		return 1;
	}

	int contador_linhas = 0;
	while (fgets(linhas[contador_linhas], TAMANHO_MAXIMO, thyagoQuintas) != NULL && contador_linhas < MAX_LINHAS) {
		contador_linhas++;
	}
	contador_linhas = 0;

    al_draw_filled_rectangle(square.x_npc, square.y_npc, square.x_npc + square.largura_npc, square.y_npc + square.altura_npc, al_map_rgb(255, 0, 0)); //npc
    al_draw_filled_rectangle(player.x_player, player.y_player, player.x_player + player.largura_player, player.y_player + player.altura_player, al_map_rgb(0, 0, 255)); //player
    al_flip_display();
    EstadoTela = telainicio;
    bool sair = false;

	while (!sair) {
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		//Navegacao Game Menu;
		if (event.type == ALLEGRO_EVENT_KEY_DOWN) {

			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE && Menu == principal) {
				sair = true;
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_2) {
				Controles();
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && Menu == controles) {
				GameMenu();
			}

			if (event.keyboard.keycode == ALLEGRO_KEY_3 && EstadoTela == principal) { // va para a tela de creditos
				Creditos();
				

			}

			if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE && EstadoTela == creditos) { // volte a tela de inicio
				GameMenu();

			}
		}

		// Movimentacao
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			sair = true;
		}

		else if (event.type == ALLEGRO_EVENT_KEY_CHAR && EstadoTela == jogando) {
			contador_linhas = 0;
			//EstadoTela = jogando;
			switch (event.keyboard.keycode) {
				// Movimentacao do jogador
			case ALLEGRO_KEY_UP:
				player.y_player -= player_speed;
				break;
			case ALLEGRO_KEY_DOWN:
				player.y_player += player_speed;
				break;
			case ALLEGRO_KEY_LEFT:
				player.x_player -= player_speed;
				break;
			case ALLEGRO_KEY_RIGHT:
				player.x_player += player_speed;
				break;

			}
		}

		bool colisao = (player.x_player < square.x_npc + square.largura_npc &&
			player.x_player + player.largura_player > square.x_npc &&
			player.y_player < square.y_npc + square.altura_npc &&
			player.y_player + player.altura_player > square.y_npc);

		// Limpa a tela
		LimpaTela();

		// Desenha o NPC com a cor vermelha
		al_draw_filled_rectangle(square.x_npc, square.y_npc, square.x_npc + square.largura_npc, square.y_npc + square.altura_npc, al_map_rgb(255, 0, 0));

		// Se houver colisão, desenha o NPC com a cor azul
		if (colisao) {
			al_draw_filled_rectangle(square.x_npc, square.y_npc, square.x_npc + square.largura_npc, square.y_npc + square.altura_npc, al_map_rgb(0, 0, 255));
            if(event.keyboard.keycode == ALLEGRO_KEY_E && EstadoTela == jogando){
				//desenhando a caixa de dialogo
				al_draw_filled_rectangle(0, tela_altura / 1.3, tela_largura, tela_altura, al_map_rgb(0, 0, 200));
				al_draw_filled_rectangle(3, tela_altura / 1.3 + 3, tela_largura - 3, tela_altura - 3, al_map_rgb(0, 0, 0));
				al_draw_text(font, al_map_rgb(255, 255, 255), 13, tela_altura / 1.3 + 13, ALLEGRO_ALIGN_LEFT, linhas[contador_linhas]);
				EstadoTela = dialogo;
				contador_linhas++;
            }
			else if(linhas[contador_linhas] != NULL && EstadoTela == dialogo){
					if (event.keyboard.keycode == ALLEGRO_KEY_E) {
						al_draw_filled_rectangle(0, tela_altura / 1.3, tela_largura, tela_altura, al_map_rgb(0, 0, 200));
						al_draw_filled_rectangle(3, tela_altura / 1.3 + 3, tela_largura - 3, tela_altura - 3, al_map_rgb(0, 0, 0));
						al_draw_text(font, al_map_rgb(255, 255, 255), 13, tela_altura / 1.3 + 13, ALLEGRO_ALIGN_LEFT, linhas[contador_linhas]);
						contador_linhas++;
					}
					EstadoTela = jogando;
					contador_linhas = 0;
			}
						
		}
		
		// Desenha o jogador
		al_draw_filled_rectangle(player.x_player, player.y_player, player.x_player + player.largura_player, player.y_player + player.altura_player, al_map_rgb(0, 0, 255));

		// Atualiza a tela
		al_flip_display();
	}

	fclose(thyagoQuintas);
    return 0;
}