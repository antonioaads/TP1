/*

	Secção de OBS.:
		- Lembrar de trocar colisão do quadrado para "metade inferior do quadrado", como sugere a textura (espada está apenas na metade inferior)
		- Lembrar de perguntar pro fessor se é má ideia guardar texutra em cada classe (talvez utilizar ponteiro ao invés de copiar a textura?)(problema do quadrado branco)
*/

#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>
#include <math.h>
#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "player.h"
#include "draw.h"
#include "physics.h"
#include "objects.h"

// Audio
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_mixer.h>

	#define MUS_PATH "music/CloZee - Koto.mp3" // OGG(e outros) = música de fundo
	#define SLASH_PATH "music/273477__n-audioman__fatalstrike.wav"
	#define INITWAR_PATH "music/ForHonorWarStartFINAL.wav"
	#define PICKUP_PATH "music/317789__jalastram__sfx-powerup-21.wav"
	#define AURA_PATH "music/399904__mrthenoronha__shining-8-bit.wav"
	#define CLICK_PATH "music/317785__jalastram__sfx-powerup-47.wav"
	#define WIN_PATH "music/jiraiya_theme_shortBOOST.wav"	//270333__littlerobotsoundfactory__jingle-win-00.wav

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#define MAP_BORDERX 8000	// Delimitação do mapa (usado para tamanho e colisões com borda)
#define MAP_BORDERY 4000
#define MAX_COLLECTABLES 65 // Caso aumente, mudar em physics.cpp também

// Estado do jogo
enum GAME_STATE{MENU=-1,HIGHSCORE_MENU,HELP_MENU,CREDITS_MENU,DEAD,PAUSE,QUIT,GAME_0};
// Conjunto de animações do player
enum PLAYER_ANIM{IDLE=0,WALK,BACKIDLE,BACKWALK};
// Textura de coletáveis
enum COLLECTABLE_TEXTURES{PIXIE=0,DEMON,MIKO,KITSUNE};
// Texturas do GUI
enum GUI_TEXTURES{RESTARTGREYED=0,RESTARTBRIGHT,RESTARTBRIGHTYES,RESTARTBRIGHTNO,QUITGREYED,QUITBRIGHT,QUITBRIGHTYES,QUITBRIGHTNO,TEXPAUSE,WOODPLATE,PORTRAIT,MOLDURA};
// "Sub-estado" do jogo quando no menu
enum MENU_SELECTION{START=0,HIGHSCORE,HELP,CREDITS,EXIT};
// Modo de controle da espada
enum SWORD_MODE{SWORD_KEY=0,SWORD_MOUSE};
// Efeitos sonoros do jogo
enum SFX{SLASH_SOUND=0,INITWAR_SOUND,PICKUP_SOUND,AURA_SOUND,CLICK_SOUND,WIN_SOUND};

using namespace std;

// Arquivo de musica (ponteiro)
	Mix_Music *music = NULL;
// Arquivo de SFX
	Mix_Chunk *sfx[10];

// Vetor de nomes dos 5 melhores jogadores
	string top_player_names[5];
	string name;

// Controle da callback para a glutKeyboardFunc quando em estado DEAD
	bool word_input=false;

// Vetor de pontuação dos 
	int top_player_scores[5];

// Inicializar variavel
    bool keyState[300];
    int gameState=MENU;
    int menuSelection=START;
    double splash_accel=1,splash_vel=0,splash_alpha=1;
    bool first_time_splash=true;

// Instanciar classes
    Player p1(WIDTH,HEIGHT);
    Collectable *objArray[MAX_COLLECTABLES];
    Camera cam((double)WIDTH,(double)HEIGHT);

// Importar texturas
    GLuint texturePlayer;
    GLuint texturePlayerAnim[4];
    GLuint textureSword;
    GLuint textureBackground;
    GLuint textureCollectables[10];
    GLuint textureAura;
    GLuint textureShadowblob;
    GLuint textureGUI[20];
    GLuint textureMenu[10];
    GLuint textureSplash;
    GLuint textureSideFace;
    GLuint textureHelp;
    GLuint textureCredits;
    GLuint textureBackButton;

void importTextures() 	// Faz o que o nome sugere =D
{
    texturePlayerAnim[IDLE] = SOIL_load_OGL_texture("tex/samuraiIDLE.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    texturePlayerAnim[WALK] = SOIL_load_OGL_texture("tex/samuraiWALK.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    texturePlayerAnim[BACKIDLE] = SOIL_load_OGL_texture("tex/samuraiBACKIDLE.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    texturePlayerAnim[BACKWALK] = SOIL_load_OGL_texture("tex/samuraiBACKWALK.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    texturePlayer = texturePlayerAnim[0];
    textureSword = SOIL_load_OGL_texture("tex/katana.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureBackground = SOIL_load_OGL_texture("tex/bgredish.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[RESTARTGREYED] = SOIL_load_OGL_texture("tex/restartg.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[RESTARTBRIGHT] = SOIL_load_OGL_texture("tex/restartb.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[RESTARTBRIGHTYES] = SOIL_load_OGL_texture("tex/restartbY.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[RESTARTBRIGHTNO] = SOIL_load_OGL_texture("tex/restartbN.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[QUITGREYED] = SOIL_load_OGL_texture("tex/exitg.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[QUITBRIGHT] = SOIL_load_OGL_texture("tex/exitb.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[QUITBRIGHTYES] = SOIL_load_OGL_texture("tex/exitbY.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[QUITBRIGHTNO] = SOIL_load_OGL_texture("tex/exitbN.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[TEXPAUSE] = SOIL_load_OGL_texture("tex/pauseb.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[WOODPLATE] = SOIL_load_OGL_texture("tex/wood.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[PORTRAIT] = SOIL_load_OGL_texture("tex/samuraiFACE.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureGUI[MOLDURA] = SOIL_load_OGL_texture("tex/moldura.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureCollectables[PIXIE] = SOIL_load_OGL_texture("tex/pix.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureCollectables[DEMON] = SOIL_load_OGL_texture("tex/demonWALK.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureCollectables[MIKO] = SOIL_load_OGL_texture("tex/mikospiritWALK.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureCollectables[KITSUNE] = SOIL_load_OGL_texture("tex/kitsune.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureAura = SOIL_load_OGL_texture("tex/fire.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureShadowblob = SOIL_load_OGL_texture("tex/shadow.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureMenu[START] = SOIL_load_OGL_texture("tex/menuSTART.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureMenu[HIGHSCORE] = SOIL_load_OGL_texture("tex/menuHIGHSCORE.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureMenu[HELP] = SOIL_load_OGL_texture("tex/menuHELP.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureMenu[CREDITS] = SOIL_load_OGL_texture("tex/menuCREDITS.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureMenu[EXIT] = SOIL_load_OGL_texture("tex/menuEXIT.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureSplash = SOIL_load_OGL_texture("tex/splash.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureSideFace = SOIL_load_OGL_texture("tex/sideface.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureHelp = SOIL_load_OGL_texture("tex/sideface.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureCredits = SOIL_load_OGL_texture("tex/credits.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureBackButton = SOIL_load_OGL_texture("tex/xbutton.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (texturePlayer == 0) 
      printf("Erro do SOIL: '%s'\n", SOIL_last_result());
}

void saveGame()
{
	// Salvar no arquivo
		ofstream arquivoP,arquivoN;
		arquivoP.open ("top_score.txt");
		arquivoN.open ("top_names.txt");
		// Salvar cada pontuação
		for(int x=0;x<5;x++)
		{
			arquivoP << top_player_scores[x] << endl;
			arquivoN << top_player_names[x].c_str() << endl;
			printf("SALVEI - %s\n",top_player_names[x].c_str());
		}
		arquivoP.close();
		arquivoN.close();
}

void exitFromGame()
{
	saveGame();
	exit(0);
}

void init(){
    // Definir cor da borracha
    glClearColor(1,1,1,1);

    // Inicializa objetos
    initObj(objArray,MAX_COLLECTABLES,(double)MAP_BORDERY);

    // Carregar Melhores Pontuações
    ifstream arquivoP,arquivoN;
    arquivoP.open("top_score.txt");
    arquivoN.open("top_names.txt");
    for(int x=0; x<5; x++)
    {
    	arquivoP >> top_player_scores[x];
    	arquivoN >> top_player_names[x];
    	printf("topplayernames-%d = %s\n", x, top_player_names[x]);
    }
    arquivoP.close();
    arquivoN.close();

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

void key_press_callback(unsigned char key,int x,int y){ // x,y -> pos. mouse
    
    switch(gameState)
    {
    	case HELP_MENU:
    	case CREDITS_MENU:
    	case HIGHSCORE_MENU:
			if(key=='x')
			{
				gameState=MENU;
				Mix_PlayChannel(3,sfx[CLICK_SOUND],0);
			}
		break;

    	case GAME_0:
	    	// Pause ingame
		   	if(key=='p' && gameState!=PAUSE && gameState!=MENU)
		   	{
		   		gameState = PAUSE;
				cam.gui->pause=true;
		   	}
		   	if(key=='x' && p1.sword->swordMode==SWORD_KEY)
		   		p1.sword->swordMode=SWORD_MOUSE;
		   	else if(key=='x')
		   		p1.sword->swordMode=SWORD_KEY;
		break;

		case PAUSE:
			if(key=='p' && gameState==PAUSE)
			{
				gameState=GAME_0;
				cam.gui->pause=false;
			}
		break;

		case MENU:
			// Selection menu
			if(key == 'n')
			{
				name = "";
				word_input = true;
			}
			if(key=='w')
			{
				menuSelection--;
				Mix_PlayChannel(3,sfx[CLICK_SOUND],0);
			}
			if(key=='s')
			{
				menuSelection++;
				Mix_PlayChannel(3,sfx[CLICK_SOUND],0);
			}
			if(menuSelection<0)
				menuSelection=EXIT;
			if(menuSelection>EXIT)
				menuSelection=START;
		break;
    }

	// Vetor de estados de teclas
		keyState[(int)key]=true;
}

void key_release_callback(unsigned char key,int x,int y){
    keyState[(int)key]=false;
}

void passive_mouse_callback(int x, int y){
	/*
		Lembrete ultra fcking importante: Coordenadas do mouse tem como origem o canto superior esquerdo da tela WTF
	*/

	y=HEIGHT-y; // transladar/inverter origem do mouse  ||  daqui em diante... lembrete não mais tão necessário assim

	if(p1.sword->swordMode==SWORD_MOUSE && x-p1.x+y-p1.y!=0)	// Caso o modo de controle da espada seja MOUSE e deltaX + deltaY seja diferente de zero (por causa da divisão)
	{
		double senteta=(y-p1.y)/sqrt(pow(x-p1.x,2)+pow(y-p1.y,2));
		double costeta=(x-p1.x)/sqrt(pow(x-p1.x,2)+pow(y-p1.y,2));
		double teta=85-atan2(x-WIDTH/2,y-HEIGHT/2)*180/M_PI;

		p1.sword->rotation=teta;
	}

	if(gameState==DEAD)
	{
		if(x<cam.gui->restartX+cam.gui->restartSize/2 && x>cam.gui->restartX-cam.gui->restartSize/2
			&& y<cam.gui->restartY+cam.gui->restartSize/2 && y>cam.gui->restartY-cam.gui->restartSize/2)
		{
		   	cam.gui->textureRestart=RESTARTBRIGHT;

		   	// YES
		   	if(x<cam.gui->restartX && x>cam.gui->restartX-cam.gui->restartSize/2 && y>cam.gui->restartY-cam.gui->restartSize/2 && y<cam.gui->restartY)
				cam.gui->textureRestart=RESTARTBRIGHTYES;
			// NO
			else if(x<cam.gui->restartX+cam.gui->restartSize/2 && x>cam.gui->restartX  && y>cam.gui->restartY-cam.gui->restartSize/2 && y<cam.gui->restartY)
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
		   	if(x<cam.gui->quitX && x>cam.gui->quitX-cam.gui->quitSize/2 && y>cam.gui->quitY-cam.gui->quitSize/2 && y<cam.gui->quitY)
				cam.gui->textureQuit=QUITBRIGHTYES;
			// NO
			else if(x<cam.gui->quitX+cam.gui->quitSize/2 && x>cam.gui->quitX  && y>cam.gui->quitY-cam.gui->quitSize/2 && y<cam.gui->quitY)
				cam.gui->textureQuit=QUITBRIGHTNO;
		}
		else
			cam.gui->textureQuit=QUITGREYED;
	}
}

void mouse_callback(int button, int state, int x, int y)
{
	// Reconhece se mouse está em cima do botão do restart
	if(gameState == DEAD && !word_input)
	{
		// YES
		   	if(x<cam.gui->restartX && x>cam.gui->restartX-cam.gui->restartSize/2 && y<cam.gui->restartY+cam.gui->restartSize/2 && y>cam.gui->restartY)
			{
				reset(&p1,&cam,objArray);
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
				saveGame();
				gameState = MENU;
			}
		// NO
			else if(x<cam.gui->quitX+cam.gui->quitSize/2 && x>cam.gui->quitX  && y<cam.gui->quitY+cam.gui->quitSize/2 && y>cam.gui->quitY)
			{
				cam.gui->quit=false;
				gameState=GAME_0;
			}
	}
}

void key_input_name(unsigned char key, int x,int y)
{
	if(key != 13 && key !=  8)
		name += key;
	else if(key == 8)
		name.pop_back();
	else
		word_input=false;
}

void stateMachine()
{
	cout << name << endl;

	if(first_time_splash)
	{
		splash_vel+=1/(10000-splash_accel*splash_accel);
		splash_alpha-=splash_vel;
		if(splash_alpha<=0)
		{
			splash_alpha=0;
			first_time_splash=false;
		}
    }

    // Retornar com a musica
		if(!Mix_Playing(2))
			Mix_ResumeMusic();

	// Define input como controle ou palavras
	if(word_input)
		glutKeyboardFunc(key_input_name);
	else
		glutKeyboardFunc(key_press_callback);  

    switch(gameState)
    {
        case MENU:
      	  	if(keyState[' '])
			{
				Mix_PlayChannel(3,sfx[CLICK_SOUND],0);
				switch(menuSelection)
				{
					case START:
						Mix_PauseMusic();
						Mix_PlayChannel(2, sfx[INITWAR_SOUND], 0);
						gameState=GAME_0;
						reset(&p1,&cam,objArray);
					break;

					case HIGHSCORE:
						gameState=HIGHSCORE_MENU;
					break;

					case HELP:
						gameState=HELP_MENU;
					break;

					case CREDITS:
						gameState=CREDITS_MENU;
					break;

					case EXIT:
						exitFromGame();
					break;

				}
			}
        break;

        case QUIT:
        	cam.gui->quit=true;
        break;

        case DEAD:
        	if(p1.canSave)
        	{
        		// Garantir que esteja organizado o vetor de pontuação em ordem crescente
        		printf("Fadd o nome: %s\n",name.c_str());

        		for(int x=0;x<5;x++)
				{
					for(int y=0;y<4;y++)
					{
						if(top_player_scores[x]<top_player_scores[x+1])
						{
							int a=top_player_scores[x];
							string b=top_player_names[x];

							top_player_scores[x]=top_player_scores[x+1];
							top_player_names[x]=top_player_names[x+1];

							top_player_scores[x+1]=a;
							top_player_names[x+1]=b;
						}
					}
				}

				printf("Gadd o nome: %s\n",name.c_str());

				// Inserir no vetor garantindo organização
        		for(int x=0; x<=5;x++)
        		{
        			if(p1.points > top_player_scores[x])
        			{
        				// Arredar para a esquerda todos os nomes/pontuações
        				for(int y=5;y>x;y--)
        				{
        					top_player_scores[y]=top_player_scores[y-1];
        					// s.push_back(top_player_names[y-1]);
        					// top_player_names[y]=s;
        					printf("topname-%d = %s\n", y, top_player_names[y] );
        					printf("topname-%d = %s\n", y-1, top_player_names[y-1] );
        					top_player_names[y] = top_player_names[y-1];
        					printf("topname-%d = %s\n", y, top_player_names[y] );
        					printf("topname-%d = %s\n", y-1, top_player_names[y-1] );
        				}
        				printf("Madd o nome: %s\n",name.c_str());
        				top_player_scores[x] = p1.points;
        				top_player_names[x] = name;
        				printf("add o nome: %s\n",name.c_str());
        				break;
        			}
        		}

				p1.canSave=false;
        	}
        	cam.gui->restart=true;     
        break;

        case PAUSE:
			
        break;
        
        case GAME_0:
            // Physics
                calculatePhysics(&p1, &cam, keyState, objArray, &gameState, sfx, word_input);
            // Animation Handling
                // Player
                	// Selecionar qual tipo de movimentação do player
	                if(keyState['s'] || keyState['d'] || keyState['a'])
	                {
	                	texturePlayer=texturePlayerAnim[WALK];
	                	p1.total_frames=3;
	                }
	                if(keyState['w'])
	                {
	                	texturePlayer=texturePlayerAnim[BACKWALK];
	                	p1.total_frames=3;
	                }
	                if(!(keyState['s'] || keyState['d'] || keyState['a'] || keyState['w']))
	                {
	                	texturePlayer=texturePlayerAnim[IDLE];
	                	p1.total_frames=2;
	                }
	                // Implementar troca de frames com delay (para não acontecer troca de frame a cada 1/FPS segundos)
	                if(p1.frame_delay>=10)
	                {
	                	if(p1.frame<p1.total_frames-1)
	                		p1.frame++;
	                	else
	                		p1.frame=1;

	                	p1.frame_delay=0;
	                }
	        		p1.frame_delay++;

        		// Aura
        		if(p1.blessframe_delay>=10)
                {
                	if(p1.blessframe<p1.blesstotal_frames-1)
                		p1.blessframe++;
                	else
                		p1.blessframe=1;

                	p1.blessframe_delay=0;
                }
        		p1.blessframe_delay++;

        		// Portrait
        		if(cam.gui->portraitframe_delay >= 10)
                {
                	if(cam.gui->portraitframe < cam.gui->portraittotal_frames-1)
                		cam.gui->portraitframe++;
                	else
                		cam.gui->portraitframe=1;

                	cam.gui->portraitframe_delay=0;
                }
        		cam.gui->portraitframe_delay++;

        		// Enemies
        		for(int x=0; x<MAX_COLLECTABLES;x++)
        		{
		    		if(objArray[x]->frame_delay>=10)
		            {
		            	if(objArray[x]->frame<objArray[x]->total_frames-1)
		            		objArray[x]->frame++;
		            	else
		            		objArray[x]->frame=1;

		            	objArray[x]->frame_delay=0;
		            }
		    		objArray[x]->frame_delay++;
		    	}

        break;
    }
}

void update_callback(int x)
{
    stateMachine();
    glutPostRedisplay();
	glutTimerFunc(1000/FPS,update_callback,0);
}

void off_shade(bool canDraw) // Efeito de "escurecer a tela"
{
	glColor4f(0,0,0,0.5);
	drawOverlay(WIDTH/2,HEIGHT/2,0,MAP_BORDERX*2,MAP_BORDERX*2,canDraw,0);
	glColor4f(1,1,1,1);
}

void drawText(void* font, string str, double x, double y)
{
	glRasterPos2d(x,y);

	for(int i=0;i<str.size();i++)
		glutBitmapCharacter(font,str[i]);
}

void draw_callback(void){
    glClear(GL_COLOR_BUFFER_BIT);

    switch (gameState)
    {
    	case HIGHSCORE_MENU:
		{
			// Fundo
			glColor4d(1,1,1,1);
			drawOverlay(WIDTH/2,HEIGHT/2,0,WIDTH,HEIGHT,true,textureSideFace);
			// Desenhar madeira
			glColor4d(1,1,1,0.6);
			drawOverlay(WIDTH/2,HEIGHT/2,0,WIDTH,HEIGHT*4,true,textureGUI[WOODPLATE]);
			// Desenhar botão
			glColor4d(0.5,1,1,1);
			drawOverlay(150,85,0,400,350,true,textureGUI[WOODPLATE]);
			glColor4d(1,1,1,1);
			drawOverlay(150,80,0,200,100,true,textureBackButton);
			// Escrever pontuação
			glColor4d(1,1,1,1);
			char str[100];
	        sprintf(str,"Top Score: ");
	        drawText(GLUT_BITMAP_HELVETICA_18,str,WIDTH/2-100,3.8*HEIGHT/5);
	        for(int x=0;x<5;x++)
	        {
	        	sprintf(str,"%d - %s \t\t\t\t\t\t\t\t\t\t\t\t\t %d",x,top_player_names[x].c_str(),top_player_scores[x]);
	        	drawText(GLUT_BITMAP_HELVETICA_18,str,300,500-x*100);	
			}
		}
	    break;

	    case HELP_MENU:
	    	// Fundo
	    	glColor4d(1,1,1,1);
	    	drawOverlay(WIDTH/2,HEIGHT/2,0,WIDTH,HEIGHT,true,textureHelp);
	    break;

	    case CREDITS_MENU:
	    	// Fundo
	    	glColor4d(1,1,1,1);
	    	drawOverlay(WIDTH/2,HEIGHT/2,0,WIDTH,HEIGHT,true,textureCredits);
	    	drawOverlay(150,80,0,200,100,true,textureBackButton);
	    	// Texto
	    	char str[10000];
	        sprintf(str,"Art by Rodrigo D Moreira");
	        drawText(GLUT_BITMAP_HELVETICA_18,str,200,3.8*HEIGHT/5-100);
	        sprintf(str,"Programming by Antonio D Sousa and Rodrigo D Moreira");
	        drawText(GLUT_BITMAP_HELVETICA_18,str,200,3.8*HEIGHT/5-150);
	        sprintf(str,"Music:");
	        drawText(GLUT_BITMAP_HELVETICA_18,str,200,3.8*HEIGHT/5-200);
	        sprintf(str,"                 Koto by CloZee");
	        drawText(GLUT_BITMAP_HELVETICA_18,str,200,3.8*HEIGHT/5-250);
	        sprintf(str,"                 SFX by Jalastram, N-Audioman, MrTheNoronha");
	        drawText(GLUT_BITMAP_HELVETICA_18,str,200,3.8*HEIGHT/5-300);
	        sprintf(str,"                 Entrance SFX by Ubisoft(For Honor team)");
	        drawText(GLUT_BITMAP_HELVETICA_18,str,200,3.8*HEIGHT/5-350);
	        sprintf(str,"                 Jiraya Theme from Naruto");
	        drawText(GLUT_BITMAP_HELVETICA_18,str,200,3.8*HEIGHT/5-400);
	    break;

    	case MENU:
    		// Desenha menu inicial
    		glColor4f(1,1,1,1);
    		drawOverlay(WIDTH/2,HEIGHT/2,0,WIDTH*1.5,HEIGHT,true,textureMenu[menuSelection]);

    		if(word_input)
    		{
    			glColor4f(1,1,1,1);
    			drawText(GLUT_BITMAP_HELVETICA_18,name.c_str(),WIDTH/2+300,HEIGHT/2);
    		}
    		// Desenhar splash screen
    		if(first_time_splash)
	    	{
	    		glColor4f(1,1,1,splash_alpha);
	    		drawOverlay(WIDTH/2,HEIGHT/2,0,WIDTH,HEIGHT,true,textureSplash);
    		}
    	break;

    	case QUIT:	// Como quero que fique desenhado por baixo o último estado do jogo
    	case PAUSE:	// junto todos os cases em um único
    	case DEAD:
    	case GAME_0:
    	{
    		// Desenhar Background
		        glColor3f(1,1,1);
		        drawBg(0,0,0, MAP_BORDERX,MAP_BORDERY, &cam, true, textureBackground);

		    // Desenha Itens / Inimigos
		        glColor3f(1,1,1);
		        for(int x=0;x<MAX_COLLECTABLES;x++)
		        {
		        	// Sombra
		        		glColor4f(1,1,1,0.5);
		        		drawObject(objArray[x]->x, (objArray[x]->y-objArray[x]->sizey/2), 0, 50,50, &cam, true, textureShadowblob,1,1,1);
		        		glColor4f(1,1,1,1);
		        	if(dynamic_cast<Pixie*>(objArray[x])) // Semelhante a "instanceof" (pelo que entendi, tenta reduzir o objeto "genérico" ao tipo Pixie, se der bom, retorna true, se não, false)
		            	drawObject(objArray[x]->x, objArray[x]->y,0, objArray[x]->sizex, objArray[x]->sizey, &cam, objArray[x]->isAlive, textureCollectables[PIXIE],objArray[x]->frame,objArray[x]->total_frames,objArray[x]->frame_orientation);	
		        	if(dynamic_cast<Demon*>(objArray[x]))
		            	drawObject(objArray[x]->x, objArray[x]->y,0, objArray[x]->sizex, objArray[x]->sizey, &cam, objArray[x]->isAlive, textureCollectables[DEMON],objArray[x]->frame,objArray[x]->total_frames,objArray[x]->frame_orientation);
		            if(dynamic_cast<Miko*>(objArray[x]))
		            	drawObject(objArray[x]->x, objArray[x]->y,0, objArray[x]->sizex, objArray[x]->sizey, &cam, objArray[x]->isAlive, textureCollectables[MIKO],objArray[x]->frame,objArray[x]->total_frames,objArray[x]->frame_orientation);
		            if(dynamic_cast<Kitsune*>(objArray[x]))
		            	drawObject(objArray[x]->x, objArray[x]->y,0, objArray[x]->sizex, objArray[x]->sizey, &cam, objArray[x]->isAlive, textureCollectables[KITSUNE],objArray[x]->frame,objArray[x]->total_frames,objArray[x]->frame_orientation);
		        }

	  		// Desenhar Player
		        // Sombra
	        		glColor4f(1,1,1,0.5);
	        		drawWithinCamera(p1.localx, (p1.localy-p1.sizex/2-10), 0, 50, 50, 0, textureShadowblob,1,1,1);
	        		glColor4f(1,1,1,1);
	        	// Player e Espada
			        glColor4f(1,1,1,1);
			        drawWithinCamera(p1.localx,p1.localy,0,p1.sizex,p1.sizey,0,texturePlayer,p1.frame,p1.total_frames,p1.frame_orientation);
			        drawSword(p1.localx,p1.localy,0,p1.sword->size,p1.sword->fixed_width,p1.sword->rotation,textureSword,1,1,1);
		        // Aura (caso ativa)
			        glColor4f(1,1,1,0.7);
			        drawObject(p1.x,p1.y+30,0,p1.sizex+100,p1.sizey+100,&cam,p1.bless,textureAura,p1.blessframe,p1.blesstotal_frames,p1.frame_orientation);

			// FX

			// Possível efeito maneiro
	        	// drawObject(rotationConvert(p1.sword->size,0,p1.sword->rotation,'x')+p1.x, rotationConvert(p1.sword->size,0,p1.sword->rotation,'y')+p1.y, 0, 100,&cam,true, 0);


	  		// Desenha Menus por cima
		        // Moldura dentro do jogo
			        glColor4f(1,1,1,0.5);
			        drawOverlay(WIDTH/2,HEIGHT/2,0,WIDTH,HEIGHT,true,textureGUI[MOLDURA]);
		        // Madeira atrás da pontuação
			        glColor4f(1,1,1,1);
			        drawOverlay(265,80,0,250,250,true,textureGUI[WOODPLATE]);
		        // Portrait no canto esquerdo inferior
		        	drawWithinCamera(cam.gui->portraitX,cam.gui->portraitY,0,cam.gui->portraitSize,cam.gui->portraitSize,0,textureGUI[PORTRAIT],cam.gui->portraitframe,cam.gui->portraittotal_frames,1);
		        // Pontuação e nível da espada
			        char str[100];
			        sprintf(str,"Sword lv: %.0f",(p1.sword->size-100)/4);
			        drawText(GLUT_BITMAP_HELVETICA_18,str,210,80);
			        sprintf(str,"    Points: %d",p1.points);
			        drawText(GLUT_BITMAP_HELVETICA_18,str,210,60);
		        // Efeito de escurecer + restart/pause/quit
			        off_shade(cam.gui->restart);
		        	drawOverlay(WIDTH/2,HEIGHT/2,0,cam.gui->restartSize,cam.gui->restartSize,cam.gui->restart,textureGUI[cam.gui->textureRestart]); // Restart Button
		        	off_shade(cam.gui->quit);
		        	drawOverlay(WIDTH/2,HEIGHT/2,0,cam.gui->quitSize,cam.gui->quitSize,cam.gui->quit,textureGUI[cam.gui->textureQuit]); // Quit Button
		        	off_shade(cam.gui->pause);
		        	drawOverlay(WIDTH/2,HEIGHT/2,0,cam.gui->pauseSize,cam.gui->pauseSize,cam.gui->pause,textureGUI[cam.gui->texturePause]);
		        // Desenhar Pontuação no restart
		        	if(gameState==DEAD)
		        	{
		        		glColor4f(0.8,0,0.2,1);
			        	sprintf(str,"Total Points: %d",p1.points);
				        drawText(GLUT_BITMAP_HELVETICA_18,str,WIDTH/2-50,HEIGHT/2+120);
				    }
	    }
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

		// Load our sound effect
			sfx[SLASH_SOUND] = Mix_LoadWAV(SLASH_PATH);
			sfx[INITWAR_SOUND] = Mix_LoadWAV(INITWAR_PATH);
			sfx[PICKUP_SOUND] = Mix_LoadWAV(PICKUP_PATH);
			sfx[AURA_SOUND] = Mix_LoadWAV(AURA_PATH);
			sfx[CLICK_SOUND] = Mix_LoadWAV(CLICK_PATH);
			sfx[WIN_SOUND] = Mix_LoadWAV(WIN_PATH);
			//if (sfx[SLASH_SOUND] == NULL || sfx[INITWAR_SOUND] == NULL || sfx[PICKUP_SOUND] == NULL || sfx[AURA_SOUND])
				//return -1;

		// Load our music
			music = Mix_LoadMUS(MUS_PATH);
			if (music == NULL)
				return -1;
		// Play Music
			if ( Mix_PlayMusic( music, -1) == -1 ) // (-1) -> loop  (1) -> one_time
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
        Mix_FreeChunk(sfx[SLASH_SOUND]);
        Mix_FreeChunk(sfx[INITWAR_SOUND]);
        Mix_FreeChunk(sfx[PICKUP_SOUND]);
        Mix_FreeChunk(sfx[AURA_SOUND]);
        Mix_FreeChunk(sfx[CLICK_SOUND]);
        Mix_FreeChunk(sfx[WIN_SOUND]);
		Mix_FreeMusic(music);

	// quit SDL_mixer
		Mix_CloseAudio();

    
    return 0;
}