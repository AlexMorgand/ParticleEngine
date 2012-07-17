#ifndef MAIN_LOOP_HH_
# define MAIN_LOOP_HH_

# include <GL/glew.h>
# include <GL/glut.h>
# include <SFML/Graphics.hpp>
# include "camera.hh"
# include "input-handler.hh"
# include "particle.hh"
# include "displayer.hh"

class MainLoop
{
public:
  MainLoop (int argc, char** argv);

  void start();

  Camera* camera() { return c; }
  ParticleEngine* particle() { return pe; }

protected:
  Camera* c;
  InputHandler* ih;
  ParticleEngine* pe;
  Displayer* d;

  sf::RenderWindow window;
};

#endif /// !MAIN_LOOP_HH_
