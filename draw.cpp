#include <stdio.h>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "player.h"

using namespace std;

void drawBg(double x, double y, double z, double size,Camera *cam, bool canDraw, GLuint texture)
{
	/*
		A ideia aqui é transladar todo o fundo/objetos de acordo com o input/movimentação do player
		Transladamos uma primeira vez para efeito de "rolagem de camera"
	*/

	if(!canDraw) return;
	
	size/=2;
	
	glPushMatrix();
	
		glLoadIdentity();
		glTranslated(-cam->x,-cam->y, 0);

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
}

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

void drawPlayer(double x, double y, double z, double size, double degree, GLuint texture,int frame,int total_frames,int frame_orientation)
{
	/*
		Nesta função, a origem é transladada para a posição local do player e utilizada como referencia para desenhar o mesmo a partir do centro
		(referencia no meio do personagem)
	*/
	size/=2;

	glPushMatrix();
	
		glLoadIdentity();
		glTranslated(x,y,z);

		glRotated(degree,0,0,1);

		glScaled(frame_orientation,1,1);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Remover interpolação entre pixels
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2d(0, 0); glVertex3d(-size,-size,z);
			glTexCoord2d(frame*(1/total_frames), 0); glVertex3d(size,-size,z);
			glTexCoord2d(frame*(1/total_frames), frame*(1/total_frames)); glVertex3d(size,size,z);
			glTexCoord2d(0, frame*(1/total_frames)); glVertex3d(-size,size,z);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}

void drawSword(double x, double y, double z, double size, double fixed_width,double degree, GLuint texture,int frame,int total_frames,int frame_orientation)
{
	/*
		Nesta função, a origem é transladada para a posição local da espada e utilizada como referencia para desenhar a mesma a partir do canto inferior esquerdo
		(lembrando, esse canto é virtual, ou seja, "inferior esquerdo" é uma referencia anterior da rotação do objeto (quando o mesmo está na posição horizontal para a direita))
	*/

	glPushMatrix();
	
		glLoadIdentity();
		glTranslated(x,y,z);

		glRotated(degree,0,0,1);

		glScaled(frame_orientation,1,1);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Remover interpolação entre pixels
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2d(0, 0); glVertex3d(0,0,z);
			glTexCoord2d(frame*(1/total_frames), 0); glVertex3d(size,0,z);
			glTexCoord2d(frame*(1/total_frames), frame*(1/total_frames)); glVertex3d(size,fixed_width,z);
			glTexCoord2d(0, frame*(1/total_frames)); glVertex3d(0,fixed_width,z);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}

void drawOverlay(double x, double y, double z, double size, bool canDraw, GLuint texture)
{
	if(canDraw)
	{
		size/=2;
	
		glPushMatrix();
		
			glLoadIdentity();
			glTranslated(x,y,z);
	
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
	
			//Remover interpolação entre pixels
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	  			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2d(0, 0); glVertex3d(-size,-size,z);
				glTexCoord2d(1, 0); glVertex3d(size,-size,z);
				glTexCoord2d(1, 1); glVertex3d(size,size,z);
				glTexCoord2d(0, 1); glVertex3d(-size,size,z);
			glEnd();
	
			glDisable(GL_TEXTURE_2D);
		
		glPopMatrix();
	}
}