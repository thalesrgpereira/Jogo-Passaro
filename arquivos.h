#ifndef _ARQUIVOS
   #define _ARQUIVOS_ 1
#include <stdio.h>
#include <string.h>

typedef struct{
	char nome[20];
	int pontuacao;
	int posicao;
} rank;

void gravaRank(char jogador[20],int novoRecorde, int *posicao, int *melhorPontuacao);
void getRank(int inicio,int qRegistros, rank *registros);
void getDadosJogador(char jogador[20], int *posicao, int *melhorPontuacao,int *recorde);
void getUltimoUsuario(char *nome, int *posicao, int *melhorPontuacao,int *recorde);
void gravaUltimoUsuario(char nome[20], int posicao, int melhorPontuacao,int recorde);

#endif
