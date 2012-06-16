#include "particles.hh"

ParticleEngine* pe = 0;

/* The number of our GLUT window */
int window;

GLfloat zoom = -15.0f;   // viewing distance from stars.
GLfloat tilt = 90.0f;    // tilt the view
GLfloat spin;            // spin twinkling stars

GLuint loop;             // general loop variable
GLuint texture[1];       // storage for one texture;


/*
 * getint and getshort are help functions to load the bitmap byte by byte on
 * SPARC platform (actually, just makes the thing work on platforms of either
 * endianness, not just Intel's little endian)
 */

FILE *fp;

static unsigned int getint(FILE* fp)
{
  int c, c1, c2, c3;

  // get 4 bytes
  c = getc(fp);
  c1 = getc(fp);
  c2 = getc(fp);
  c3 = getc(fp);

  return ((unsigned int) c) +
    (((unsigned int) c1) << 8) +
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

static unsigned int getshort(FILE* fp)
{
  int c, c1;

  //get 2 bytes
  c = getc(fp);
  c1 = getc(fp);

  return ((unsigned int) c) + (((unsigned int) c1) << 8);
}

// quick and dirty bitmap loader...for 24 bit bitmaps with 1 plane only.
// See http://www.dcs.ed.ac.uk/~mxr/gfx/2d/BMP.txt for more info.

int ImageLoad(std::string filename, Image *image)
{
    FILE *file;
    unsigned long size;                 // size of the image in bytes.
    unsigned long i;                    // standard counter.
    unsigned short int planes;          // number of planes in image (must be 1)
    unsigned short int bpp;             // number of bits per pixel (must be 24)
    char temp;                          // used to convert bgr to rgb color.

    // make sure the file is there.
    if ((file = fopen(filename.c_str(), "rb")) == NULL)
    {
      std::cout << "File Not Found : " <<  filename << std::endl;
      return 0;
    }

    // seek through the bmp header, up to the width/height:
    fseek(file, 18, SEEK_CUR);

    // No 100% errorchecking anymore!!!

    // read the width
    image->sizeX = getint(file);
    std::cout << "width of " << filename << ": " << image->sizeX << "%lu\n" << std::endl;

    // read the height
    image->sizeY = getint(file);
    std::cout << "height of " << filename << image->sizeY << std::endl;

    // calculate the size (assuming 24 bits or 3 bytes per pixel).
    size = image->sizeX * image->sizeY * 3;

    // read the planes
    // FIXME: convert c to c++.
    planes = getshort(file);
    if (planes != 1) {
	//printf("Planes from %s is not 1: %u\n", filename, planes);
	return 0;
    }

    // read the bpp
    bpp = getshort(file);
    if (bpp != 24) {
      //printf("Bpp from %s is not 24: %u\n", filename, bpp);

      return 0;
    }

    // seek past the rest of the bitmap header.
    fseek(file, 24, SEEK_CUR);

    // read the data.
    image->data = (char *) malloc(size);
    if (image->data == NULL)
    {
      printf("Error allocating memory for color-corrected image data");
      return 0;
    }

    if ((i = fread(image->data, size, 1, file)) != 1)
    {
      //printf("Error reading image data from %s.\n", filename);
      return 0;
    }

    for (i = 0; i < size; i += 3)
    {
      // reverse all of the colors. (bgr -> rgb)
      temp = image->data[i];
      image->data[i] = image->data[i+2];
      image->data[i+2] = temp;
    }

    return 1;
}

// Load Bitmaps And Convert To Textures
GLvoid LoadGLTextures()
{
    Image *image;

    image = (Image *) malloc(sizeof(Image));

    if (!image)
    {
	printf("Error allocating space for image");
	exit(0);
    }

    if (!ImageLoad("data/star.bmp", image))
	exit(1);

    // Create Textures.
    glGenTextures(3, &texture[0]);

    // Linear filtered texture.
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, image->sizeX, image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, image->data);
};

void initParticles ()
{
  /* set up the stars */
  for (int i = 0; i < pe->nbPart(); i++)
  {
    Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, 0, 0, 0, 0);
    p->resetParticle ();
    pe->vpart(i, p);
  }
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLsizei width, GLsizei height)	// We call this right after our OpenGL window is created.
{
  LoadGLTextures();

  // Enable texture mapping.
  glEnable(GL_TEXTURE_2D);

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  // Enables Clearing Of The Depth Buffer.
  glClearDepth(1.0);

  // Enables Smooth Color Shading.
  glShadeModel(GL_SMOOTH);

  glMatrixMode(GL_PROJECTION);

  // Reset The Projection Matrix.
  glLoadIdentity();

  // Calculate The Aspect Ratio Of The Window.
  gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);

  /* setup blending */
  // Set The Blending Function For Translucency
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glEnable(GL_BLEND);

  initParticles ();
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
  // Reset The Current Viewport And Perspective Transformation.
  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, (GLfloat)width / (GLfloat) height, 0.1f, 100.0f);
  glMatrixMode(GL_MODELVIEW);
}

GLvoid DrawGLScene()
{
  // Clear The Screen And The Depth Buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindTexture(GL_TEXTURE_2D, texture[0]);

  // FIXME: handle elapsedtime.
  // FIXME: this is not suppose to be done here !
  // int currentTime = glutGet(GLUT_ELAPSED_TIME);
  // std::cout << currentTime << std::endl;

  for (int i = 0; i < pe->nbPart(); i++)
  {
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, zoom);
    glRotatef(tilt, 1.0f, 0.0f, 0.0f);

    glRotatef(pe->vpart ()[i]->angle (), 0.0f, 1.0f, 0.0f);
    glTranslatef(pe->vpart ()[i]->x (),
        pe->vpart ()[i]->y (),
        pe->vpart ()[i]->z ());

    glRotatef(-tilt, 1.0f, 0.0f, 0.0f);

    // Main star.
    glRotatef(spin, 0.0f, 0.0f, 1.0f);

    // Assign A Color Using Bytes.
    glColor4ub(pe->vpart ()[i]->r (),
        pe->vpart ()[i]->g (),
        pe->vpart ()[i]->b (),
        255);

    // Begin Drawing The Textured Quad.
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();

    spin += 0.01f;

    // Change star angle.
    pe->vpart ()[i]->angle_ += pe->vpart ()[i]->angle_;

    pe->vpart ()[i]->r_ = rand() % 256;
    pe->vpart ()[i]->g_ = rand() % 256;
    pe->vpart ()[i]->b_ = rand() % 256;

    pe->vpart ()[i]->x_ += pe->vpart ()[i]->vx_ * 0.001;//time_delta;
    pe->vpart ()[i]->y_ += pe->vpart ()[i]->vy_ * 0.001;//time_delta;
    pe->vpart ()[i]->z_ += pe->vpart ()[i]->vz_ * 0.001;//time_delta;
    // FIXME: do a dispatcher for different patterns.
    //  pe->vpart ()[i]->x_ = 3 * sin (pe->t_ + i);
    //  pe->vpart ()[i]->z_ = 2 * sin (2 * pe->t_ + i);
  }
  glutSwapBuffers();

  pe->t_ += 0.001;
}

ParticleEngine::ParticleEngine(int nbPart)
  : vpart_ (nbPart),
    nbPart_ (nbPart),
    t_ (0)

{
}

// FIXME: constructor must handle image.
Particle::Particle(/*Image img,*/ int r, int g, int b,
                   float x, float y, float z, float angle)
  : r_(r),
    g_(g),
    b_(b),
    x_(x),
    y_(y),
    z_(z),
    vx_(0),
    vy_(0),
    vz_(0),
    angle_(angle),
    // FIXME: Put it in parameter.
    lifeRemaining_(100),
    life_(100),
    isAlive_(true)
{
}

void Particle::resetParticle ()
{
  isAlive_ = true;
  lifeRemaining_ = life_;
  vx_ = (float) (rand() % 2000 - 1000) / 1000;
  vy_ = (float) (rand() % 2000 - 1000) / 1000;
  vz_ = (float) (rand() % 2000 - 1000) / 1000;
}

int main(int argc, char **argv)
{
  pe = new ParticleEngine (50);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(925, 480);
  glutInitWindowPosition(0, 0);
  window = glutCreateWindow("Particle test");
  glutDisplayFunc(&DrawGLScene);
 // glutFullScreen();
  glutIdleFunc(&DrawGLScene);
  glutReshapeFunc(&ReSizeGLScene);
  InitGL(640, 480);

  glutMainLoop();

  return 0;
}

