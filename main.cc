#include "tools.hh"
#include "particles.hh"

Camera* Camera::ca_ = 0;

int main(int argc, char **argv)
{
  /* The number of our GLUT window */
  int window;

  ParticleEngine* pe = ParticleEngine::instanciate();
  Camera* c = Camera::instanciate ();

  // FIXME: put options clearly. Indicates format here or in the README.
  if (argc > 1)
  {
    std::string type = std::string(argv[1]);
    pe->type(type);
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(925, 480);
  glutInitWindowPosition(0, 0);
  window = glutCreateWindow("PRPA project");
  c->window(window);
  glutDisplayFunc(&DrawGLScene);
  glutIdleFunc(&DrawGLScene);
  glutReshapeFunc(&ReSizeGLScene);
  glutKeyboardFunc(&input);
  glutPassiveMotionFunc(&mouse);
  InitGL(640, 480);

  glutMainLoop();

  return 0;
}

