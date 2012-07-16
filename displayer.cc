#include "displayer.hh"

#include "tools.hh"

Displayer::Displayer (int width, int height, ParticleEngine* pe, Camera* cam)
  : pe_ (pe),
    cam_ (cam)
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
  gluPerspective(45.0f, (GLfloat) width / (GLfloat) height, 0.1f, 200.0f);

  glMatrixMode(GL_MODELVIEW);

  /* setup blending */
  // Set The Blending Function For Translucency
  glBlendFunc(GL_SRC_ALPHA,GL_ONE);
  glEnable(GL_BLEND);
  glEnable (GL_DEPTH_TEST);
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

  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  glDisable (GL_TEXTURE_2D);
  glColor3ub (255, 255, 255);
  // drawing the floor
  glPushMatrix ();
  for (int i = 0; i <= 10; ++i)
  {
    if (i > 0)
      glTranslatef (10, 0, 0);
    glBegin (GL_LINES);
    glVertex3f (0, 0, 0);
    glVertex3f (0, 100, 0);
    glEnd ();
  }
  glPopMatrix ();

  glPushMatrix ();
  for (int i = 0; i <= 10; ++i)
  {
    if (i > 0)
      glTranslatef (0, 10, 0);
    glBegin (GL_LINES);
    glVertex3f (0, 0, 0);
    glVertex3f (100, 0, 0);
    glEnd ();
  }
  glPopMatrix ();

  // drawing the walls
  glBegin (GL_QUADS);
  glVertex3f (0, 0, 0);
  glVertex3f (0, 0, 100);
  glVertex3f (0, 100, 100);
  glVertex3f (0, 100, 0);
  glEnd ();

  glBegin (GL_QUADS);
  glVertex3f (0, 0, 0);
  glVertex3f (0, 0, 100);
  glVertex3f (100, 0, 100);
  glVertex3f (100, 0, 0);
  glEnd ();

  glBegin (GL_QUADS);
  glVertex3f (0, 100, 0);
  glVertex3f (0, 100, 100);
  glVertex3f (100, 100, 100);
  glVertex3f (100, 100, 0);
  glEnd ();

  glBegin (GL_QUADS);
  glVertex3f (100, 0, 0);
  glVertex3f (100, 0, 100);
  glVertex3f (100, 100, 100);
  glVertex3f (100, 100, 0);
  glEnd ();

  glEnable(GL_TEXTURE_2D);
  glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

  // drawing particles

  for (std::map<int, ParticleEmittor*>::iterator it = pe_->lpe()->begin();
      it != pe_->lpe()->end(); ++it)
  {
    if (it->second->etype() == "immediate")
    {
      ImmediateEmittor* p = (ImmediateEmittor*) it->second;
      // FIXME : cut it !
      for (int i = 0; i < p->nbPart(); i++)
      {
        glDepthMask(GL_FALSE);
        glPushMatrix();

        glTranslatef(p->vpart ()[i]->pos ()(0),
            p->vpart ()[i]->pos ()(1),
            p->vpart ()[i]->pos ()(2));

        glRotatef(cam_->theta(), 0.0f, 0.0f, 1.0f);
        glRotatef(90 - cam_->phi(), 0.0f, 1.0f, 0.0f);

        // Main star.
        glRotatef(spin, 0.0f, 0.0f, 1.0f);

        // Assign A Color Using Bytes.
        glColor4ub(p->vpart ()[i]->rgb ()(0), p->vpart ()[i]->rgb ()(1),
            p->vpart ()[i]->rgb ()(2), 255);

        if (p->type() == "smoke")
          glBindTexture(GL_TEXTURE_2D, texture[4]);
        else
          glBindTexture(GL_TEXTURE_2D, texture[0]);

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
    else
    {
      ProgressiveEmittor* p = (ProgressiveEmittor*) it->second;
      for (std::list<Particle*>::iterator it = p->pvpart().begin();
           it != p->pvpart().end(); ++it)
      {
        glDepthMask(GL_FALSE);
        glPushMatrix();

        glTranslatef((*it)->pos ()(0), (*it)->pos ()(1), (*it)->pos ()(2));

        glRotatef(cam_->theta(), 0.0f, 0.0f, 1.0f);
        glRotatef(90 - cam_->phi(), 0.0f, 1.0f, 0.0f);

        // Main star.
        glRotatef(spin, 0.0f, 0.0f, 1.0f);

        // Assign A Color Using Bytes.
        glColor4ub((*it)->rgb ()(0), (*it)->rgb ()(1), (*it)->rgb ()(2), 255);

        if (p->type() == "smoke")
          glBindTexture(GL_TEXTURE_2D, texture[4]);
        else
          glBindTexture(GL_TEXTURE_2D, texture[0]);

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
}
