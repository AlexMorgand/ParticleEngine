#ifndef DISPLAYER_HH_
# define DISPLAYER_HH_

# include "particles.hh"

// displays all that need to be displayed
class Displayer
{
public:
  Displayer (int width, int height, ParticleEngine* pe);
  ~Displayer ();

  // display the emmitors and the map
  void draw ();

protected:
  ParticleEngine* pe_;
};

#endif /// !DISPLAYER_HH_
