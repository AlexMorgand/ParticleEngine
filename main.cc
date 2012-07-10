#include "tools.hh"
#include "particles.hh"


int main(int argc, char **argv)
{
  /* The number of our GLUT window */
  int window;

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(925, 480);
  glutInitWindowPosition(0, 0);
  window = glutCreateWindow("PRPA project");
  (void) window;
  glutDisplayFunc(&DrawGLScene);
  glutIdleFunc(&DrawGLScene);
  glutReshapeFunc(&ReSizeGLScene);
  InitGL(640, 480);

  glutMainLoop();

  return 0;
}

