#ifndef  OBJECTS_H // <- Prevents multiple inclusions
#define OBJECTS_H // <- ...

#include <bits/stdc++.h>
#include "global.h"

/**************************/
// your stuff goes here
// classes
class Collectable
{
	public:
		double x,y;
		double size;

		static double map_limits_x;
		static double map_limits_y;

	Collectable(double map_limits_x,double map_limits_y)
	{
		this->map_limits_x = map_limits_x;
		this->map_limits_y = map_limits_y;

		this->x=(int)rand()%(int)(map_limits_x-100);
		this->y=(int)rand()%(int)(map_limits_y-100);
	}
};

class Banana : public Collectable
{
	public:
		double multiply_speed;

	Banana(double map_limits_x,double map_limits_y)
	{
		SuperClass(map_limits_x, map_limits_y);
		this->multiply_speed=2;
	}
}

// function prototypes, etc.
void initObjs(Collectable*);
// reservate names
	//(declaration on module.c/cpp needed)

#endif // OBJECTS_H