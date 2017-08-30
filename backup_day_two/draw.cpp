#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "global.h"
#include "draw.h"

using namespace std;

void drawObject(double x, double y, double z, double size)
{
	size/=2;
	
	glPushMatrix();
	
		glLoadIdentity();
		glTranslatef(-cam.x,-cam.y, 0);
		
		glPushMatrix();
			glTranslatef(x,y,z);
			
			glBegin(GL_TRIANGLE_FAN);
				glColor3f(0,0,1); glVertex3f(-size,-size,0);
				glColor3f(0,1,0); glVertex3f(size,-size,0);
				glColor3f(1,0,0); glVertex3f(size,size,0);
				glColor3f(0,0,0); glVertex3f(-size,size,0);
			glEnd();
		
		glPopMatrix();
	
	glPopMatrix();
}

void drawPlayer(double x, double y, double z, double size)
{
	size/=2;
	
	glPushMatrix();
	
		glLoadIdentity();
		glTranslatef(x,y,z);
			
		glBegin(GL_TRIANGLE_FAN);
			glVertex3f(-size,-size,z);
			glVertex3f(size,-size,z);
			glVertex3f(size,size,z);
			glVertex3f(-size,size,z);
		glEnd();
	
	glPopMatrix();
}


void draw_callback(void){
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Desenhar Background
        drawObject(WIDTH/2,HEIGHT/2,0,HEIGHT*2);

    // Desenhar Player
        glColor3f(0, 1, 0); // verde
        drawPlayer(p1.x,p1.y,0,p1.size);
    
    glutSwapBuffers();
}