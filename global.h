#ifndef  GLOBAL_H // <- Prevents multiple inclusions
#define GLOBAL_H // <- ...

#include "player.h"

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#define MAP_LIMITS_X 2000
#define MAP_LIMITS_Y 2000

// gameStates
	#define MENU -1
	#define GAME_0 0

/**************************/
// your stuff goes here

// classes
class Camera
{
	public:
		double x,y;
		double size;
	
	Camera(void)
	{
		this->x=0;
		this->y=0;
		this->size=100;
	}
};


// function prototypes, etc.
void stateMachine(int,int*,Player,Camera);

#endif // GLOBAL_H