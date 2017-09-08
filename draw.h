#ifndef  DRAW_H // <- Prevents multiple inclusions
#define DRAW_H // <- ...

/**************************/
// your stuff goes here

// function prototypes, etc.
void animationControl(Player *);
void drawBg(double x, double y, double z, double sizex,double sizey,Camera *cam, bool canDraw, GLuint texture);
void drawObject(double x, double y, double z, double size,Camera *cam, bool canDraw, GLuint texture,int frame,int total_frames,int frame_orientation);
void drawOnScreen(double x, double y, double z, double sizex,double sizey, double degree, GLuint texture,int frame,int total_frames,int frame_orientation);
void drawSword(double x, double y, double z, double size, double fixed_width,double degree, GLuint texture,int frame,int total_frames,int frame_orientation);
void drawOverlay(double x, double y, double z, double sizex,double sizey, bool canDraw, GLuint texture);
double rotationConvert(double xr,double yr,double degree, char coord);

// classes
	// -------

// reservar nomes

#endif // DRAW_H