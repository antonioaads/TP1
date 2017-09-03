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
		double size;
		bool isAlive;
		bool canKill;

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
		Collectable::canKill=false;
	}
};

class Demon : public Collectable
{
	public:

	Demon(double map_border) : Collectable(map_border)
	{
		Collectable::canKill=true;
		Collectable::size=100;
	}
};

// function prototypes, etc.
void initObj(Collectable**,int,double);
// reservate names
	//(declaration on module.c/cpp needed)

#endif // OBJECTS_H