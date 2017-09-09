#ifndef  OBJECTS_H // <- Prevents multiple inclusions
#define OBJECTS_H // <- ...

#include <bits/stdc++.h>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

/**************************/
// your stuff goes here
// classes
class Collectable
{
	public:
		double x,y;
		double vx,vy;
		int delay_randomv,canRand; // Para aleatorizar velocidade
		int frame,total_frames,frame_delay,frame_orientation;
		double sizex,sizey;
		bool isAlive;
		bool canKill;
		int bufftime;

		double map_border;

	Collectable(double map_border);

	void rePosition();

	virtual ~Collectable() // Destrutor virtual - só pra caracterizar polimorfismo (pode ser qualquer função como virtual, desde que seja pelo menos uma)
	{}
};

class Pixie : public Collectable
{
	public:
		double multiply_speed;

	Pixie(double map_border) : Collectable(map_border)
	{
		this->multiply_speed=2;
		Collectable::total_frames=4;
		Collectable::canKill=false;
		Collectable::sizex=100;
		Collectable::sizey=100;
	}
};

class Demon : public Collectable
{
	public:

	Demon(double map_border) : Collectable(map_border)
	{
		Collectable::canKill=true;
		Collectable::total_frames=4;
		Collectable::sizex=75;
		Collectable::sizey=100;
	}
};

class Miko : public Collectable
{
	public:
	Miko(double map_border) : Collectable(map_border)
	{
		Collectable::canKill=false;
		Collectable::total_frames=4;
		Collectable::sizex=75;
		Collectable::sizey=150;
	}
};

class Kitsune : public Collectable
{
	public:
	Kitsune(double map_border) : Collectable(map_border)
	{
		Collectable::canKill=false;
		Collectable::total_frames=3;
		Collectable::sizex=100;
		Collectable::sizey=100;
	}
};

// function prototypes, etc.
void initObj(Collectable**,int,double);
// reservate names
	//(declaration on module.c/cpp needed)

#endif // OBJECTS_H