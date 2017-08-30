#ifndef  GLOBAL_H // <- Prevents multiple inclusions
#define GLOBAL_H // <- ...

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60

/**************************/
// your stuff goes here

// function prototypes, etc.

// classes
class Player{
	public:
		double x,y;
		double size;
		int alive;
	
	Player(void)
	{
		this->x=WIDTH/(float)2;
		this->y=HEIGHT/(float)2;
		this->size=100;
		this->alive=0;
		
	}
};

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

// reservar nomes
extern int keyState[300];
extern Player p1;
extern Camera cam;

#endif // GLOBAL_H