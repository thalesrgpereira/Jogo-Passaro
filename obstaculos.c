#include "obstaculos.h"

void iniciaObstaculos(obstaculo *ob, int espaco){
	int i,pos = espaco + 80;
	for(i = 9; i >= 0; i-=2) {
		//obstaculo de cima
		ob[i].x = pos * -1;
		ob[i].angulo = 128;
		ob[i].w = 80;
		ob[i].h = 287;
		//obstaculo de de baixo
		ob[i-1].x = pos * -1;
		ob[i-1].w = 80;
		ob[i-1].h = 287;
		pos += espaco + 80;
	} 
}

void posicionaObstaculosTempo(BITMAP *buffer, BITMAP *cano, obstaculo *ob, int *obOrdem, int *obTempoInicio, int time, int placar){ 
	int i;
	int tpObstaculo;
	int obTempo = 1500;
	int incAbertura = 30;

	//faz o aumento da dificuldade
	if(placar > 10 && placar <=20){
		obTempo = 1200;
		incAbertura = 25;
	}
	else if(placar > 20 && placar <=30){      
		obTempo = 1100;
		incAbertura = 20;
	}
	else if(placar > 30 && placar <=40){      
		obTempo = 1000;
		incAbertura = 10;
	}
	else if(placar > 40 && placar <=50){      
		obTempo = 900;
		incAbertura = 5;
	}
	else if(placar > 50){
		obTempo = 900;
		incAbertura = 0;
	}

	if(ob[*obOrdem].x < (ob[*obOrdem].w * -1)){ 
		if(*obTempoInicio == 0){ 
			*obTempoInicio = time;  
		}
		else {
			if((*obTempoInicio == -1) || (time - *obTempoInicio) >= obTempo) {
				ob[*obOrdem].x = SCREEN_W + ob[*obOrdem].w;
				ob[*obOrdem+1].x = SCREEN_W + ob[*obOrdem].w;

				//existem tres obstaculos: passagem por baixo, pelo meio e por cima            
				//sorteia os obstaculos:
				tpObstaculo = rand() %3;  
                switch (tpObstaculo){
					case 0: //por baixo
						ob[*obOrdem].y = 350;
						ob[*obOrdem+1].y = -50 -incAbertura;
						break;
					case 1: //pelo meio
						ob[*obOrdem].y = 250 + incAbertura; 
						ob[*obOrdem+1].y = -140; 
						break;
					case 2: //por cima
						ob[*obOrdem].y = 140 + incAbertura; 
						ob[*obOrdem+1].y = -250;
						break;
				}
				*obOrdem += 2;
				if(*obOrdem > 8) *obOrdem = 0;
				*obTempoInicio = 0;
			}   
		}
	} 
	for(i = 0; i < 10; i+=2) {  
		rotate_sprite(buffer,cano,ob[i].x,ob[i].y,itofix(ob[i].angulo));
		rotate_sprite(buffer,cano,ob[i+1].x,ob[i+1].y,itofix(ob[i+1].angulo));   
	}  
}

void movimentaObstaculos(obstaculo *ob){
	int i;
	for(i = 0; i < 10; i++){ 
		if(ob[i].x > ((ob[i].w +1) * -1)) ob[i].x -= VELOCIDADE_OBSTACULOS;
	}
}

void incrementaPontuacao(int xPassaro, obstaculo *ob, int *placar){
	int i;
	for(i = 0; i < 10; i += 2) {
		if (xPassaro == (ob[i].x + (ob[i].w /2))) ++*placar;
	}
}

int colidir(int Ax,int Ay,int Aw,int Ah, obstaculo *ob, int *placar){
	int i;
	for(i = 0; i < 10; i++) {
		if(Ax + Aw > ob[i].x && Ax < ob[i].x + ob[i].w && Ay + Ah > ob[i].y && Ay < ob[i].y + ob[i].h) return 1; 
	}
	return 0;
}

