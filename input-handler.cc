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
        ml_.particle()->addEmittor(new ParticleEmittor(100,
                      ml_.particle()->walls(), "explosion"));
        ml_.particle()->initParticles();
      }
      else if (Event.Key.Code == '2')
      {
        ml_.particle()->addEmittor(new ParticleEmittor(100,
                      ml_.particle()->walls(), "nova"));
        ml_.particle()->initParticles();
      }
      else if (Event.Key.Code == '3')
      {
        ml_.particle()->addEmittor(new ParticleEmittor(500,
                      ml_.particle()->walls(), "smoke"));
        ml_.particle()->initParticles();
      }
    }
    else if (Event.Type == sf::Event::MouseMoved)
      ml_.camera()->move(Event.MouseMove.X, Event.MouseMove.Y);
  }
}
