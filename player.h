#ifndef  PLAYER_H // <- Prevents multiple inclusions
#define PLAYER_H // <- ...

#include "global.h"

/**************************/
// your stuff goes here

// function prototypes, etc.

// classes
class Player{
	public:
		double x,y,	// pos no universo (necessario para colisoes (isso+local))
				localx,localy,	// pos no mundo (tela)
				lastx,lasty; // ultima pos (no universo)
		double xaxis,yaxis; //direção/sentido
		double size; //lado do quadrado|tamnho do player
		double vx,vy,vmax,fric; //velocidade nos eixos x,y, o maximo que o jogador pode atingir e a fricção com o ambiente
		int alive;	//player vivo? vidas?
	
	Player(double width,double height)
	{
		this->localx=width/(float)2;
		this->localy=height/(float)2;
		this->lastx=this->x;
		this->lasty=this->y;
		this->size=100;

		this->vx=0;
		this->vy=0;
		this->vmax=10;
		this->fric=1.1;
		
		this->alive=0;
	}
};

// reservar nomes

#endif // PLAYER_H