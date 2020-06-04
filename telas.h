#ifndef _TELAS
   #define _TELAS_ 1
#include <stdio.h>
#include <string.h>
#include <allegro.h>

#define ATRASO 35
#define SALTO_PASSARO 15
#define GRAVIDADE 3
#define ACELERA_GRAVIDADE 8
#define Y_CHAO 410

typedef struct {
	int x;
	int y;
	int comprimento;
	int altura;
	int visivel;
	int tempo;
	int tempoAsa;
	BITMAP *bmp; 
} passarinho;

void atualizaPontuacao(BITMAP *buffer,FONT *font, int placar);
void menu(BITMAP *buffer,FONT *font, char *jogador,int *selMenu);
void desenhaPlacar(BITMAP *buffer,BITMAP *pontuacao, FONT *font, char *jogador,int *selMenu, int jogadorPosicao, int jogadorPontuacao, int recorde);
void animacaoExplosao(BITMAP *buffer, BITMAP *explosao1,BITMAP *explosao2,BITMAP *explosao3,BITMAP *explosao4, BITMAP *explosao5,BITMAP *explosao6,BITMAP *explosao7,int *inicioExplosao, int timer, passarinho passaro);
void desenhaChao(BITMAP *buffer,BITMAP *chao, int *x, int y, int colidiu);
void passaroSubireDescer(passarinho *passaro, int *subir);
void atualizaNomeJogador(char *jogador, int *indice); 
#endif
