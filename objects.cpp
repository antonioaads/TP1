#include "objects.h"	

void initObj(Collectable *objArray,int max_colllectables,double map_limits_x,double map_limits_y)
{
	for(int x;x<max_colllectables;x++)
	{
		(objArray[x]).setup(map_limits_x,map_limits_y);
	}
}