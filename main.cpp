#include <SOIL/SOIL.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "global.h"
#include "player.h"
#include "draw.h"
#include "physics.h"
#include "objects.h"

using namespace std;

// Inicializar variavel
    int keyState[300];
    int gameState;

// Instanciar classes
    Player p1(WIDTH,HEIGHT);
    //Collectable objArray[1000];
    Camera cam;

// Importar texturas
    GLuint texturePlayer;
    GLuint textureBackground;

void stateMachine(int gameState,int keyState[])
{
    switch(gameState)
    {
        case MENU:
        break;
        
        case GAME_0:
            // Physics
                calculatePhysics(&p1,&cam,keyState);
        break;
    }
}

void init(){
    // Definir cor da borracha
    glClearColor(1,1,1,1);

    // Define estado inicial de jogo
    gameState=GAME_0;

    // Importa texturas
    importTextures(texturePlayer,textureBackground);

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
        drawObject(WIDTH/2,HEIGHT/2,0,MAP_LIMITS_X,&cam,textureBackground);

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