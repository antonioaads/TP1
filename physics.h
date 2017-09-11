#ifndef  PHYSICS_H // <- Prevents multiple inclusions
#define PHYSICS_H // <- ...

#include "objects.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

/**************************/
// your stuff goes here
// function prototypes, etc.
	void calculatePhysics(Player *p1, Camera *cam,bool keyState[],Collectable **objArray, int *gameState, Mix_Chunk **sfx);
	void reset(Player *p1, Camera *cam, Collectable **objArray);
	double modulo(double x);
// classes
// reservate names

#endif // PHYSICS_H