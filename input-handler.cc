#include "input-handler.hh"

#include "main-loop.hh"
#include <SFML/Graphics.hpp>

InputHandler::InputHandler (sf::Window& app, MainLoop& ml)
  : app_ (app)
  , ml_ (ml)
  , closed_ (false)
{
}

void
InputHandler::update ()
{
  sf::Event Event;
  Vector3f orig (30, 30, 30);

  while (app_.GetEvent(Event))
  {
    if (Event.Type == sf::Event::Closed ||
        (Event.Type == sf::Event::KeyPressed && Event.Key.Code == sf::Key::Escape))
      closed_ = true;
    else if (Event.Type == sf::Event::KeyPressed)
    {
      if (Event.Key.Code == 'z')
        ml_.camera()->front ();
      else if (Event.Key.Code == 's')
        ml_.camera()->back ();
      else if (Event.Key.Code == 'q')
        ml_.camera()->left ();
      else if (Event.Key.Code == 'd')
        ml_.camera()->right ();
      else if (Event.Key.Code == 'r')
        ml_.camera()->up ();
      else if (Event.Key.Code == 'f')
        ml_.camera()->down ();
      else if (Event.Key.Code == '1')
      {
        ml_.particle()->addEmittor(new ImmediateEmittor(100,
                      ml_.particle()->walls(), orig, "explosion"));
        ml_.particle()->initParticles();
      }
      else if (Event.Key.Code == '2')
      {
        ml_.particle()->addEmittor(new ImmediateEmittor(100,
                      ml_.particle()->walls(), orig, "nova"));
        ml_.particle()->initParticles();
      }
      else if (Event.Key.Code == '3')
      {
        // FIXME: maybe less particles.
        ml_.particle()->addEmittor(new ProgressiveEmittor(500,
                      ml_.particle()->walls(), orig, "smoke"));
        ml_.particle()->initParticles();
      }
      else if (Event.Key.Code == '4')
      {
        ml_.particle()->addEmittor(new ProgressiveEmittor(1000,
                      ml_.particle()->walls(), orig, "fire"));
        ml_.particle()->initParticles();
      }
      else if (Event.Key.Code == '5')
      {
        ml_.particle()->addEmittor(new ImmediateEmittor(1000,
                      ml_.particle()->walls(), orig, "fragmentation"));
        ml_.particle()->initParticles();
      }
    }
    else if (Event.Type == sf::Event::MouseMoved)
      ml_.camera()->move(Event.MouseMove.X, Event.MouseMove.Y);
  }
}
