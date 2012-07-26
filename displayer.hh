#ifndef DISPLAYER_HH_
# define DISPLAYER_HH_

# include "particle.hh"
# include "camera.hh"
# include <SFML/Graphics.hpp>

// displays all that need to be displayed
class Displayer
{
public:
  Displayer (int width, int height, ParticleEngine* pe, Camera* cam);
  ~Displayer ();

  // display the emmitors and the map
  void draw (sf::RenderWindow& w);
  void parallel (bool para);
  void fps (int frame);

  void operator() (const tbb::blocked_range<size_t>& r) const;

protected:
  ParticleEngine* pe_;
  Camera* cam_;
  sf::Font font_;
  sf::String parallel_on_;
  sf::String fps_;
  bool para_;

  ImmediateEmittor* ip_;
  ProgressiveEmittor* pp_;
  bool immed_;
};

#endif /// !DISPLAYER_HH_
