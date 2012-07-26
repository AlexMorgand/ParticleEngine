#include "displayer.hh"

#include "tools.hh"
#include <cassert>
#include <sstream>
#include <tbb/parallel_for.h>

Displayer::Displayer (int width, int height, ParticleEngine* pe, Camera* cam)
  : pe_ (pe),
    cam_ (cam),
    para_ (false),
    ip_ (0),
    pp_ (0),
    immed_ (false)
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

  // loading the font to display FPS
  if (!font_.LoadFromFile("data/lucon.ttf"))
  {
    std::cout << "Can't load the font" << std::endl;
    assert (false);
  }

  parallel_on_.SetText ("Parallel Mode Off");
  parallel_on_.SetFont (font_);
  parallel_on_.SetSize (20);
  parallel_on_.SetColor(sf::Color(255, 255, 255));
  parallel_on_.Move(590.f, 0);

  fps_.SetText ("FPS: 0");
  fps_.SetFont (font_);
  fps_.SetSize (20);
  fps_.SetColor(sf::Color(128, 128, 128));

}

void
Displayer::parallel (bool para)
{
  para_ = para;
  if (para)
    parallel_on_.SetText ("Parallel Mode On");
  else
    parallel_on_.SetText ("Parallel Mode Off");

}

void
Displayer::fps (int frame)
{
  std::stringstream st;
  st << frame;
  std::string tmp = "FPS: ";
  tmp += st.str();

  fps_.SetText (tmp.c_str ());

}

Displayer::~Displayer ()
{
}

void
Displayer::draw (sf::RenderWindow& w)
{
  // FIXME: barriere pour tous les émiteurs

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


      // if (para_)
      // 	{
      // 	  ip_ = p;
      // 	  immed_ = true;

      // 	  parallel_for(tbb::blocked_range<size_t> (0, p->nbPart ()),
      // 	  	       *this);
      // 	}
      // else
      // 	{
	  // FIXME : cut it !
	  for (int i = 0; i < p->nbPart(); i++)
	    {
	      if (p->vpart()[i]->isAlive())
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
	    // }
	}
    }
    else
      {
	ProgressiveEmittor* p = (ProgressiveEmittor*) it->second;

	// if (para_)
	//   {
	//     pp_ = p;
	//     immed_ = false;

	//     parallel_for(tbb::blocked_range<size_t> (0, p->pvpart ().size ()),
	// 		 *this);
	//   }
	// else
	//   {
	    for (std::list<Particle*>::iterator it = p->pvpart().begin();
		 it != p->pvpart().end(); ++it)
	      {
		if ((*it)->isAlive())
		  {

		    glDepthMask(GL_FALSE);
		    glPushMatrix();

		    glTranslatef((*it)->pos ()(0), (*it)->pos ()(1), (*it)->pos ()(2));

		    glRotatef(cam_->theta(), 0.0f, 0.0f, 1.0f);
		    glRotatef(90 - cam_->phi(), 0.0f, 1.0f, 0.0f);

		    // Main star.
		    glRotatef(spin, 0.0f, 0.0f, 1.0f);

		    // Assign A Color Using Bytes.
		    glColor4ub((*it)->rgb ()(0), (*it)->rgb ()(1), (*it)->rgb ()(2),
			       255 - ((*it)->life() - (*it)->lifeRemaining()));

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
	      // }
	  }
      }
  }

  w.Draw (parallel_on_);
  w.Draw (fps_);
}

void
Displayer::operator() (const tbb::blocked_range<size_t>& r) const
{
  unsigned int i = 0;

  if (immed_)
    {
      for (i = r.begin (); i != r.end (); ++i)
	{
	  if (ip_->vpart()[i]->isAlive())
	    {
	      glDepthMask(GL_FALSE);
	      glPushMatrix();

	      glTranslatef(ip_->vpart ()[i]->pos ()(0),
			   ip_->vpart ()[i]->pos ()(1),
			   ip_->vpart ()[i]->pos ()(2));

	      glRotatef(cam_->theta(), 0.0f, 0.0f, 1.0f);
	      glRotatef(90 - cam_->phi(), 0.0f, 1.0f, 0.0f);

	      // Main star.
	      glRotatef(spin, 0.0f, 0.0f, 1.0f);

	      // Assign A Color Using Bytes.
	      glColor4ub(ip_->vpart ()[i]->rgb ()(0), ip_->vpart ()[i]->rgb ()(1),
			 ip_->vpart ()[i]->rgb ()(2), 255);

	      if (ip_->type() == "smoke")
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
  else
    {
      std::list<Particle*>::iterator it = pp_->pvpart ().begin ();

      for (; i < r.begin (); ++i, ++it);

      for (; i != r.end () && it != pp_->pvpart ().end (); ++it, ++i)
	{
	  if ((*it)->isAlive())
	    {

	      glDepthMask(GL_FALSE);
	      glPushMatrix();

	      glTranslatef((*it)->pos ()(0), (*it)->pos ()(1), (*it)->pos ()(2));

	      // Assign A Color Using Bytes.
	      glColor4ub((*it)->rgb ()(0), (*it)->rgb ()(1), (*it)->rgb ()(2),
			 (*it)->lifeRemaining() / (*it)->life() * 50);

	      glRotatef(cam_->theta(), 0.0f, 0.0f, 1.0f);
	      glRotatef(90 - cam_->phi(), 0.0f, 1.0f, 0.0f);

	      // Main star.
	      glRotatef(spin, 0.0f, 0.0f, 1.0f);

	      // Assign A Color Using Bytes.
	      glColor4ub((*it)->rgb ()(0), (*it)->rgb ()(1), (*it)->rgb ()(2),
			 255 - ((*it)->life() - (*it)->lifeRemaining()));

	      if (pp_->type() == "smoke")
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
