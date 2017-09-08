#ifndef  DRAW_H // <- Prevents multiple inclusions
#define DRAW_H // <- ...

/**************************/
// your stuff goes here

// function prototypes, etc.
void animationControl(Player *);
void drawBg(double, double, double, double, double, Camera*, bool, GLuint);
void drawObject(double x, double y, double z, double size,Camera *cam, bool canDraw, GLuint texture,int frame,int total_frames,int frame_orientation);
void drawPlayer(double, double, double, double, double, double, GLuint, int, int, int);
void drawSword(double, double, double, double, double, double, GLuint, int, int, int);
void drawOverlay(double,double ,double,double,double,bool,GLuint);
double rotationConvert(double xr,double yr,double degree, char coord);

// classes
	// -------

// reservar nomes

#endif // DRAW_H