#include "player.h"
#include "objects.h"
#include "draw.h"
#include <iostream>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define WIDTH 1024
#define HEIGHT 768
#define MAP_BORDERX 8000
#define MAP_BORDERY 4000
#define FPS 30
#define MAX_COLLECTABLES 50

#define UP 1
#define DOWN -1
#define RIGHT 1
#define LEFT -1

#define SWORD_INCREMENT 4
#define SWORD_MAX_SIZE 500

enum GAME_STATE{MENU=-1,HIGHSCORE_MENU,HELP_MENU,CREDITS_MENU,DEAD,PAUSE,QUIT,GAME_0};
enum SWORD_MODE{SWORD_KEY=0,SWORD_MOUSE};
enum SFX{SLASH_SOUND=0,INITWAR_SOUND,PICKUP_SOUND,AURA_SOUND,CLICK_SOUND,WIN_SOUND};

using namespace std;

bool canPause=true;
// Booleans para colisão com as paredes
bool left_wall=false,right_wall=false,top_wall=false,bottom_wall=false;

double modulo(double x)
{ return x>=0 ? x:-x; }

void reset(Player *p1, Camera *cam, Collectable **objArray) // Reseta o jogo (camera, personagem, e monstros com novas posições)
{
	for(int x=0;x<MAX_COLLECTABLES;x++)
	{
		objArray[x]->rePosition();
	}

	*p1 = Player(WIDTH,HEIGHT);
	*cam = Camera(WIDTH,HEIGHT);
}

bool contraryDir(bool keyState[]) // Função que talvez auxilie em trazer o cara de volta para o centro da tela
{
	if(left_wall) return (!keyState['a'] && keyState['d']);
	if(right_wall) return (!keyState['d'] && keyState['a']);
	if(top_wall) return (!keyState['w'] && keyState['s']);
	if(bottom_wall) return (!keyState['s'] && keyState['w']);
}

void calculatePhysics(Player *p1, Camera *cam,bool keyState[],Collectable **objArray, int *gameState, Mix_Chunk **sfx)
{

	// Sair do jogo
	if(keyState[27])
		*gameState=QUIT;
	// Resetar jogo
	if(keyState['r'])
		*gameState=DEAD; // Por enquanto não tem volta
	if(keyState['+'])
		p1->sword->size++;
	if(keyState['n'])
		p1->sword->rotation++;
	if(keyState['m'])
		p1->sword->rotation--;

	// Calcular colisão
		bool colided = false;
		double dist1;
		
		for(int x=0;x<MAX_COLLECTABLES;x++)
		{
			// Colisão com o JOGADOR
			if(objArray[x]->isAlive && objArray[x]->x - objArray[x]->sizex/2 < p1->x+p1->sizex/2 
									&& objArray[x]->x + objArray[x]->sizex/2 > p1->x-p1->sizex/2 
									&& objArray[x]->y - objArray[x]->sizey/2 < p1->y+p1->sizey/2 
									&& objArray[x]->y + objArray[x]->sizey/2 > p1->y-p1->sizey/2)
			{
				colided=true;
				objArray[x]->isAlive=false; // por precaução, desativar temporariamente a colisão com o jogador
				objArray[x]->rePosition();	// reposicionar coletável

				if(!p1->fakePlayer)	// artifício para impedir que coletáveis sejam gerados embaixo do jogador no início da partida
				{
					if(objArray[x]->canKill && !p1->bless)	// caso o objeto possa me matar, que ele me mate (e ativar salvamento de pontuação)
					{
						*gameState=DEAD;
						p1->canSave=true;
						Mix_PauseMusic();
						Mix_PlayChannel(2,sfx[WIN_SOUND],0);
					}
					else
					{
						Mix_PlayChannel(1, sfx[PICKUP_SOUND], 0);

						// Caso o objeto seja to tipo pixie, cresce a espada
						if(dynamic_cast<Pixie*>(objArray[x]) && p1->sword->size <= (double)SWORD_MAX_SIZE)
							p1->sword->size+=(double)SWORD_INCREMENT;

						// Caso o objeto seja do tipo Kitsune, aumenta velocidade
						if(dynamic_cast<Kitsune*>(objArray[x]))
							p1->vmult=1.5;

						// Caso o objeto seja do tipo Miko, ativa aura de invencibilidade
						if(dynamic_cast<Miko*>(objArray[x]))
						{
							p1->bless=true;
							Mix_PlayChannel(-1,sfx[AURA_SOUND], 0);	// 1º -1/1,2,3...: numero do canal ou -1 pro primeiro livre
						}											// 3º 0/1: única execução/loop
					}
				}

				objArray[x]->isAlive=true;
			}

			// Colisão com a ESPADA

			for(int i=0; i<=p1->sword->size - p1->sword->fixed_width/2; i+=10){
				if(objArray[x]->canKill)
				{
					dist1 =sqrt(((rotationConvert((i),0,p1->sword->rotation,'x'))+p1->x - (objArray[x]->x)) * 
								((rotationConvert((i),0,p1->sword->rotation,'x'))+p1->x - (objArray[x]->x)) +
								((rotationConvert((i),0,p1->sword->rotation,'y'))+p1->y - (objArray[x]->y)) * 
								((rotationConvert((i),0,p1->sword->rotation,'y'))+p1->y - (objArray[x]->y)));



					if(dist1 <= p1->sword->fixed_width/2 + (objArray[x]->sizex+objArray[x]->sizey)/4){
						colided=true;
						objArray[x]->isAlive=false;
						objArray[x]->rePosition();

						if(!p1->fakePlayer)
						{
							p1->points++;

							Mix_PlayChannel(-1, sfx[SLASH_SOUND], 0);
							
							if(p1->sword->size >= 15)
								p1->sword->size-=4;
						}

						objArray[x]->isAlive=true;
						//cout << "p1_points = \n"<< p1->points << endl;

						//break;
					}
				}
			}

		}

	// Recurso utilizado para que o player não spawne em cima de um monstro e já morra de primeira vez, A.K.A.: migué =D
	p1->fakePlayer=false;

	// Movimentação do Player

		// Interpretar movimentação do player (com aceleração) || FAZER ANDAR
			if(keyState[(int)('w')])
			{
				if(p1->vy<p1->vmax)
		        	p1->vy+=2;
		        else
		        	p1->vy=p1->vmax;

		        if(p1->sword->swordMode==SWORD_KEY)
		        	p1->sword->rotation=90;
			}
			else if(keyState[(int)('s')])
		    {
				if(p1->vy>-p1->vmax)
		        	p1->vy-=2;
		        else
		        	p1->vy=-p1->vmax;

		        if(p1->sword->swordMode==SWORD_KEY)
		        	p1->sword->rotation=270;
			}
			if(keyState[(int)('d')])
		    {
				if(p1->vx<p1->vmax)
		        	p1->vx+=2;
		        else
		        	p1->vx=p1->vmax;

		        p1->frame_orientation=1;

		        if(p1->sword->swordMode==SWORD_KEY)
		        	p1->sword->rotation=0;
			}
		    else if(keyState[(int)('a')])
		    {
				if(p1->vx>-p1->vmax)
		        	p1->vx-=2;
		        else
		        	p1->vx=-p1->vmax;

		        p1->frame_orientation=-1;

		        if(p1->sword->swordMode==SWORD_KEY)
		       		p1->sword->rotation=180;
			}
			// Corrigir velocidade na diagonal (para não ficar mais rápido)
			if(keyState[(int)('w')] && keyState[(int)('d')])
			{
				p1->vx/=1.25; // 1/sqrt(2) (valor para fazer com que a hipotenusa seja 1 || módulo do vetor na diagonal seja 1)
				p1->vy/=1.25; // (menos um pouco na verdade, pra dar mais a sensação de que as velocidades estão próximas)
				if(p1->sword->swordMode==SWORD_KEY)
					p1->sword->rotation=45;
			}

			else if(keyState[(int)('w')] && keyState[(int)('a')])
			{
				p1->vx/=1.25; // 1/sqrt(2) (valor para fazer com que a hipotenusa seja 1 || módulo do vetor na diagonal seja 1)
				p1->vy/=1.25; // (menos um pouco na verdade, pra dar mais a sensação de que as velocidades estão próximas)
				if(p1->sword->swordMode==SWORD_KEY)
					p1->sword->rotation=135;
			}

			else if(keyState[(int)('s')] && keyState[(int)('a')])
			{
				p1->vx/=1.25; // 1/sqrt(2) (valor para fazer com que a hipotenusa seja 1 || módulo do vetor na diagonal seja 1)
				p1->vy/=1.25; // (menos um pouco na verdade, pra dar mais a sensação de que as velocidades estão próximas)
				if(p1->sword->swordMode==SWORD_KEY)
					p1->sword->rotation=225;
			}

			else if(keyState[(int)('s')] && keyState[(int)('d')])
			{
				p1->vx/=1.25; // 1/sqrt(2) (valor para fazer com que a hipotenusa seja 1 || módulo do vetor na diagonal seja 1)
				p1->vy/=1.25; // (menos um pouco na verdade, pra dar mais a sensação de que as velocidades estão próximas)
				if(p1->sword->swordMode==SWORD_KEY)
					p1->sword->rotation=315;
			}

		// FAZER PARAR
			if(!keyState[(int)('a')] && !keyState[(int)('d')])
				p1->vx/=p1->fric;
			if(!keyState[(int)('w')] && !keyState[(int)('s')])
				p1->vy/=p1->fric;

		// Perceber qual borda do mapa colidiu
			bool left_wall=false,right_wall=false,top_wall=false,bottom_wall=false;

			if(p1->x + WIDTH/2 >= MAP_BORDERX-10)
				right_wall=true;
			if(p1->x - WIDTH/2 <= 10)
				left_wall=true;
			if(p1->y + HEIGHT/2 >= MAP_BORDERY-10)
				top_wall=true;
			if(p1->y - HEIGHT/2 <= 10)
				bottom_wall=true;

		// Interromper movimento nas paredes do mundo
			if((right_wall && keyState['d']) || (left_wall && keyState['a']))
				p1->vx=0;
			if((top_wall && keyState['w']) || (bottom_wall && keyState['s']))
				p1->vy=0;

		// Passar a movimentação do player para a camera
			cam->x+=p1->vx*p1->vmult;
			cam->y+=p1->vy*p1->vmult;

		// Guardar pos no universo do player (e espada)
			p1->x=cam->x+p1->localx;
			p1->y=cam->y+p1->localy;
			p1->sword->x=p1->x;
			p1->sword->y=p1->y;

	// Movimentação dos inimigos
	if(objArray[0]->delay_randomv>=2) // CASO DE TEMPO, APRENDER A UTILIZAR STATIC "ATRIBUTO" PARA TER APENAS UM FRAME_DELAY PARA TODOS DA CLASSE
	{
		objArray[0]->canRand=true;
	}
	objArray[0]->delay_randomv++;
	
	for(int x=0;x<MAX_COLLECTABLES;x++)
	{
		if(objArray[0]->canRand)
		{
			objArray[x]->vx=rand()*20-10;
			objArray[x]->vy=rand()*20-10;
			objArray[x]->canRand=false;
		}

		if(objArray[x]->x >= MAP_BORDERX-500 || objArray[x]->x <= 500)
			objArray[x]->vx*=-1;
		if(objArray[x]->y >= MAP_BORDERY-400 || objArray[x]->y <= 400)
			objArray[x]->vy*=-1;

		if(!(dynamic_cast<Pixie*>(objArray[x])))
		{
			objArray[x]->x+=objArray[x]->vx;
			objArray[x]->y+=objArray[x]->vy;

			if(objArray[x]->vx>=0)
				objArray[x]->frame_orientation=1;
			else
				objArray[x]->frame_orientation=-1;
		}		

		
	}

	// Buff/Debuff Timers

		// Speed Bless
			if(p1->vmult != 1)
			{
				p1->vmult_reset_count++;
				if(p1->vmult_reset_count>=FPS*5)
				{
					p1->vmult_reset_count=0;
					p1->vmult=1;
				}
			}
		// Aura Bless
			if(p1->bless)
			{
				p1->bless_reset_count++;
				if(p1->bless_reset_count>=FPS*7)
				{
					p1->bless_reset_count=0;
					p1->bless=false;
				}
			}
}