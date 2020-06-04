#include <stdio.h>
#include <string.h>
#include <allegro.h>
#include "obstaculos.h"
#include "arquivos.h"
#include "telas.h"

//imagens
BITMAP *buffer;
BITMAP *chao;
BITMAP *campo;
BITMAP *cano;
BITMAP *p1;
BITMAP *p2;
BITMAP *titulo;
BITMAP *pontuacao;
BITMAP *explosao1;
BITMAP *explosao2;
BITMAP *explosao3;
BITMAP *explosao4;
BITMAP *explosao5;
BITMAP *explosao6;
BITMAP *explosao7;
//fontes
FONT *fontArialBold20;
FONT *fontstencil28;
FONT *fontYuGothicLight12;
//sons
SAMPLE  *piu;
SAMPLE  *explosao;

int sair = 0;
volatile int timer;

void sair_allegro();
void init();
void deinit();
void incrementa_timer();

int main() {
	//permite adicionar acentos aos textos
	set_uformat(U_ASCII);
	init();
	
	//cria e inicializa as variaveis do passaro
    	passarinho passaro;
	passaro.x = 250;
	passaro.y = 100;
	passaro.comprimento = 42;
	passaro.altura = 34;
	passaro.tempoAsa = 10;
	passaro.visivel = 1;

	//controla a movimentacao do chao
	int x_chao = 0;
	//controla o a batidae de asa do passaro
	int t = 0;
	//controla o subir e descer do passaro
	int subir = 0;

	//variavel para marcar o colisao
	int colidiu = 0;

	//variavel que controla em que tela o jogos esta
	int status = 0;
	//variavel que controla qual opcao do menu foi selecionada.
	int selMenu = 0;

	//variavei do recorde e do placar
	int recorde = 0;
	int placar = 0;

	//variaveis do jogados
	char jogador[20] = "";
	int jogadorPosicao = 0;
	int jogadorPontuacao = 0;
	getUltimoUsuario(jogador,&jogadorPosicao,&jogadorPontuacao,&recorde);
	//variavel utilizada da digitado do nome do jogador
	int indice = 0; 
	indice = strlen(jogador);

	//variaveis de controle do rank
	int rankInicio = 1;
	int rankAlterado = 1; 
	rank rankRegistros[10];

	//marca o tempo em que o passado comecou a piar
	int piando = 0; 
	//marca o tempo em que a explosao comicou
	int inicioExplosao = 0;

	//variaveis dos obstaculos
	int tpObstaculo;
	int obDistacia = 220;
	int obOrdem = 0;
	int obTempoInicio = -1;
	obstaculo ob[10];
	iniciaObstaculos(ob,obDistacia);

	//roda o aplicativo enquento nao for clicado no botao sair
	while (!sair) { 

		//se clicar o botao ESC fechar o programa tambem  
		if (key[KEY_ESC])  {
			sair = 1;  
			allegro_exit();
		}
		//cria o fundo azul da tela
		rectfill(buffer,0,0,640,480,makecol(26, 185, 198));

		//alterna as imagens dos passaros a cada TEMPO_ASA.
		++t;
		if(t > passaro.tempoAsa) {
			passaro.bmp = p1;
			if(t == 2 * passaro.tempoAsa) t = 0;
		}
		else {
			passaro.bmp = p2;
		}

		switch(status){
			case 0:{//TELA DO MENU
				
				//reinicia pontuacao.
				placar = 0;
				//remove a colisao
				colidiu = 0;
				//desenha o placar
				desenhaPlacar(buffer,pontuacao,font,jogador,&selMenu,jogadorPosicao,jogadorPontuacao,recorde);
				//desenha o menu
				menu(buffer,fontYuGothicLight12,jogador,&selMenu);
				//coloca o foco nas opicoes do menu
				if (key[KEY_UP]) {
					while(key[KEY_UP]);
					if(strcmp(jogador,"") == 0){
						if (selMenu > 1) selMenu--;
					}
					else {
						if (selMenu > 0) selMenu--;
					}
				}
				if (key[KEY_DOWN]) {
					while(key[KEY_DOWN]);
					if (selMenu < 3) selMenu++;
				}   

				//desenha o titulo
				draw_sprite(buffer,titulo,220,50);

				//reposiciona o passaro
				passaro.x = (SCREEN_W/2) - (passaro.comprimento/2); 

				//faz o passaro subir e descer
				passaroSubireDescer(&passaro, &subir);

				//controla a mudanca de tela ao clicar em space
				if (key[KEY_SPACE]) {
					while(key[KEY_SPACE]);
					t = 0; 
					passaro.tempoAsa = 10;
					switch(selMenu){
						case 0:
							passaro.x = 200;
							passaro.tempoAsa = 1;
							status = 2;
							break;
						case 1:
							status = 1;
							break;
						case 2:
							rankInicio = 1;
							status = 4;
							break;
						case 3:
							status = 5;
							break;
					} 

				}  
				break;
			}
			case 1:{//CADASTRO DO JOGADOR
				
				atualizaNomeJogador(jogador,&indice);

				//desenha onde sera escrito o nome do jogador
				textout_ex(buffer, font, "Digite seu nome:",(SCREEN_W / 2) - 174, 310, makecol(255, 225, 255), -1);
				rectfill(buffer,(SCREEN_W/2) - 175,325,(SCREEN_W/2) + 175, 370,makecol(0,96,130));

				//esceve o nome do jogador
				textout_centre_ex(buffer, fontArialBold20, jogador ,SCREEN_W / 2, 330, makecol(255, 225, 255), -1);

				//desenha titulo
				draw_sprite(buffer,titulo,220,50);

				//obtem os dados do jogador e retorna a tela inicial
				if (key[KEY_SPACE]) {
					while(key[KEY_SPACE]);
					if(strcmp(jogador,"") != 0){
						getDadosJogador(jogador, &jogadorPosicao, &jogadorPontuacao, &recorde);
						selMenu = 0;
						t = 0; 
						passaro.tempoAsa = 10;
						status = 0;
					}
				}  
				break;
			}
			case 2:{ //TELA DE COMECAR 
				
				//escreve comecar na tela
				textout_centre_ex(buffer, fontArialBold20, "COMECAR!!" ,SCREEN_W / 2, 20, makecol(255, 225, 255), -1);

				//reinicia placar.
				placar = 0;
				//remove a colisao
				colidiu = 0;
				//reposiciona o passaro
				passaro.x = 100; 

				if (key[KEY_SPACE]) {
					while(key[KEY_SPACE]);
					t = 0;
					passaro.tempoAsa = 1;
					status = 3;
				}     
				break;
			}
			case 3:{//JOGO INICIADO
				
				posicionaObstaculosTempo(buffer, cano, ob, &obOrdem, &obTempoInicio, timer, placar);

				if(!colidiu){
					if(passaro.y > (Y_CHAO - passaro.altura)) {
						colidiu  = 1;
					}
					else {
						colidiu  = colidir(passaro.x,passaro.y,passaro.comprimento,passaro.altura, ob, &placar);
					}
				}
				atualizaPontuacao(buffer,fontArialBold20,placar);

				if(colidiu){ 
					//marca o inicio da explosao no tempo
					if(inicioExplosao == 0){
						passaro.visivel = 0;
						inicioExplosao = timer;
						play_sample(explosao,255,178,1000,0);
					}
					else { 
						//se a animacao da explosao ja terminou 
						if(inicioExplosao == -1) {                   
							//atualiza o rank
							if(strcmp(jogador,"") != 0){ 
								if(placar > recorde) recorde = placar; 
								if(placar > jogadorPontuacao){
									gravaRank(jogador,placar, &jogadorPosicao, &jogadorPontuacao);
									rankAlterado = 1;
								}
								gravaUltimoUsuario(jogador,jogadorPosicao,jogadorPontuacao,recorde);
							}
							
							//exibe gameover
							textout_centre_ex(buffer, fontstencil28, "  GAME OVER  ",SCREEN_W / 2, (SCREEN_H / 2) - 50, makecol(255, 0, 0), makecol(0, 0, 0));

							//permite sair para a tela inicial
							if (key[KEY_SPACE]){
								//espera a tecla ser desprecionada                 
								while(key[KEY_SPACE]);
								t = 0; 
								passaro.tempoAsa = 10;
								passaro.y = 100;
								passaro.visivel = 1;
								inicioExplosao = 0;
								iniciaObstaculos(ob,220);
								status = 0;
							}
						}
						else {
							//faz a animacao da explosao
							animacaoExplosao(buffer,explosao1,explosao2,explosao3,explosao4,explosao5,explosao6,explosao7,&inicioExplosao,timer,passaro);

						}
					}
				}
				else {   
					movimentaObstaculos(ob);

					passaro.y += GRAVIDADE;
					
                    			incrementaPontuacao(passaro.x,ob,&placar);

					if(key[KEY_SPACE]){
						//faz o passaro saltar
                        			passaro.y -= SALTO_PASSARO;
						//impede que o passaro passe da tela
                        			if(passaro.y < 0) passaro.y = 0;
						//zera a contagem para acelerar a gravidade
                        			passaro.tempo = 0;
						
                        			//controla o piado do passaro
                        			if(piando == 0){
							piando = timer;
							play_sample(piu,255,178,1000,0);       
						}
						else {
							if(timer - piando >= 200){
								piando = 0; 
							}
						}                       
					}
					else {
                        			//se o tempo chegar a 5 incrementa a aceleracao a gravidade
                        			if(passaro.tempo > 5) {   
							passaro.y += ACELERA_GRAVIDADE;
						}
						else {
							passaro.tempo++;  
						} 
					}	

				}
				break;
			}
			case 4:{//TELA RANK
				
				int qRegistros = 10;
				int i,j;
				int y1 = 75,y2 = 100;
				int h = 25;
				char str[10];
				//obtem os novo rank se ele foi altarado ou se a tabela foi movida
				if(rankAlterado = 1 || rankInicio != rankRegistros[0].posicao){
					getRank(rankInicio,qRegistros,rankRegistros);
				}

				//reposiciona o passaro
				passaro.x = 10;
				passaro.y = 10;

				//desenha a tabela e os dados obtido do arquivo de rank
				rectfill(buffer,100,y1,540,y2,makecol(79,79,79));
				textout_right_ex(buffer,font,"Rank", 140,y1+10, makecol(255, 255, 255), -1);
				textout_ex(buffer,font,"Nome", 180,y1+10, makecol(255, 255, 255), -1);
				textout_right_ex(buffer,font,"Pontos", 530,y1+10, makecol(255, 255, 255), -1);
				y1+=h;
				y2+=h;
				for(i = 0, j = rankInicio; i < qRegistros; i++, j++){
					if(j % 2 == 0) {  
						rectfill(buffer,100,y1,540,y2,makecol(255, 255, 255));
					}
					else{
						rectfill(buffer,100,y1,540,y2,makecol(220,220,220));
					}
					if (rankRegistros[i].posicao > 0) {
						sprintf(str, "%dº", rankRegistros[i].posicao);
						textout_right_ex(buffer,font,str, 140,y1+10, makecol(54,54,54), -1);
						textout_ex(buffer,font,rankRegistros[i].nome, 180,y1+10, makecol(54,54,54), -1);
						sprintf(str, "%d", rankRegistros[i].pontuacao);
						textout_right_ex(buffer,font,str, 530,y1+10, makecol(54,54,54), -1);
					}
					y1+=h;
					y2+=h;
				}
				if (key[KEY_SPACE]) {
					while(key[KEY_SPACE]);
					selMenu = 0;
					t = 0; 
					passaro.tempoAsa = 10;
					status = 0;
				}

				if (key[KEY_UP]) {
					while(key[KEY_UP]);
					if (rankInicio > 1) rankInicio--;
				}
				if (key[KEY_DOWN]) {
					while(key[KEY_DOWN]);
					if (rankRegistros[9].posicao > 0) rankInicio++;
				}  

				break;
			}
			case 5:{//TELA DE CREDITOS
				
				//Desenha o titulo
				draw_sprite(buffer,titulo,220,50);
				
                		//desenha os nomes e matriculas
				textout_centre_ex(buffer,font,"1910026651 - JOSÉ PAULO DA SILVA JÚNIOR", SCREEN_W / 2, (SCREEN_H / 2)+20, makecol(255, 255, 255), -1);
				textout_centre_ex(buffer,font,"1910024533 - JOSÉ VINÍCIUS FIRMINO DANTAS LEITA", SCREEN_W / 2,(SCREEN_H / 2)+ 40, makecol(255, 255, 255), -1);
				textout_centre_ex(buffer,font,"1910026678 - MATHEUS FERNANDES DE ANDRADE OLIVEIRA", SCREEN_W / 2,(SCREEN_H / 2)+ 60, makecol(255, 255, 255), -1);  
				textout_centre_ex(buffer,font,"1920026785 - THALES RAMALHO GUEDES PEREIRA", SCREEN_W / 2,(SCREEN_H / 2)+ 80, makecol(255, 255, 255), -1); 

				//faz o passaro subir e descer
				passaroSubireDescer(&passaro, &subir);

				if (key[KEY_SPACE]) {
					while(key[KEY_SPACE]);
					t = 0;
					passaro.tempoAsa = 10;
					status = 0;
				}     
				break;
			}

		}
		desenhaChao(buffer,chao,&x_chao, Y_CHAO, colidiu);          

		if(passaro.visivel == 1){
			//desenha o passaro
			draw_sprite(buffer,passaro.bmp,passaro.x,passaro.y);
		}

		//faz um atraso no loop
		rest(ATRASO);

        	draw_sprite(screen,buffer,0,0);
		clear(buffer);
	}
	deinit();
	return 0;
}
END_OF_MAIN()

void sair_allegro(){
	sair = 1;
}
END_OF_FUNCTION(sair_allegro)

void incrementa_timer() { 
	timer++; 
}
END_OF_FUNCTION(incrementa_timer)

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	set_close_button_callback(sair_allegro);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) { 
		allegro_message(allegro_error);
		exit(-1);
	}
	set_window_title("drunk bird");

	install_timer();
	install_keyboard();
	install_mouse();
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT,NULL);

	/* add other initializations here */

	//cria a imagem onde vai ser redesenhado as outras imagens
	buffer = create_bitmap(640, 480);

	//carrega as imagens:
	chao = load_bitmap("imagens\\chao.bmp",NULL);
	campo = load_bitmap("imagens\\edit.bmp",NULL);
	cano = load_bitmap("imagens\\cano.bmp",NULL);	
	p1 = load_bitmap("imagens\\passaro1.bmp",NULL);
	p2 = load_bitmap("imagens\\passaro2.bmp",NULL);
	titulo = load_bitmap("imagens\\titulo.bmp",NULL);
	pontuacao = load_bitmap("imagens\\pontuacao.bmp",NULL);
	explosao1 = load_bitmap("imagens\\explosao1.bmp",NULL);
	explosao2 = load_bitmap("imagens\\explosao2.bmp",NULL);
	explosao3 = load_bitmap("imagens\\explosao3.bmp",NULL);
	explosao4 = load_bitmap("imagens\\explosao4.bmp",NULL);
	explosao5 = load_bitmap("imagens\\explosao5.bmp",NULL);
	explosao6 = load_bitmap("imagens\\explosao6.bmp",NULL);
	explosao7 = load_bitmap("imagens\\explosao7.bmp",NULL);
	//fontes
	fontArialBold20 = load_font("fontes\\ArialBold20.pcx",NULL,NULL);
	fontstencil28 = load_font("fontes\\stencil28.pcx",NULL,NULL);
	fontYuGothicLight12 = load_font("fontes\\YuGothicLight12.pcx",NULL,NULL);
	//sons
	piu = load_sample("sons\\piu.wav");
	explosao = load_sample("sons\\explosao.wav");

	//inicializa a variavel timer
	timer = 0;
	//faz o incremento da variavel global timer a cada milisegundos
	LOCK_FUNCTION(incrementa_timer);
	LOCK_VARIABLE(timer);
	install_int_ex(incrementa_timer, MSEC_TO_TIMER(1));
}

void deinit() {
	clear_keybuf();
	/* add other deinitializations here */
	destroy_bitmap(cano);
	destroy_bitmap(campo);
	destroy_bitmap(p1);
	destroy_bitmap(p2);
	destroy_bitmap(chao);
	destroy_bitmap(titulo);
	destroy_bitmap(buffer);
	destroy_bitmap(pontuacao);
	destroy_bitmap(explosao1);
	destroy_bitmap(explosao2);
	destroy_bitmap(explosao3);
	destroy_bitmap(explosao4);
	destroy_bitmap(explosao5);
	destroy_bitmap(explosao6);
	destroy_bitmap(explosao7);
	destroy_font(fontArialBold20);
	destroy_font(fontstencil28);
	destroy_font(fontYuGothicLight12);
	destroy_sample(piu);
	destroy_sample(explosao);
}
