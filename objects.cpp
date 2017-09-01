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