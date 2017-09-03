#ifndef  PHYSICS_H // <- Prevents multiple inclusions
#define PHYSICS_H // <- ...

#include "objects.h"

/**************************/
// your stuff goes here
// function prototypes, etc.
	void calculatePhysics(Player*,Camera*,bool*,Collectable**,int,int,int*);
	void reset(Player *p1, Camera *cam, Collectable **objArray,int objCount);
// classes
// reservate names

#endif // PHYSICS_H