#ifndef  PLAYER_H // <- Prevents multiple inclusions
#define PLAYER_H // <- ...

/**************************/
// your stuff goes here

// function prototypes, etc.

// classes
class Sword{
	public:
		double x,y,
			   localx,localy;
		double size,fixed_width;
		double rotation;
		double xoffset,yoffset;

	Sword(double width,double height)
	{
		this->x=width/2;
		this->y=height/2;
		this->size=100;
		this->fixed_width=this->size;
		this->rotation=-90;
		this->xoffset=20;
		this->yoffset=20;
	}
};

class Player{
	public:
		double x,y,	// pos no universo (necessario para colisoes (isso+local))
				localx,localy,	// pos no mundo (tela)
				lastx,lasty; // ultima pos (no universo)
		double xaxis,yaxis; //direção/sentido
		double size; //lado do quadrado|tamnho do player
		double vx,vy,vmax,fric; //velocidade nos eixos x,y, o maximo que o jogador pode atingir e a fricção com o ambiente
		int isAlive;	//player vivo? vidas?
		int points;
		int frame,total_frames;
		int frame_orientation;
		Sword *sword;
	
	Player(double width,double height)
	{
		this->x=width/(float)2;
		this->y=height/(float)2;
		this->localx=width/(float)2;
		this->localy=height/(float)2;
		this->lastx=this->x;
		this->lasty=this->y;
		this->size=100;
		this->points=0;
		this->frame=1;
		this->total_frames=1;
		this->frame_orientation=1;

		this->sword=new Sword(width,height);

		this->vx=0;
		this->vy=0;
		this->vmax=10;
		this->fric=1.1;
		
		this->isAlive=0;
	}
};

class Camera
{
	public:
		double x,y;
		double size;
	
	Camera(void)
	{
		this->x=0;
		this->y=0;
		this->size=100;
	}
};

// reservar nomes

#endif // PLAYER_H