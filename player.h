#ifndef  PLAYER_H // <- Prevents multiple inclusions
#define PLAYER_H // <- ...

#include <bits/stdc++.h>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

/**************************/
// your stuff goes here

// classes
class Sword{
	public:
		double x,y,
			   localx,localy;
		double size,fixed_width;
		double rotation;
		double xoffset,yoffset;

	Sword(double width,double height);
};

class Player{
	public:
		double x,y,	// pos no universo (necessario para colisoes (isso+local))
				localx,localy,	// pos no mundo (tela)
				lastx,lasty; // ultima pos (do objeto no universo)(para calcular vetor de direção)
		double xaxis,yaxis; //direção/sentido
		double size; //lado do quadrado|tamnho do player
		double vx,vy,vmax,fric; //velocidade nos eixos x,y, o maximo que o jogador pode atingir e a fricção com o ambiente
		bool fakePlayer; // deixa o player existir, porem as interações ao seu redor não afetam
		int isAlive;	//player vivo? vidas?
		int points;
		int frame,total_frames;
		int frame_orientation;
		Sword *sword;
	
	Player(double width,double height);
};

class GUI
{
	public:
		bool restart;
		double restartX, restartY, restartSize;
		int textureRestart;
		
	GUI(double width, double height);
};

class Camera
{
	public:
		double x,y;
		double size;
		GUI *gui;
	
	Camera(double width,double height);
};

// function prototypes, etc.

// reservar nomes

#endif // PLAYER_H