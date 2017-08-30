#ifndef  OBJECTS_H // <- Prevents multiple inclusions
#define OBJECTS_H // <- ...

#include <bits/stdc++.h>

/**************************/
// your stuff goes here
// classes
class Collectable
{
	public:
		double x,y;
		double size;
		bool isAlive;

		double map_limits_x;
		double map_limits_y;

	Collectable()
	{
		this->map_limits_x = 0;
		this->map_limits_y = 0;

		this->isAlive=true;

		this->size=60;

		this->x=(int)rand()%(int)(2000);
		this->y=(int)rand()%(int)(2000);
	}

	void setup(double map_limits_x,double map_limits_y)
	{
		this->map_limits_x = map_limits_x;
		this->map_limits_y = map_limits_y;

		this->isAlive=true;

		this->size=60;

		this->x=(int)rand()%(int)(map_limits_x-100);
		this->y=(int)rand()%(int)(map_limits_y-100);
	}
};
/*
class Pixie : public Collectable
{
	public:
		double multiply_speed;

	Banana(double map_limits_x,double map_limits_y) : Collectable(map_limits_x, map_limits_y)
	{
		this->multiply_speed=2;
	}
};*/

// function prototypes, etc.
void initObj(Collectable*,int,double,double);
// reservate names
	//(declaration on module.c/cpp needed)

#endif // OBJECTS_H