INCLUDE = -I/usr/X11R6/include/
LIBDIR  = -L/usr/X11R6/lib

COMPILERFLAGS = -Wall -g -ggdb
CXX = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm

all: particles

particles: particles.o
	$(CXX) $(CFLAGS) -o $@ $(LIBDIR) $< $(LIBRARIES)

clean:
	rm particles particles.o
