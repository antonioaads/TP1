#ifndef  DRAW_H // <- Prevents multiple inclusions
#define DRAW_H // <- ...

/**************************/
// your stuff goes here

// function prototypes, etc.
void animationControl(Player *);
void drawObject(double, double, double, double, Camera*, bool, GLuint);
void drawPlayer(double, double, double, double, double, double, GLuint, int, int, int);

// classes
	// -------

// reservar nomes

#endif // DRAW_H