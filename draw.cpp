#include <stdio.h>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "player.h"

using namespace std;

void drawObject(double x, double y, double z, double size,Camera *cam, bool canDraw, GLuint texture)
{
	/*
		A ideia aqui é transladar todo o fundo/objetos de acordo com o input/movimentação do player
		Transladamos uma primeira vez para efeito de "rolagem de camera" e
		transladamos uma segunda vez para utilizar a ideia de "desenhar com base no próprio sistema de coordenadas do objeto"
	*/

	if(!canDraw) return;
	
	size/=2;
	
	glPushMatrix();
	
		glLoadIdentity();
		glTranslated(-cam->x,-cam->y, 0);
		
		glPushMatrix();
			glTranslatef(x,y,z);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
			
			//Remover interpolação entre pixels
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2d(0,0); glVertex3d(-size,-size,0);
				glTexCoord2d(1,0); glVertex3d(size,-size,0);
				glTexCoord2d(1,1); glVertex3d(size,size,0);
				glTexCoord2d(0,1); glVertex3d(-size,size,0);
			glEnd();

			glDisable(GL_TEXTURE_2D);
		
		glPopMatrix();
	
	glPopMatrix();
}

void drawPlayer(double x, double y, double z, double size, double fixed_width,double degree, GLuint texture)
{
	size/=2;
	fixed_width/=2;

	glPushMatrix();
	
		glLoadIdentity();
		glTranslated(x,y,z);

		glRotated(degree,0,0,1);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Remover interpolação entre pixels
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		
		if(fixed_width==0)
		{
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2d(0,0); glVertex3d(-size,-size,z);
				glTexCoord2d(1,0); glVertex3d(size,-size,z);
				glTexCoord2d(1,1); glVertex3d(size,size,z);
				glTexCoord2d(0,1); glVertex3d(-size,size,z);
			glEnd();
		}
		else
		{
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2d(0,0); glVertex3d(-fixed_width,-size,z);
				glTexCoord2d(1,0); glVertex3d(fixed_width,-size,z);
				glTexCoord2d(1,1); glVertex3d(fixed_width,size,z);
				glTexCoord2d(0,1); glVertex3d(-fixed_width,size,z);
			glEnd();	
		}

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}