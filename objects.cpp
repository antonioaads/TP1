#include "objects.h"

void initObjs(Collectable *objArray,double map_limits_x,double map_limits_y)
{
	for(int x;x<1000;x++)
	{
		objArray[x]= new Collectable(map_limits_x,map_limits_y);
	}
}