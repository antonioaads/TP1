#ifndef  DRAW_H // <- Prevents multiple inclusions
#define DRAW_H // <- ...

/**************************/
// your stuff goes here

// function prototypes, etc.
void animationControl(Player *);
void drawBg(double, double, double, double, Camera*, bool, GLuint);
void drawObject(double, double, double, double, Camera*, bool, GLuint);
void drawPlayer(double, double, double, double, double, GLuint, int, int, int);
void drawSword(double, double, double, double, double, double, GLuint, int, int, int);
void drawOverlay(double,double,double,double,bool,GLuint);

// classes
	// -------

// reservar nomes

#endif // DRAW_H