#ifndef _OBSTACULOS
	#define _OBSTACULOS_ 1
#include <allegro.h>
#define VELOCIDADE_OBSTACULOS 10



typedef struct { 
	int x; 
	int y;
	int w;
	int h;
	float angulo;
} obstaculo;

void iniciaObstaculos(obstaculo *ob, int espaco); 
void posicionaObstaculosTempo(BITMAP *buffer, BITMAP *cano, obstaculo *ob, int *obOrdem, int *obTempoInicio, int time, int placar);
void movimentaObstaculos(obstaculo *ob);
void incrementaPontuacao(int xPassaro, obstaculo *ob, int *placar);
int  colidir(int Ax,int Ay,int Aw,int Ah, obstaculo *ob, int *placar);
#endif
