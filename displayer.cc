#include "displayer.hh"

#include "tools.hh"

Displayer::Displayer (int width, int height, ParticleEngine* pe)
  : pe_ (pe)
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

  pe_->initParticles ();
}

Displayer::~Displayer ()
{
}

void
Displayer::draw ()
{
  // FIXME: barriere pour tous les émiteurs
  // FIXME: parallel for pour les particules

  for (std::map<int, ParticleEmittor*>::iterator it = pe_->lpe()->begin();
      it != pe_->lpe()->end(); ++it)
  {
    ParticleEmittor* p = it->second;
    for (int i = 0; i < p->nbPart(); i++)
    {
      glPushMatrix();

      glTranslatef(0.0f, 0.0f, 0.0f);
      glRotatef(tilt, 1.0f, 0.0f, 0.0f);

      glRotatef(p->vpart ()[i]->angle (), 0.0f, 1.0f, 0.0f);

      glTranslatef(p->vpart ()[i]->x (),
          p->vpart ()[i]->y (),
          p->vpart ()[i]->z ());

      glRotatef(-tilt, 1.0f, 0.0f, 0.0f);

      // Main star.
      glRotatef(spin, 0.0f, 0.0f, 1.0f);

      // Assign A Color Using Bytes.
      glColor4ub(p->vpart ()[i]->r (), p->vpart ()[i]->g (),
          p->vpart ()[i]->b (), 255);

      // Begin Drawing The Textured Quad.
      glBegin(GL_QUADS);

      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);

      glEnd();

      glPopMatrix();
    }
  }
}
