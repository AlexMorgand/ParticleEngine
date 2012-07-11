#ifndef TOOLS_HH
# define TOOLS_HH

# include <GL/glut.h>
# include <GL/gl.h>
# include <GL/glu.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <iostream>
# include <vector>
# include <time.h>
# include "particles.hh"
# include <math.h>
# include "camera.hh"

# define GRAVITY 2

typedef struct
{
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
} Image;

extern GLuint texture[5];
extern GLfloat tilt;    // tilt the view
extern GLfloat spin;            // spin twinkling stars
extern float elapsedTime;

int ImageLoad(std::string filename, Image *image);
unsigned int getint(FILE* fp);
unsigned int getshort(FILE* fp);
GLvoid LoadGLTextures();
void initParticles ();
GLvoid InitGL(GLsizei width, GLsizei height);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
GLvoid DrawGLScene();
void input (unsigned char key, int x, int y);
void mouse (int x, int y);

#endif
