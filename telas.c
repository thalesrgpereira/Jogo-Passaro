#include "telas.h"
void atualizaPontuacao(BITMAP *buffer, FONT *font, int placar){
	char str[12];
	sprintf(str, "%d", placar);
	textout_centre_ex(buffer, font, str ,SCREEN_W / 2, 20, makecol(255, 225, 255), -1);
}

void atualizaNomeJogador(char *jogador, int *indice){
	int ch;    
	if(keypressed()){
		ch = readkey();

		if((ch & 0xff) >= 33 && (ch & 0xff) <= 126 && (ch & 0xff) != 59 && *indice < 20) {    
			jogador[*indice] = ch;
			jogador[++*indice] = '\0';
		}
		else if((ch & 0xff) == 8 && *indice > 0) {
			jogador[--*indice] = '\0';
		}       
	}
}

void menu(BITMAP *buffer,FONT *font, char *jogador,int *selMenu){ 
	if(strcmp(jogador,"") != 0){
		if(*selMenu == 0){
			textout_centre_ex(buffer,font,"[INICICAR]" , SCREEN_W/2, SCREEN_H/2, makecol(255, 225, 255), -1);
		}
		else {
			textout_centre_ex(buffer,font,"INICICAR" , SCREEN_W/2, SCREEN_H/2, makecol(255, 225, 255), -1);
		}
	}
	if(*selMenu == 1){
		if(strcmp(jogador,"") == 0){                     
			textout_centre_ex(buffer,font,"[NOVO JOGADOR]" , SCREEN_W/2 , SCREEN_H/2 + 40, makecol(255, 225, 255), -1);
		}
		else {
			textout_centre_ex(buffer,font,"[TROCAR JOGADOR]" , SCREEN_W/2 , SCREEN_H/2 + 40, makecol(255, 225, 255), -1);
		}
	}
	else {
		if(strcmp(jogador,"") == 0){
			textout_centre_ex(buffer,font,"NOVO JOGADOR" , SCREEN_W/2 , SCREEN_H/2 + 40, makecol(255, 225, 255), -1);
		}
		else {
			textout_centre_ex(buffer,font,"TROCAR JOGADOR" , SCREEN_W/2 , SCREEN_H/2 + 40, makecol(255, 225, 255), -1);
		}
	}
	if(*selMenu == 2){
		textout_centre_ex(buffer,font,"[CLASSIFICACAO]" , SCREEN_W/2 , SCREEN_H/2 + 80, makecol(255, 225, 255), -1);
	}
	else {
		textout_centre_ex(buffer,font,"CLASSIFICACAO" , SCREEN_W/2 , SCREEN_H/2 + 80, makecol(255, 225, 255), -1);
	}
	if(*selMenu == 3){
		textout_centre_ex(buffer,font,"[CREDITOS]" , SCREEN_W/2 , SCREEN_H/2 + 120, makecol(255, 225, 255), -1);
	}
	else {
		textout_centre_ex(buffer,font,"CREDITOS" , SCREEN_W/2 , SCREEN_H/2 + 120, makecol(255, 225, 255), -1);
	}
}

void desenhaPlacar(BITMAP *buffer,BITMAP *pontuacao, FONT *font, char *jogador,int *selMenu, int jogadorPosicao, int jogadorPontuacao, int recorde){
	if(strcmp(jogador,"") != 0){
		char str[20];
		draw_sprite(buffer,pontuacao,10,10);
		textout_ex(buffer,font,jogador, 60,30, makecol(0, 0, 0), -1);
		sprintf(str, "%dº - %d", jogadorPosicao,jogadorPontuacao);
		textout_ex(buffer,font,str , 60,70, makecol(0, 0, 0), -1);
		sprintf(str, "%d", recorde);
		textout_ex(buffer,font,str, 60,110, makecol(0, 0, 0), -1);
	}
	else {
		if(*selMenu == 0) *selMenu = 1;
	}
}

//desenha o chao em movimento
void desenhaChao(BITMAP *buffer,BITMAP *chao, int *x, int y, int colidiu){ 
	int x2;
	if(colidiu != 1){
		//movimenta o chao
		*x-=6;
		if(*x < -653){
			*x = 653;
			x2 = 0;  
		}      
	}
	if(*x < 0){
		x2 = *x + 653; 
	}
	else {
		x2 = *x - 653;
	}
	draw_sprite(buffer,chao,*x,y);
	draw_sprite(buffer,chao,x2,y);
}

void animacaoExplosao(BITMAP *buffer, BITMAP *explosao1,BITMAP *explosao2,BITMAP *explosao3,BITMAP *explosao4, BITMAP *explosao5,BITMAP *explosao6,BITMAP *explosao7, int *inicioExplosao, int timer, passarinho passaro){
	int vtempo = timer - *inicioExplosao;
	if(vtempo < 50)
		draw_sprite(buffer,explosao1,passaro.x+14,passaro.y+10);//14
	else if (vtempo >= 50 && vtempo < 100)
		draw_sprite(buffer,explosao2,passaro.x+4,passaro.y); //33   
	else if (vtempo >= 100 && vtempo < 150)
		draw_sprite(buffer,explosao3,passaro.x,passaro.y-3); //41
	else if (vtempo >= 150 && vtempo < 200)
		draw_sprite(buffer,explosao4,passaro.x,passaro.y-3);//41
	else if (vtempo >= 200 && vtempo < 250)
		draw_sprite(buffer,explosao5,passaro.x-16,passaro.y-20);//74
	else if (vtempo >= 250 && vtempo < 300)
		draw_sprite(buffer,explosao6,passaro.x-9,passaro.y-13);//60
	else if (vtempo >= 300 && vtempo < 350)
		draw_sprite(buffer,explosao7,passaro.x-9,passaro.y-13);//60
	else {
		*inicioExplosao = -1;
		draw_sprite(buffer,explosao7,passaro.x-9,passaro.y-13);//60
	} 
}

void passaroSubireDescer(passarinho *passaro, int *subir){
	if (*subir != 0){ 
		passaro -> y -= 2;
	}
	else {
		passaro -> y += 2;
	}

	if( passaro -> y > 200){
		*subir = 1;
	}
	else {
		if(passaro -> y < 100) *subir = 0;
	}
}
