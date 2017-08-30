#include <GL/glew.h>
#include <GL/freeglut.h>

#define WIDTH 1024
#define HEIGHT 768
#define FPS 60

int keyState[300];

typedef struct Player{
    int x,y;
    int size;
}Player;

typedef struct Camera
{
    int x, y;
    int size;
}Camera;

Player p1;
Camera cam;

void init(){
    // Definir cor do 
    glClearColor(1,1,1,1);
    
    p1.x=(float)WIDTH/2;
    p1.y=(float)HEIGHT/2;
    p1.size=100;
}

void draw_callback(void){
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Desenhar Background
    glPushMatrix();
    
        glLoadIdentity();
        glTranslatef(-cam.x,-cam.y, 0);
    
        glBegin(GL_TRIANGLE_FAN);
            glColor3f(0,0,1); glVertex3f(-500,-500,0);
            glColor3f(0,1,0); glVertex3f(WIDTH+500,-500,0);
            glColor3f(1,0,0); glVertex3f(WIDTH+500,HEIGHT+500,0);
            glColor3f(0,0,0); glVertex3f(-500,HEIGHT+500,0);
        glEnd();
    
    glPopMatrix();
    
    // Desenhar Player
        
    glColor3f(0, 1, 0); // verde
    
    glBegin(GL_TRIANGLE_FAN);
        glVertex3f(p1.x,p1.y,0);
        glVertex3f(p1.x+p1.size,p1.y,0);
        glVertex3f(p1.x+p1.size,p1.y+p1.size,0);
        glVertex3f(p1.x,p1.y+p1.size,0);
    glEnd();
    
    glutSwapBuffers();
}

void reshape_callback(int w,int h){
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();       // Reseta a matriz para seu estado inicial
    
    glOrtho(0, WIDTH, 0, HEIGHT, -1.0, 1.0);
    
    glMatrixMode(GL_MODELVIEW);
}

void key_press_callback(unsigned char key,int x,int y){ // x,y -> pos. mouse
    switch(key)
   {
      // Tecla ESC
        case 27:
            exit(0);
        break;
        
        case 'w':
            keyState[(int)('w')]=1;
        break;
        
        case 'a':
            keyState[(int)('a')]=1;
        break;
        
        case 's':
            keyState[(int)('s')]=1;
        break;
        
        case 'd':
            keyState[(int)('d')]=1;
        break;
   }
}

void key_release_callback(unsigned char key,int x,int y){
    switch(key)
   {
      // Tecla ESC
        case 'w':
            keyState[(int)('w')]=0;
        break;
        
        case 'a':
            keyState[(int)('a')]=0;
        break;
        
        case 's':
            keyState[(int)('s')]=0;
        break;
        
        case 'd':
            keyState[(int)('d')]=0;
        break;
   }
}

void mouse_callback(int x, int y){
    
}

void update_callback(int x)
{
    
    // Physics
    if(keyState[(int)('w')]==1)
        cam.y+=2;
    if(keyState[(int)('a')]==1)
        cam.x-=2;
    if(keyState[(int)('d')]==1)
        cam.x+=2;
    if(keyState[(int)('s')]==1)
        cam.y-=2;
    
    glutTimerFunc(1000/FPS,update_callback,0);
    glutPostRedisplay();
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
    glutKeyboardFunc(key_press_callback);
    glutKeyboardUpFunc(key_release_callback);
    glutPassiveMotionFunc(mouse_callback);      // Callback do mouse (chamada quando ele se move)
    
    // Registra a funcao "atualiza" para executar daqui a 0 milissegundos
    glutTimerFunc(0, update_callback, 0);

    // Start Loop
    glutMainLoop();
    
    return 0;
}