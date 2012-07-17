#ifndef DISPLAYER_HH_
# define DISPLAYER_HH_

# include "particle.hh"
# include "camera.hh"

// displays all that need to be displayed
class Displayer
{
public:
  Displayer (int width, int height, ParticleEngine* pe, Camera* cam);
  ~Displayer ();

  // display the emmitors and the map
  void draw ();

protected:
  ParticleEngine* pe_;
  Camera* cam_;
};

#endif /// !DISPLAYER_HH_
