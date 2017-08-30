#ifndef  INPUT_H // <- Prevents multiple inclusions
#define INPUT_H // <- ...

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "global.h"

/**************************/
// your stuff goes here
// function prototypes, etc.
	void key_press_callback(unsigned char key,int x,int y);
	void key_release_callback(unsigned char key,int x,int y);
	void passive_mouse_callback(int x, int y);
// classes
// reservate names
	//(declaration on module.c/cpp needed)

#endif // INPUT_H