#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#include "player.h"
#include "draw.h"
#include "physics.h"
#include "objects.h"

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#define MAP_BORDER 3000
#define MAX_COLLECTABLES 30

// gameStates
enum GAME_STATE{MENU=-1,DEAD,GAME_0};

// Collectable textures
enum TEXTURE_TYPES{PIXIE=0,DEMON};

using namespace std;

// Inicializar variavel
    bool keyState[300];
    int gameState;

// Instanciar classes
    Player p1(WIDTH,HEIGHT);
    Collectable *objArray[MAX_COLLECTABLES];
    Camera cam;

// Importar texturas
    GLuint texturePlayer;
    GLuint textureSword;
    GLuint textureBackground;
    GLuint textureCollectables[10];

void importTextures()
{
    texturePlayer = SOIL_load_OGL_texture("tex/player.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureSword = SOIL_load_OGL_texture("tex/sword.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureBackground = SOIL_load_OGL_texture("tex/background.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureCollectables[PIXIE] = SOIL_load_OGL_texture("tex/pix.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureCollectables[DEMON] = SOIL_load_OGL_texture("tex/littled.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (texturePlayer == 0) 
      printf("Erro do SOIL: '%s'\n", SOIL_last_result());
}

void init(){
    // Definir cor da borracha
    glClearColor(1,1,1,1);

    // Define estado inicial de jogo
    gameState=GAME_0;

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

void stateMachine(int gameState,bool keyState[])
{
    switch(gameState)
    {
        case MENU:
        break;

        case DEAD:
        break;
        
        case GAME_0:
            // Physics
                calculatePhysics(&p1,&cam,keyState,objArray,MAX_COLLECTABLES,MAP_BORDER);
        break;
    }
}

void update_callback(int x)
{
    stateMachine(gameState,keyState);
    
    glutPostRedisplay();

    glutTimerFunc(1000/FPS,update_callback,0);
}

void key_press_callback(unsigned char key,int x,int y){ // x,y -> pos. mouse
    if(key==27)
        exit(0);

    keyState[(int)key]=true;
}

void key_release_callback(unsigned char key,int x,int y){
    keyState[(int)key]=false;
}

void passive_mouse_callback(int x, int y){
    
}

void draw_callback(void){
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Desenhar Background
        glColor3f(1,1,1);
        drawObject(WIDTH/2, HEIGHT/2,0, MAP_BORDER, &cam, true, textureBackground);

    // Desenha Itens
        glColor3f(1,1,1);
        for(int x=0;x<MAX_COLLECTABLES;x++)
        {
        	if(dynamic_cast<Pixie*>(objArray[x])) // Semelhante a "instanceof"
            	drawObject(objArray[x]->x, objArray[x]->y,0, objArray[x]->size, &cam, objArray[x]->isAlive, textureCollectables[PIXIE]);	
        	if(dynamic_cast<Demon*>(objArray[x]))
            	drawObject(objArray[x]->x, objArray[x]->y,0, objArray[x]->size, &cam, objArray[x]->isAlive, textureCollectables[DEMON]);
        }

    // Desenhar Player e Espada
        glColor3f(1, 1, 1);
        drawPlayer(p1.localx,p1.localy,0,p1.size,0,0,texturePlayer,p1.frame,p1.total_frames,p1.frame_orientation);
        drawPlayer(p1.localx+p1.sword->xoffset,p1.localy+p1.sword->yoffset,0,p1.sword->size,p1.sword->fixed_width,p1.sword->rotation,textureSword,1,1,1);
    
    glutSwapBuffers();
}

int main(int argc, char** argv){
    
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
        // Callbacks presentes em input.cpp
        glutKeyboardFunc(key_press_callback);
        glutKeyboardUpFunc(key_release_callback);
        glutPassiveMotionFunc(passive_mouse_callback);      // Callback do mouse (chamada quando ele se move)
    
    // Registra a funcao "atualiza" para executar daqui a 0 milissegundos
        glutTimerFunc(0, update_callback, 0);

    // Start Loop
        glutMainLoop();
    
    return 0;
}