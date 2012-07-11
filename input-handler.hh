#ifndef INPUT_HANDLER_HH_
# define INPUT_HANDLER_HH_

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
};

#endif /// !INPUT_HANDLER_HH_
