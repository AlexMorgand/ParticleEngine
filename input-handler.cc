#include "input-handler.hh"

#include "main-loop.hh"
#include <SFML/Graphics.hpp>

InputHandler::InputHandler (sf::Window& app, MainLoop& ml)
  : app_ (app)
  , ml_ (ml)
  , closed_ (false)
  , parallel_ (false)
  , x_coord (0)
  , y_coord (20)
  , orig (x_coord, y_coord, 30)
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
      if (Event.Key.Code >= '1' && Event.Key.Code <= '9')
      	{
      	  x_coord += 20;
      	  if (x_coord >= 100)
      	    {
      	      x_coord = 20;
      	      y_coord += 20;
      	      if (y_coord >= 100)
      		y_coord = 20;
      	    }
      	  orig (0, x_coord);
      	  orig (1, y_coord);
      	}

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
        ImmediateEmittor* ie = new ImmediateEmittor(200,
                      ml_.particle()->walls(), orig, "explosion");
        ml_.particle()->addEmittor(ie);
        ie->initParticles();
      }
      else if (Event.Key.Code == '2')
      {
        ImmediateEmittor* ie = new ImmediateEmittor(100,
                      ml_.particle()->walls(), orig, "nova");
        ml_.particle()->addEmittor(ie);
        ie->initParticles();
      }
      else if (Event.Key.Code == '3')
      {
        ProgressiveEmittor* pe = new ProgressiveEmittor(500,
                      ml_.particle()->walls(), orig, "smoke");
        ml_.particle()->addEmittor(pe);
        pe->initParticles();
      }
      else if (Event.Key.Code == '4')
      {
        ProgressiveEmittor* pe = new ProgressiveEmittor(1000,
                      ml_.particle()->walls(), orig, "fire");
        ml_.particle()->addEmittor(pe);
        pe->initParticles();
      }
      else if (Event.Key.Code == '5')
      {
        ImmediateEmittor* ie = new ImmediateEmittor(1000,
                      ml_.particle()->walls(), orig, "fragmentation");
        ml_.particle()->addEmittor(ie);
        ie->initParticles();
      }
      else if (Event.Key.Code == 'p')
      {
	parallel_ = !parallel_;
	ml_.particle()->parallel (parallel_);
	ml_.displayer()->parallel (parallel_);
      }
    }
    else if (Event.Type == sf::Event::MouseMoved)
      ml_.camera()->move(Event.MouseMove.X, Event.MouseMove.Y);
  }
}
