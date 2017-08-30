#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "global.h"
#include "input.h"

using namespace std;

void key_press_callback(unsigned char key,int x,int y){ // x,y -> pos. mouse
	if(key==27)
		exit(0);

	keyState[(int)key]=1;
}

void key_release_callback(unsigned char key,int x,int y){
   keyState[(int)key]=0;
}

void passive_mouse_callback(int x, int y){
    
}