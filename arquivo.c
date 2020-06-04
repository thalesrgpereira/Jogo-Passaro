#include "arquivos.h"
void getUltimoUsuario(char *nome, int *posicao, int *melhorPontuacao,int *recorde){
	*posicao = 0;
	*recorde = 0;
	strcpy(nome,"");
	*melhorPontuacao = 0;
	FILE *file = fopen ("ultimo.txt","r");
	if(file){
		fscanf(file,"%[^\t;]%*c%d%*c%d%*c%d",nome,posicao,melhorPontuacao,recorde);
		fclose (file);
	}
}
void gravaUltimoUsuario(char nome[20], int posicao, int melhorPontuacao,int recorde){
	FILE *file = fopen("ultimo.txt","w");
	if(file){
		fprintf(file,"%s;%d;%d;%d;",nome,posicao,melhorPontuacao,recorde);
		fclose (file);
	}
}

void getRank(int inicio,int qRegistros, rank *registros){
	int pontos, i;
	char nome[20];
	FILE *file;
	//zera a posicao dos registro que serao retornado
	//para ignorar o lixo.
	for(i = 0; i < qRegistros; i++) {
		registros[i].posicao = 0;
		registros[i].pontuacao = 0;
	}
	i = 1; 
	//retorna a quantidade de registros, iniciada na posicao informada.
	file = fopen("rank.txt","r");
	if(file){
		while (fscanf(file," %[^\t=]%*[\t=]%d",&nome,&pontos) > 0){
			if (i >= inicio) {
				registros[i - inicio].pontuacao = pontos;
				strcpy(registros[i - inicio].nome,nome);
				registros[i - inicio].posicao = i; 
			}
			i++;
			if (i > (inicio + qRegistros - 1)) break;
		}
		fclose (file);
	}
}

void getDadosJogador(char jogador[20], int *posicao, int *melhorPontuacao,int *recorde){
	int pontos, i = 1;
	char nome[20];
	*recorde = 0;
	*posicao =  0;
	*melhorPontuacao = 0;
	FILE *file = fopen ("rank.txt","r");
	if(file){
		while (fscanf(file," %[^\t=]%*[\t=]%d",&nome,&pontos) > 0){
			//pega os pontos do primeiro do rank
			if(*recorde == 0) *recorde = pontos;
			//pega od dados do jogoador
			if(strcmp(nome,jogador) == 0){
				*posicao =  i;
				*melhorPontuacao = pontos;
				break;
			}
			i++;
		}
		fclose(file);
	}
}

void gravaRank(char jogador[20],int novaPontuacao, int *posicao, int *melhorPontuacao){
	rank *c = (rank*) malloc(sizeof(rank));
	int i, j, pontos, linha = 0, encontrado = 0;
	char nome[20];
	//arquivo atual
	FILE *file = fopen("rank.txt","a+");
	//arquivo com nova classificacao
	FILE *file2 = fopen("rank.temp","w");
	//se conseguiu abrir ambos os arquivos
	if((file != NULL) && (file2 != NULL)){
		rewind(file);    
		//obtem os dados de rank.txt // feof(file)
		while (fscanf(file," %[^\t=]%*[\t=]%d",&nome,&pontos) > 0){
			//fprintf(file2,"%s=%d\n",nome,pontos);
			strcpy(c[linha].nome,nome);
			if(strcmp(nome,jogador) == 0){
				if(c[linha].pontuacao < novaPontuacao) {
					c[linha].pontuacao = novaPontuacao;
				}
				else {
					c[linha].pontuacao = pontos;
				}
				encontrado = 1;
			}
			else {
				c[linha].pontuacao = pontos;
			}
			linha++;
			c = (rank*) realloc(c,(linha+1) * sizeof(rank));
		}
		//inclui o usuario caso ele nao seja encontrado
		if (encontrado ==0) {
			strcpy(c[linha].nome,jogador);
			c[linha].pontuacao = novaPontuacao;
			++linha;
		}

		//faz as nova classificacao
		for (i = 0; i < linha; ++i) {
			for (j = i + 1; j < linha; ++j){
				if (c[i].pontuacao < c[j].pontuacao) {
					pontos =  c[i].pontuacao;
					strcpy(nome,c[i].nome);
					c[i].pontuacao = c[j].pontuacao;
					strcpy(c[i].nome,c[j].nome);
					c[j].pontuacao = pontos;
					strcpy(c[j].nome,nome);
				}
			}
		}  
		//grava a nova classificacao no novo arquivo
		for(i = 0; i < linha; i++){
			if(strcmp(c[i].nome,jogador) == 0){
				*posicao =  i + 1;
				*melhorPontuacao = c[i].pontuacao;
			}
			fprintf(file2,"%s=%d\n",c[i].nome,c[i].pontuacao);    
		}
		//libara a memoria
		free(c);
		//fecha os arquivos
		fclose (file);
		fclose (file2);
		//grava a nova classificacao em rank.txt  
		if(remove("rank.txt") == 0){
			rename("rank.temp","rank.txt");                  
		}
	}
	if(file != NULL) fclose (file);
	if(file2 != NULL) fclose (file2);
}
