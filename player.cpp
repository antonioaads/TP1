#include "player.h"

enum GUI_TEXTURES{RESTARTGREYED=0,RESTARTBRIGHT,RESTARTBRIGHTYES,RESTARTBRIGHTNO,QUITGREYED,QUITBRIGHT,QUITBRIGHTYES,QUITBRIGHTNO,TEXPAUSE}; 

// Classes Methods
Sword::Sword(double width,double height)
{
	this->x=width/2;
	this->y=height/2;
	this->size=100;
	this->fixed_width=50;
	this->rotation=0;
}

Player::Player(double width,double height)
{
	this->x=width/(float)2;
	this->y=height/(float)2;

	this->localx=width/(float)2;
	this->localy=height/(float)2;
	
	this->lastx=this->x;
	this->lasty=this->y;
	
	this->sizey=100;
	this->sizex=75;
	this->points=0;
	
	this->frame=1;
	this->total_frames=1;
	this->frame_orientation=1;
	this->frame_delay=0;

	this->sword=new Sword(width,height);

	this->fakePlayer=true;

	this->vx=0;
	this->vy=0;
	this->vmax=10;
	this->fric=1.1;
	
	this->isAlive=1;
}

GUI::GUI(double width, double height)
{
	this->restart=false;
	this->restartSize=600;
	this->restartX=width/2;
	this->restartY=height/2;
	this->textureRestart=RESTARTGREYED;

	this->quit=false;
	this->quitSize=600;
	this->quitX=width/2;
	this->quitY=height/2;
	this->textureQuit=QUITGREYED;

	this->pause=false;
	this->canPause=true;
	this->pauseSize=600;
	this->pauseX=width/2;
	this->pauseY=height/2;
	this->texturePause=TEXPAUSE;
}

Camera::Camera(double width,double height)
{
	this->x=0;
	this->y=0;
	this->size=100;
	this->gui=new GUI(width,height);
}