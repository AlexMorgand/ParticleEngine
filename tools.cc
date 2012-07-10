#include "tools.hh"

// FIXME: more flexible.
GLuint texture[5];
GLfloat zoom = -15.0f;
GLfloat tilt = 90.0f;
GLfloat spin;
float elapsedTime = 0.0f;

int ImageLoad(std::string filename, Image *image)
{
  FILE *file;
  unsigned long size;
  unsigned long i;
  char temp;

  // make sure the file is there.
  if ((file = fopen(filename.c_str(), "rb")) == NULL)
  {
    std::cout << "File Not Found : " <<  filename << std::endl;
    return 0;
  }

  // seek through the bmp header, up to the width/height:
  fseek(file, 18, SEEK_CUR);

  // Read the width.
  image->sizeX = getint(file);
  image->sizeY = getint(file);

  std::cout << "width of " << filename << ": " << image->sizeX << std::endl;
  std::cout << "height of " << filename << ": " << image->sizeY << std::endl;

  // Calculate the size (assuming 24 bits or 3 bytes per pixel).
  size = image->sizeX * image->sizeY * 3;

  // read the planes
  getshort(file);


  // read the bpp
  getshort(file);

  // seek past the rest of the bitmap header.
  fseek(file, 24, SEEK_CUR);

  // read the data.
  image->data = (char *) malloc(size);

  if ((i = fread(image->data, size, 1, file)) != 1)
  {
    std::cout << "Error reading image data from " << filename << std::endl;
    return 0;
  }

  for (i = 0; i < size; i += 3)
  {
    // Reverse all of the colors. (bgr -> rgb).
    temp = image->data[i];
    image->data[i] = image->data[i + 2];
    image->data[i + 2] = temp;
  }

  return 1;
}

unsigned int getint(FILE* fp)
{
  // get 4 bytes
  int c = getc(fp);
  int c1 = getc(fp);
  int c2 = getc(fp);
  int c3 = getc(fp);

  return ((unsigned int) c) +
    (((unsigned int) c1) << 8) +
    (((unsigned int) c2) << 16) +
    (((unsigned int) c3) << 24);
}

unsigned int getshort(FILE* fp)
{
  unsigned int c, c1;

  // Get 2 bytes.
  c = getc(fp);
  c1 = getc(fp);

  return c + (c1 << 8);
}

GLvoid LoadGLTextures()
{
  std::vector<Image*> limage (5);

  // FIXME: Load a directory ! + load the other images !
  for (int i = 0; i < 1; ++i)
  {
   // limage[i] = (Image *) malloc(sizeof (Image));
   // if (!ImageLoad("data/star.bmp", limage[i]))
   //   exit(1);
   // if (!ImageLoad("data/fairy.bmp", limage[i]))
    //  exit(1);
  }
  limage[0] = (Image *) malloc(sizeof (Image));
  if (!ImageLoad("data/star.bmp", limage[0]))
    exit(1);
  limage[1] = (Image *) malloc(sizeof (Image));
  if (!ImageLoad("data/fairy.bmp", limage[1]))
    exit(1);
 // limage[2] = (Image *) malloc(sizeof (Image));
 // if (!ImageLoad("data/flare.bmp", limage[2]))
 //   exit(1);
  limage[3] = (Image *) malloc(sizeof (Image));
  if (!ImageLoad("data/failsquare.bmp", limage[3]))
    exit(1);

  // Create Textures.
  glGenTextures(1, &texture[0]);
  glGenTextures(1, &texture[1]);
  glGenTextures(1, &texture[2]);
  glGenTextures(1, &texture[3]);

  // Linear filtered texture.
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, limage[0]->sizeX, limage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, limage[0]->data);
};

void initParticles ()
{
  ParticleEngine* pe = ParticleEngine::instanciate();

  for (int i = 0; i < pe->nbPart(); i++)
  {
    Particle* p = new Particle (rand() % 256, rand() % 256, rand() % 256, 0, 0, 0, 0);
    p->resetParticle ();
    pe->vpart(i, p);
  }
}

GLvoid InitGL(GLsizei width, GLsizei height)
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
  ParticleEngine* pe = ParticleEngine::instanciate();

  // Clear The Screen And The Depth Buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // FIXME: handle elapsedtime.
  // FIXME: this is not suppose to be done here !
  int currentTime = glutGet(GLUT_ELAPSED_TIME);

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
    glColor4ub(pe->vpart ()[i]->r (), pe->vpart ()[i]->g (),
               pe->vpart ()[i]->b (), 255);

    // Begin Drawing The Textured Quad.
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();

    spin += 0.01f;

    pe->vpart ()[i]->r_ = rand() % 256;
    pe->vpart ()[i]->g_ = rand() % 256;
    pe->vpart ()[i]->b_ = rand() % 256;

    if (pe->type () == "explosion")
    {
      pe->vpart ()[i]->x_ += pe->vpart ()[i]->vx_ * elapsedTime;
      pe->vpart ()[i]->y_ += pe->vpart ()[i]->vy_ * elapsedTime;
      pe->vpart ()[i]->z_ += pe->vpart ()[i]->vz_ * elapsedTime;
    }
    else if (pe->type () == "nova")
    {
      pe->vpart ()[i]->x_ += pe->vpart ()[i]->vx_ * elapsedTime;
      pe->vpart ()[i]->y_ += pe->vpart ()[i]->vy_ * elapsedTime;
      pe->vpart ()[i]->z_ += pe->vpart ()[i]->vz_ * elapsedTime;
    }
    else if (pe->type () == "circle")
    {
      // FIXME: do a dispatcher for different patterns.
      pe->vpart ()[i]->x_ = 3 * sin (pe->t_ + i);
      pe->vpart ()[i]->z_ = 2 * sin (2 * pe->t_ + i);
    }

    // FIXME: Dying not everytime.
    // Handle remaining life.
    pe->vpart ()[i]->lifeRemaining_--;
    if (pe->vpart ()[i]->lifeRemaining_ < 0)
      pe->vpart ()[i]->resetParticle();
  }
  glutSwapBuffers();

  elapsedTime = (glutGet(GLUT_ELAPSED_TIME) - currentTime) / 100.0f;
  pe->t_ += elapsedTime;
}
