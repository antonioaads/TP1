#include "objects.h"

enum COLLECTABLE_TYPE { PIXIE=0, DEMON };
#define COLLECTABLE_TYPE_COUNT 2

void initObj(Collectable **objArray,int max_colllectables,double map_border)
{
	for(int x=0;x<max_colllectables;x++)
	{
		switch(x % COLLECTABLE_TYPE_COUNT)
		{
			case PIXIE:
				objArray[x] = new Pixie(map_border);
			break;
			case DEMON:
				objArray[x] = new Demon(map_border);
			break;
		}
	}
}

// Classes Methods

Collectable::Collectable(double map_border)
{
	this->map_border = map_border/2;

	this->isAlive=true;

	this->size=60;

	this->x=(int)rand()%(int)(this->map_border);
	this->y=(int)rand()%(int)(this->map_border);
}

void Collectable::rePosition()
{
	this->x=(int)rand()%(int)(this->map_border);
	this->y=(int)rand()%(int)(this->map_border);
}