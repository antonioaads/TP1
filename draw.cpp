#include <stdio.h>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "global.h"

using namespace std;

void importTextures(GLuint &texturePlayer, GLuint &textureBackground)
{
	texturePlayer = SOIL_load_OGL_texture("tex/player.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
	textureBackground = SOIL_load_OGL_texture("tex/background.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

	if (texturePlayer == 0) 
      printf("Erro do SOIL: '%s'\n", SOIL_last_result());
}

void drawObject(double x, double y, double z, double size,Camera *cam,GLuint texture)
{
	/*
		A ideia aqui é transladar todo o fundo/objetos de acordo com o input/movimentação do player
		Transladamos uma primeira vez para efeito de "rolagem de camera" e
		transladamos uma segunda vez para utilizar a ideia de "desenhar com base no próprio sistema de coordenadas do objeto"
	*/

	size/=2;
	
	glPushMatrix();
	
		glLoadIdentity();
		glTranslatef(-cam->x,-cam->y, 0);
		
		glPushMatrix();
			glTranslatef(x,y,z);

			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
			
			//Remover interpolação entre pixels
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2d(0,0); glVertex3f(-size,-size,0);
				glTexCoord2d(1,0); glVertex3f(size,-size,0);
				glTexCoord2d(1,1); glVertex3f(size,size,0);
				glTexCoord2d(0,1); glVertex3f(-size,size,0);
			glEnd();

			glDisable(GL_TEXTURE_2D);
		
		glPopMatrix();
	
	glPopMatrix();
}

void drawPlayer(double x, double y, double z, double size, GLuint texture)
{
	size/=2;
	
	glPushMatrix();
	
		glLoadIdentity();
		glTranslatef(x,y,z);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Remover interpolação entre pixels
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
			
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2d(0,0); glVertex3f(-size,-size,z);
			glTexCoord2d(1,0); glVertex3f(size,-size,z);
			glTexCoord2d(1,1); glVertex3f(size,size,z);
			glTexCoord2d(0,1); glVertex3f(-size,size,z);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}