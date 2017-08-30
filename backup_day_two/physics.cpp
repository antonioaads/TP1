#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "global.h"
#include "physics.h"

using namespace std;

void calculatePhysics()
{
	if(keyState[(int)('w')]==1)
        cam.y+=2;
    if(keyState[(int)('a')]==1)
        cam.x-=2;
    if(keyState[(int)('d')]==1)
        cam.x+=2;
    if(keyState[(int)('s')]==1)
        cam.y-=2;
}