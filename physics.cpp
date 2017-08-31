#include "player.h"
#include "objects.h"
#include <iostream>

using namespace std;

double modulo(double x)
{
	return x>=0 ? x:-x;
}

void calculatePhysics(Player *p1, Camera *cam,int keyState[],Collectable *objArray,int objCount)
{
	// Calcular colisão
		for(int x=0;x<objCount;x++)
		{
			if(objArray[x].x - objArray[x].size/2 < p1->x+p1->size/2 && objArray[x].x + objArray[x].size/2 > p1->x-p1->size/2 && 
			   objArray[x].y - objArray[x].size/2 < p1->y+p1->size/2 && objArray[x].y + objArray[x].size/2 > p1->y-p1->size/2)
			{
				cout << "COLIDI\n"<< endl;
				//objArray[x].isAlive=false;
				//p1->points++;
			}
		}

	// Calcular direção do player
		if(p1->x!=p1->lastx)
			p1->xaxis=(p1->x-p1->lastx)/modulo(p1->x-p1->lastx);
		if(p1->y!=p1->lasty)
			p1->yaxis=(p1->y-p1->lasty)/modulo(p1->y-p1->lasty);

	// Movimentação do Player

		// Interpretar movimentação do player (com aceleração) || FAZER ANDAR
			if(keyState[(int)('w')])
			{
				if(p1->vy<p1->vmax)
		        	p1->vy+=2;
		        else
		        	p1->vy=p1->vmax;
			}
			if(keyState[(int)('s')])
		    {
				if(p1->vy>-p1->vmax)
		        	p1->vy-=2;
		        else
		        	p1->vy=-p1->vmax;
			}
			if(keyState[(int)('d')])
		    {
				if(p1->vx<p1->vmax)
		        	p1->vx+=2;
		        else
		        	p1->vx=p1->vmax;
			}
		    if(keyState[(int)('a')])
		    {
				if(p1->vx>-p1->vmax)
		        	p1->vx-=2;
		        else
		        	p1->vx=-p1->vmax;
			}
			// Corrigir velocidade na diagonal (para não ficar mais rápido)
			if((keyState[(int)('w')] && keyState[(int)('d')]) || (keyState[(int)('w')] && keyState[(int)('a')]) || (keyState[(int)('s')] && keyState[(int)('a')]) || (keyState[(int)('s')] && keyState[(int)('d')]))
			{
				p1->vx/=1.25; // 1/sqrt(2) (valor para fazer com que a hipotenusa seja 1 || módulo do vetor na diagonal seja 1)
				p1->vy/=1.25; // (menos um pouco na verdade, pra dar mais a sensação de que as velocidades estão próximas)
			}

		// FAZER PARAR
			if(!keyState[(int)('a')] && !keyState[(int)('d')])
				p1->vx/=p1->fric;
			if(!keyState[(int)('w')] && !keyState[(int)('s')])
				p1->vy/=p1->fric;

		// Passar a movimentação do player para a camera
			cam->x+=p1->vx;
			cam->y+=p1->vy;

		// Guardar pos no universo do player
			p1->x=cam->x;
			p1->y=cam->y;

}