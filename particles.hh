#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <time.h>

#include <math.h>

#define PI 3.14159265

enum e_particle
{
  explosion,
  circle,
  nova
};

typedef struct
{
  int r, g, b;
  GLfloat dist;
  GLfloat angle;
} stars;

/* Image type - contains height, width, and data */
typedef struct
{
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
} Image;

// FIXME: think about better arguments.
class Particle
{
  public:
    Particle(/*Image img,*/ int r, int g, int b, float x, float y, float z, float angle);
    void resetParticle ();
    float angle () { return angle_; }
    float x () { return x_; }
    float y () { return y_; }
    float z () { return z_; }
    float vx () { return vx_; }
    float vy () { return vy_; }
    float vz () { return vz_; }
    int r () { return r_; }
    int g () { return g_; }
    int b () { return b_; }
  //private:
    int r_, g_, b_;
    float x_, y_, z_;
    float vx_, vy_, vz_;
    float angle_;
    float lifeRemaining_;
    float life_;
    bool isAlive_;
   // Image img_;
};

// FIXME: we have to make it a GlobalVariable to be used by OpenGL func.
class ParticleEngine
{
  public:
    ParticleEngine(int nbPart, e_particle = nova);
    int nbPart () { return nbPart_; }
    e_particle type () { return type_; }
    int vpart (int index, Particle* val) { vpart_[index] = val; }
    std::vector<Particle*> vpart () { return vpart_; }

    // FIXME: put it in private.
    double t_;
  private:
    std::vector<Particle*> vpart_;
    int nbPart_;
    e_particle type_;
};
