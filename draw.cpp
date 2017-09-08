#include <stdio.h>
#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "player.h"

using namespace std;

void drawBg(double x, double y, double z, double sizex,double sizey,Camera *cam, bool canDraw, GLuint texture)
{
	/*
		A ideia aqui é transladar todo o fundo/objetos de acordo com o input/movimentação do player
		Transladamos uma primeira vez para efeito de "rolagem de camera"
	*/

	if(!canDraw) return;
	
	glPushMatrix();
	
		glLoadIdentity();
		glTranslated(-cam->x,-cam->y, 0);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		//Remover interpolação entre pixels
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2d(0,0); glVertex3d(0,0,0);
			glTexCoord2d(1,0); glVertex3d(sizex,0,0);
			glTexCoord2d(1,1); glVertex3d(sizex,sizey,0);
			glTexCoord2d(0,1); glVertex3d(0,sizey,0);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}

void drawObject(double x, double y, double z, double size,Camera *cam, bool canDraw, GLuint texture,int frame,int total_frames,int frame_orientation)
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

			glScaled(frame_orientation,1,1);

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

void drawPlayer(double x, double y, double z, double sizex,double sizey, double degree, GLuint texture,int frame,int total_frames,int frame_orientation)
{
	/*
		Nesta função, a origem é transladada para a posição local do player e utilizada como referencia para desenhar o mesmo a partir do centro
		(referencia no meio do personagem)
	*/
	sizex/=2;
	sizey/=2;

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
			glTexCoord2d((frame-1)*((double)1/total_frames), 0); glVertex3d(-sizex,-sizey,z);
			glTexCoord2d(frame*((double)1/total_frames), 0); glVertex3d(sizex,-sizey,z);
			glTexCoord2d(frame*((double)1/total_frames),1); glVertex3d(sizex,sizey,z);
			glTexCoord2d((frame-1)*((double)1/total_frames), 1); glVertex3d(-sizex,sizey,z);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}

double rotationConvert(double xr,double yr,double degree, char coord)
{
	degree = degree*M_PI/180;

	if(coord == 'x') return (xr*cos(degree) - yr*sin(degree)); 		// Conversão do sistema de coordenadas usando fórmula de gaav
	return (xr*sin(degree) + yr*cos(degree));						// x=x'cos + y'sen  ||  y=x'sen + y'cos
}

void drawSword(double x, double y, double z, double size, double fixed_width,double degree, GLuint texture,int frame,int total_frames,int frame_orientation)
{
	/*
		Nesta função, a origem é transladada para a posição local da espada e utilizada como referencia para desenhar a mesma a partir do canto inferior esquerdo
		(lembrando, esse canto é virtual, ou seja, "inferior esquerdo" é uma referencia anterior da rotação do objeto (quando o mesmo está na posição horizontal para a direita))
	*/

	fixed_width/=2;

	glPushMatrix();
	
		glLoadIdentity();
		glTranslated(x,y,z);

		//glRotated(degree,0,0,1);

		glScaled(frame_orientation,1,1);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);

		//Remover interpolação entre pixels
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
  			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		
		glBegin(GL_TRIANGLE_FAN);
			glTexCoord2d(0, 0); glVertex3d(rotationConvert(0,-fixed_width,degree,'x'),rotationConvert(0,-fixed_width,degree,'y'),z);
			glTexCoord2d(frame*(1/total_frames), 0); glVertex3d(rotationConvert(size,-fixed_width,degree,'x'),rotationConvert(size,-fixed_width,degree,'y'),z);
			glTexCoord2d(frame*(1/total_frames), frame*(1/total_frames)); glVertex3d(rotationConvert(size,fixed_width,degree,'x'),rotationConvert(size,fixed_width,degree,'y'),z);
			glTexCoord2d(0, frame*(1/total_frames)); glVertex3d(rotationConvert(0,fixed_width,degree,'x'),rotationConvert(0,fixed_width,degree,'y'),z);
		glEnd();

		glDisable(GL_TEXTURE_2D);
	
	glPopMatrix();
}

void drawOverlay(double x, double y, double z, double sizex,double sizey, bool canDraw, GLuint texture)
{
	if(canDraw)
	{
		sizex/=2;
		sizey/=2;
	
		glPushMatrix();
		
			glLoadIdentity();
			glTranslated(x,y,z);
	
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
	
			//Remover interpolação entre pixels
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	  			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	
			glBegin(GL_TRIANGLE_FAN);
				glTexCoord2d(0, 0); glVertex3d(-sizex,-sizey,z);
				glTexCoord2d(1, 0); glVertex3d(sizex,-sizey,z);
				glTexCoord2d(1, 1); glVertex3d(sizex,sizey,z);
				glTexCoord2d(0, 1); glVertex3d(-sizex,sizey,z);
			glEnd();
	
			glDisable(GL_TEXTURE_2D);
		
		glPopMatrix();
	}
}