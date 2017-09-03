#include "player.h"
#include "objects.h"
#include <iostream>
#include <math.h>

#define WIDTH 1024
#define HEIGHT 768

#define UP 1
#define DOWN -1
#define RIGHT 1
#define LEFT -1

#define SWORD_INCREMENT 4
#define SWORD_MAX_SIZE 100

enum GAME_STATE{MENU=-1,DEAD,PAUSE,QUIT,GAME_0};

using namespace std;

bool canPause=true;
// Booleans para colisão com as paredes
bool left_wall=false,right_wall=false,top_wall=false,bottom_wall=false;

double modulo(double x)
{ return x>=0 ? x:-x; }

void reset(Player *p1, Camera *cam, Collectable **objArray,int objCount) // Reseta o jogo (camera, personagem, e monstros com novas posições)
{
	for(int x=0;x<objCount;x++)
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

void calculatePhysics(Player *p1, Camera *cam,bool keyState[],Collectable **objArray,int objCount,int map_border,int *gameState)
{
	// Sair do jogo
	if(keyState[27])
		*gameState=QUIT;
	// Pausar jogo
	if(keyState['p'] && cam->gui->canPause && cam->gui->pause==false)
	{
		*gameState=PAUSE;
		cam->gui->canPause=false;
		cam->gui->pause=true;
	}
	// Resetar jogo
	if(keyState['r'])
		*gameState=DEAD; // Por enquanto não tem volta

	// Pausar o jogo
	/*if(keyState['p'] && canPause)
    {
    	canPause=false;
    	glutTimerFunc(10000,enablePause,0);

    	if(*gameState==PAUSE)
    		*gameState=GAME_0;
    	else
    		*gameState=PAUSE;
    }*/

	// Calcular colisão
		bool colided = false;
		
		for(int x=0;x<objCount;x++)
		{
			// Colisão com o JOGADOR
			if(objArray[x]->isAlive && objArray[x]->x - objArray[x]->size/2 < p1->x+p1->size/2 
									&& objArray[x]->x + objArray[x]->size/2 > p1->x-p1->size/2 
									&& objArray[x]->y - objArray[x]->size/2 < p1->y+p1->size/2 
									&& objArray[x]->y + objArray[x]->size/2 > p1->y-p1->size/2)
			{
				colided=true;
				objArray[x]->isAlive=false;
				objArray[x]->rePosition();

				if(!p1->fakePlayer)
				{
					if(objArray[x]->canKill)
						*gameState=DEAD;
					else
					{
						p1->points++;
						if(p1->sword->size <= SWORD_MAX_SIZE)
							p1->sword->size+=SWORD_INCREMENT;
					}
				}

				objArray[x]->isAlive=true;
				cout << "p1_points = \n"<< p1->points << endl;
			}
			// Colisão com a ESPADA
			/*if(objArray[x]->isAlive && objArray[x]->x - objArray[x]->size/2 < (p1->sword->x+p1->sword->size)*cos(p1->sword->rotation*360/M_PI)
									&& objArray[x]->x + objArray[x]->size/2 > 0 
									&& objArray[x]->y - objArray[x]->size/2 < (p1->sword->y+p1->sword->fixed_width)*sin(p1->sword->rotation*360/M_PI)
									&& objArray[x]->y + objArray[x]->size/2 > 0)
			{
				colided=true;
				objArray[x]->isAlive=false;
				p1->points++;
				p1->sword->size+=SWORD_INCREMENT;
				objArray[x]->rePosition();
				if(objArray[x]->canKill)
					reset(p1,cam,gameState);
				cout << "p1_points = \n"<< p1->points << endl;
			}*/
		}

		/*cout<<"p1->xaxis = "<<p1->xaxis<<endl;
		cout<<"p1->yaxis = "<<p1->yaxis<<endl;*/

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

		        p1->sword->rotation=90;
			}
			if(keyState[(int)('s')])
		    {
				if(p1->vy>-p1->vmax)
		        	p1->vy-=2;
		        else
		        	p1->vy=-p1->vmax;

		        p1->sword->rotation=-90;
			}
			if(keyState[(int)('d')])
		    {
				if(p1->vx<p1->vmax)
		        	p1->vx+=2;
		        else
		        	p1->vx=p1->vmax;

		        p1->frame_orientation=1;

		        p1->sword->rotation=0;
			}
		    if(keyState[(int)('a')])
		    {
				if(p1->vx>-p1->vmax)
		        	p1->vx-=2;
		        else
		        	p1->vx=-p1->vmax;

		        p1->frame_orientation=-1;

		        p1->sword->rotation=180;
			}
			// Corrigir velocidade na diagonal (para não ficar mais rápido)
			if((keyState[(int)('w')] && keyState[(int)('d')]) || (keyState[(int)('w')] && keyState[(int)('a')]) 
					|| (keyState[(int)('s')] && keyState[(int)('a')]) || (keyState[(int)('s')] && keyState[(int)('d')]))
			{
				p1->vx/=1.25; // 1/sqrt(2) (valor para fazer com que a hipotenusa seja 1 || módulo do vetor na diagonal seja 1)
				p1->vy/=1.25; // (menos um pouco na verdade, pra dar mais a sensação de que as velocidades estão próximas)
			}

		// FAZER PARAR
			if(!keyState[(int)('a')] && !keyState[(int)('d')])
				p1->vx/=p1->fric;
			if(!keyState[(int)('w')] && !keyState[(int)('s')])
				p1->vy/=p1->fric;

		// Perceber qual borda do mapa colidiu
			bool left_wall=false,right_wall=false,top_wall=false,bottom_wall=false;

			if(p1->x + WIDTH/2 >= WIDTH/2 + map_border/2)
				right_wall=true;
			if(p1->x - WIDTH/2 <= WIDTH/2 - map_border/2)
				left_wall=true;
			if(p1->y + HEIGHT/2 >= HEIGHT/2 + map_border/2)
				top_wall=true;
			if(p1->y - HEIGHT/2 <= HEIGHT/2 - map_border/2)
				bottom_wall=true;

		//if((!left_wall && !right_wall && !top_wall && !bottom_wall) || contraryDir(keyState)) // NAO TA ROLANDO AINDA
		{
			// Passar a movimentação do player para a camera
				cam->x+=p1->vx;
				cam->y+=p1->vy;

			// Guardar pos no universo do player (e espada)
				p1->x=cam->x+p1->localx;
				p1->y=cam->y+p1->localy;
				p1->sword->x=p1->x;
				p1->sword->y=p1->y;
		}

		// Calcular direção do player
			if(p1->x != p1->lastx)
				p1->xaxis=(p1->x-p1->lastx)/modulo(p1->x-p1->lastx);
			if(p1->y != p1->lasty)
				p1->yaxis=(p1->y-p1->lasty)/modulo(p1->y-p1->lasty);
}