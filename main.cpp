#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "player.h"
#include "draw.h"
#include "physics.h"
#include "objects.h"

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
#define MAP_LIMITS_X 2000
#define MAP_LIMITS_Y 2000
#define MAX_COLLECTABLES 30

// gameStates
    #define MENU -1
    #define GAME_0 0

using namespace std;

// Inicializar variavel
    int keyState[300];
    int gameState;

// Instanciar classes
    Player p1(WIDTH,HEIGHT);
    Collectable objArray[MAX_COLLECTABLES];
    Camera cam;

// Importar texturas
    GLuint texturePlayer;
    GLuint textureBackground;
    GLuint texturePix;

void importTextures()
{
    texturePlayer = SOIL_load_OGL_texture("tex/player.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    textureBackground = SOIL_load_OGL_texture("tex/background.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    texturePix = SOIL_load_OGL_texture("tex/pix.png",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);

    if (texturePlayer == 0) 
      printf("Erro do SOIL: '%s'\n", SOIL_last_result());
}

void init(){
    // Definir cor da borracha
    glClearColor(1,1,1,1);

    // Define estado inicial de jogo
    gameState=GAME_0;

    initObj(objArray,MAX_COLLECTABLES,(double)MAP_LIMITS_X,(double)MAP_LIMITS_Y);

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

void stateMachine(int gameState,int keyState[])
{
    switch(gameState)
    {
        case MENU:
        break;
        
        case GAME_0:
            // Physics
                calculatePhysics(&p1,&cam,keyState,objArray,MAX_COLLECTABLES);
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

    keyState[(int)key]=1;
}

void key_release_callback(unsigned char key,int x,int y){
    keyState[(int)key]=0;
}

void passive_mouse_callback(int x, int y){
    
}

void draw_callback(void){
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Desenhar Background
        glColor3f(1,1,1);
        drawObject(WIDTH/2, HEIGHT/2,0, MAP_LIMITS_X, &cam, true, textureBackground);

    // Desenha Itens
        glColor3f(1,1,1);
        for(int x=0;x<MAX_COLLECTABLES;x++)
            drawObject(objArray[x].x, objArray[x].y,0, objArray[x].size, &cam, objArray[x].isAlive, texturePix);

    // Desenhar Player
        glColor3f(1, 1, 1);
        drawPlayer(p1.localx,p1.localy,0,p1.size,texturePlayer);
    
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