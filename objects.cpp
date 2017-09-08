#include "objects.h"

enum COLLECTABLE_TYPE { PIXIE=0, DEMON, MIKO, KITSUNE};
#define COLLECTABLE_TYPE_COUNT 4

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
			case MIKO:
				objArray[x] = new Miko(map_border);
			break;
			case KITSUNE:
				objArray[x] = new Kitsune(map_border);
			break;
		}
	}
}

// Classes Methods

Collectable::Collectable(double map_border)
{
	this->map_border = map_border;

	this->isAlive=true;

	this->x=(int)rand()%(int)(this->map_border-2000)+500;
	this->y=(int)rand()%(int)(this->map_border-2000)+500;

	this->vx=rand()%20-10;
	this->vy=rand()%20-10;
	this->delay_randomv=0;

	this->canRand = true;

	this->frame=1;
	this->frame_delay=0;
	this->total_frames=1;
	this->frame_orientation=1;
}

void Collectable::rePosition()
{
	this->x=(int)rand()%(int)(this->map_border-2000)+500;
	this->y=(int)rand()%(int)(this->map_border-2000)+500;
}