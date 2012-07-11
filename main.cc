#include "main-loop.hh"

int main(int argc, char **argv)
{
  MainLoop ml (argc, argv);

  ml.start ();

  return 0;
}

