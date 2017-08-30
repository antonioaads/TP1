#include "objects.h"

void initObjs(Collectable *objArray,map_limits_x,map_limits_y)
{
	for(int x;x<1000;x++)
	{
		objArray[x]((double)map_limits_x,(double)map_limits_y);
	}
}