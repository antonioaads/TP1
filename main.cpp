/*

	Secção de OBS.:
		- Lembrar de trocar colisão do quadrado para "metade inferior do quadrado", como sugere a textura (espada está apenas na metade inferior)
		- Lembrar de perguntar pro fessor se é má ideia guardar texutra em cada classe (talvez utilizar ponteiro ao invés de copiar a textura?)(problema do quadrado branco)
*/

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "player.h"
#include "draw.h"
#include "physics.h"
#include "objects.h"

// Audio
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_mixer.h>

	#define MUS_PATH "music/tutut.ogg" // OGG(e outros) = música de fundo

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#define MAP_BORDER 3000
#define MAX_COLLECTABLES 30

enum GAME_STATE{MENU=-1,DEAD,PAUSE,QUIT,GAME_0};
enum COLLECTABEL_TEXTURES{PIXIE=0,DEMON};
enum GUI_TEXTURES{RESTARTGREYED=0,RESTARTBRIGHT,RESTARTBRIGHTYES,RESTARTBRIGHTNO,QUITGREYED,QUITBRIGHT,QUITBRIGHTYES,QUITBRIGHTNO,TEXPAUSE}; 

using namespace std;

// Arquivo de musica (ponteiro)
	Mix_Music *music = NULL;

// Inicializar variavel
    bool keyState[300];
    int gameState=MENU;

// Instanciar classes
    Player p1(WIDTH,HEIGHT);
    Collectable *objArray[MAX_COLLECTABLES];
    Camera cam((double)WIDTH,(double)HEIGHT);

// Importar texturas
    GLuint texturePlayer;
    GLuint textureSword;
    GLuint textureBackground;
    GLuint textureCollectables[10];
    GLuint textureGUI[20];
    GLuint textureMenu;

void importTextures()
{
    texturePlayer = SOIL_load_OGL_texture("tex/player.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureSword = SOIL_load_OGL_texture("tex/sword.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureBackground = SOIL_load_OGL_texture("tex/background.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[RESTARTGREYED] = SOIL_load_OGL_texture("tex/restartg.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[RESTARTBRIGHT] = SOIL_load_OGL_texture("tex/restartb.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[RESTARTBRIGHTYES] = SOIL_load_OGL_texture("tex/restartbY.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[RESTARTBRIGHTNO] = SOIL_load_OGL_texture("tex/restartbN.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[QUITGREYED] = SOIL_load_OGL_texture("tex/quitg.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[QUITBRIGHT] = SOIL_load_OGL_texture("tex/quitb.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[QUITBRIGHTYES] = SOIL_load_OGL_texture("tex/quitbY.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[QUITBRIGHTNO] = SOIL_load_OGL_texture("tex/quitbN.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[TEXPAUSE] = SOIL_load_OGL_texture("tex/pauseb.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureCollectables[PIXIE] = SOIL_load_OGL_texture("tex/pix.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureCollectables[DEMON] = SOIL_load_OGL_texture("tex/littled.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureMenu = SOIL_load_OGL_texture("tex/menu.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (texturePlayer == 0) 
      printf("Erro do SOIL: '%s'\n", SOIL_last_result());
}

void init(){
    // Definir cor da borracha
    glClearColor(1,1,1,1);

    // Inicializa objetos
    initObj(objArray,MAX_COLLECTABLES,(double)MAP_BORDER);

    // Importa texturas
    importTextures();

    // Ativar blending
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void reshape_callback(int w,int h){
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();       // Reseta a matriz para seu estado inicial
    
    glOrtho(0, WIDTH, 0, HEIGHT, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
}

void enablePause(int)
{cam.gui->canPause=true;}

void stateMachine()
{
    switch(gameState)
    {
        case MENU:
        	if(keyState[' '])
			{
				gameState=GAME_0;
				reset(&p1,&cam,objArray,MAX_COLLECTABLES);
			}
        break;

        case QUIT:
        	cam.gui->quit=true;
        break;

        case DEAD:
        	cam.gui->restart=true;
        break;

        case PAUSE:
			
        break;
        
        case GAME_0:
            // Physics
                calculatePhysics(&p1,&cam,keyState,objArray,MAX_COLLECTABLES,MAP_BORDER,&gameState);
        break;
    }
}

void update_callback(int x)
{
    stateMachine();
    glutPostRedisplay();
	glutTimerFunc(1000/FPS,update_callback,0);
}

void key_press_callback(unsigned char key,int x,int y){ // x,y -> pos. mouse
    
   	if(key=='p' && gameState!=PAUSE && gameState!=MENU)
   	{
   		gameState = PAUSE;
		cam.gui->pause=true;
   	}
	else if(key=='p' && gameState==PAUSE)
	{
		gameState=GAME_0;
		cam.gui->pause=false;
	}


    if(gameState==MENU && key==27)
        exit(0);

    keyState[(int)key]=true;
}

void key_release_callback(unsigned char key,int x,int y){
    keyState[(int)key]=false;
}

void passive_mouse_callback(int x, int y){
	/*
		Lembrete ultra fcking importante: Coordenadas do mouse tem como origem o canto superior esquerdo da tela WTF
	*/
	if(gameState==DEAD)
	{
		if(x<cam.gui->restartX+cam.gui->restartSize/2 && x>cam.gui->restartX-cam.gui->restartSize/2
			&& y<cam.gui->restartY+cam.gui->restartSize/2 && y>cam.gui->restartY-cam.gui->restartSize/2)
		{
		   	cam.gui->textureRestart=RESTARTBRIGHT;

		   	// YES
		   	if(x<cam.gui->restartX && x>cam.gui->restartX-cam.gui->restartSize/2 && y<cam.gui->restartY+cam.gui->restartSize/2 && y>cam.gui->restartY)
				cam.gui->textureRestart=RESTARTBRIGHTYES;
			// NO
			else if(x<cam.gui->restartX+cam.gui->restartSize/2 && x>cam.gui->restartX  && y<cam.gui->restartY+cam.gui->restartSize/2 && y>cam.gui->restartY)
				cam.gui->textureRestart=RESTARTBRIGHTNO;
		}
		else
			cam.gui->textureRestart=RESTARTGREYED;
	}

	if(gameState==QUIT)
	{
		if(x<cam.gui->quitX+cam.gui->quitSize/2 && x>cam.gui->quitX-cam.gui->quitSize/2
			&& y<cam.gui->quitY+cam.gui->quitSize/2 && y>cam.gui->quitY-cam.gui->quitSize/2)
		{
		   	cam.gui->textureQuit=QUITBRIGHT;

		   	// YES
		   	if(x<cam.gui->quitX && x>cam.gui->quitX-cam.gui->quitSize/2 && y<cam.gui->quitY+cam.gui->quitSize/2 && y>cam.gui->quitY)
				cam.gui->textureQuit=QUITBRIGHTYES;
			// NO
			else if(x<cam.gui->quitX+cam.gui->quitSize/2 && x>cam.gui->quitX  && y<cam.gui->quitY+cam.gui->quitSize/2 && y>cam.gui->quitY)
				cam.gui->textureQuit=QUITBRIGHTNO;
		}
		else
			cam.gui->textureQuit=QUITGREYED;
	}
}

void mouse_callback(int button, int state, int x, int y)
{
	// Reconhece se mouse está em cima do botão do restart
	if(gameState == DEAD)
	{
		// YES
		   	if(x<cam.gui->restartX && x>cam.gui->restartX-cam.gui->restartSize/2 && y<cam.gui->restartY+cam.gui->restartSize/2 && y>cam.gui->restartY)
			{
				reset(&p1,&cam,objArray,MAX_COLLECTABLES);
				cam.gui->restart=false;
				gameState=GAME_0;
			}
		// NO
			else if(x<cam.gui->restartX+cam.gui->restartSize/2 && x>cam.gui->restartX  && y<cam.gui->restartY+cam.gui->restartSize/2 && y>cam.gui->restartY)
			{
				gameState=MENU;
			}
	}

	// Mouse no botão de sair
	if(gameState == QUIT)
	{
		// YES
		   	if(x<cam.gui->quitX && x>cam.gui->quitX-cam.gui->quitSize/2 && y<cam.gui->quitY+cam.gui->quitSize/2 && y>cam.gui->quitY)
			{
				exit(0);
			}
		// NO
			else if(x<cam.gui->quitX+cam.gui->quitSize/2 && x>cam.gui->quitX  && y<cam.gui->quitY+cam.gui->quitSize/2 && y>cam.gui->quitY)
			{
				cam.gui->quit=false;
				gameState=GAME_0;
			}
	}
}
void off_shade(bool canDraw)
{
	glColor4f(0,0,0,0.5);
	drawOverlay(WIDTH/2,HEIGHT/2,0,MAP_BORDER*2,canDraw,0);
	glColor4f(1,1,1,1);
}

void draw_callback(void){
    glClear(GL_COLOR_BUFFER_BIT);

    switch (gameState)
    {
    	case MENU:
    	// Desenha menu inicial
    		drawOverlay(WIDTH/2,HEIGHT/2,0,WIDTH,true,textureMenu);

    	break;

    	case QUIT:
    	case PAUSE:
    	case DEAD:
    	case GAME_0:
    		// Desenhar Background
		        glColor3f(1,1,1);
		        drawBg(WIDTH/2, HEIGHT/2,0, MAP_BORDER, &cam, true, textureBackground);

		    // Desenha Itens
		        glColor3f(1,1,1);
		        for(int x=0;x<MAX_COLLECTABLES;x++)
		        {
		        	if(dynamic_cast<Pixie*>(objArray[x])) // Semelhante a "instanceof" (pelo que entendi, tenta reduzir o objeto "genérico" ao tipo Pixie, se der bom, retorna true, se não, false)
		            	drawObject(objArray[x]->x, objArray[x]->y,0, objArray[x]->size, &cam, objArray[x]->isAlive, textureCollectables[PIXIE]);	
		        	if(dynamic_cast<Demon*>(objArray[x]))
		            	drawObject(objArray[x]->x, objArray[x]->y,0, objArray[x]->size, &cam, objArray[x]->isAlive, textureCollectables[DEMON]);
		        }

	  		// Desenhar Player e Espada
		        glColor3f(1, 1, 1);
		        drawPlayer(p1.localx,p1.localy,0,p1.size,0,texturePlayer,p1.frame,p1.total_frames,p1.frame_orientation);
		        drawSword(p1.localx,p1.localy,0,p1.sword->size,p1.sword->fixed_width,p1.sword->rotation,textureSword,1,1,1);
	        //drawSword((p1.sword->x+p1.sword->size)*cos(p1.sword->rotation*360/M_PI), (p1.sword->y+p1.sword->fixed_width)*sin(p1.sword->rotation*360/M_PI),0,p1.sword->size,p1.sword->fixed_width,p1.sword->rotation,0,1,1,1);

	  		// Desenha Menus por cima
		        off_shade(cam.gui->restart);
	        	drawOverlay(WIDTH/2,HEIGHT/2,0,cam.gui->restartSize,cam.gui->restart,textureGUI[cam.gui->textureRestart]); // Restart Button
	        	off_shade(cam.gui->quit);
	        	drawOverlay(WIDTH/2,HEIGHT/2,0,cam.gui->quitSize,cam.gui->quit,textureGUI[cam.gui->textureQuit]); // Quit Button
	        	off_shade(cam.gui->pause);
	        	drawOverlay(WIDTH/2,HEIGHT/2,0,cam.gui->pauseSize,cam.gui->pause,textureGUI[cam.gui->texturePause]);
	        	
		break;	
    }
    
    glutSwapBuffers();
}

int main(int argc, char** argv){

	// Configuracao inicial da SDL
		// Initialize SDL.
			if (SDL_Init(SDL_INIT_AUDIO) < 0)
				return -1;
		//Initialize SDL_mixer 
			if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ) 
				return -1;

		// Load our music
			music = Mix_LoadMUS(MUS_PATH);
			if (music == NULL)
				return -1;
		// Play Music
			if ( Mix_PlayMusic( music, -1) == -1 )
				return -1;

    
    // Configuracaoo inicial da janela do GLUT
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowSize(WIDTH, HEIGHT);
        glutInitWindowPosition(100, 100);

        glutCreateWindow("Jogo top");
        init();

    // Setar callbacks
        glutDisplayFunc(draw_callback);
        glutReshapeFunc(reshape_callback);
        glutKeyboardFunc(key_press_callback);
        glutKeyboardUpFunc(key_release_callback);
        glutMouseFunc(mouse_callback);
        glutPassiveMotionFunc(passive_mouse_callback);      // Callback do mouse (chamada quando ele se move)
    
    // Registra a funcao "atualiza" para executar daqui a 0 milissegundos
        glutTimerFunc(0, update_callback, 0);

    // Start Loop
        glutMainLoop();

    // clean up our resources
		Mix_FreeMusic(music);

	// quit SDL_mixer
		Mix_CloseAudio();
    
    return 0;
}