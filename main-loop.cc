#include "main-loop.hh"

MainLoop::MainLoop (int argc, char** argv)
  : window (sf::VideoMode(800, 600, 32), "PRPA project")
{
  // FIXME: init le ParticleEngine ici avec ses paramètres de base
  //        et decommente le FIXME suivant
  // // FIXME: put options clearly. Indicates format here or in the README.
  // if (argc > 1)
  // {
  //   std::string type = std::string(argv[1]);
  //   pe->type(type);
  // }

  window.SetFramerateLimit(200);
  window.EnableKeyRepeat(true);
  window.PreserveOpenGLStates(true);

  glewInit();

  c = new Camera(Vector3f(0, -1, 20));
  pe = new// (tbb::task::allocate_root())
    ParticleEngine();
  ih = new InputHandler(window, *this);
  d = new Displayer(800, 600, pe, c);
}

void
MainLoop::start()
{
  sf::Clock Clock;

  float tmp = 0;
  int frame = 0;

  while (!ih->close())
  {
    float elapsed = Clock.GetElapsedTime();
    Clock.Reset();
    ++frame;
    tmp += elapsed;

    if (tmp > 1)
      {
	tmp -= 1;
	d->fps (frame);
	frame = 0;
      }

    c->time_set(elapsed);
    ih->update();

    // pe->elapsed(elapsed);
    // tbb::task::spawn_root_and_wait(*pe);
    pe->update(elapsed);

    window.Clear();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    c->view();
    d->draw(window);

    window.Display();
  }

  window.Close();
}
