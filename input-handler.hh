#ifndef INPUT_HANDLER_HH_
# define INPUT_HANDLER_HH_

# include "vector.hh"

namespace sf
{
class Window;
}
class MainLoop;

// handle the inputs
class InputHandler
{
public:
  InputHandler (sf::Window& app, MainLoop& ml);
  ~InputHandler ();

  // check if new inputs are recognized
  void update ();

  // check if the window has to be closed
  bool close () { return closed_; }

protected:
  sf::Window& app_;
  MainLoop& ml_;

  bool closed_;
  bool parallel_;

  int x_coord;
  int y_coord;
  Vector3f orig;
};

#endif /// !INPUT_HANDLER_HH_
