INCLUDE = -I/usr/X11R6/include/
LIBDIR  = -L/usr/X11R6/lib

COMPILERFLAGS = -Wall -g -ggdb
CXX = g++
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBRARIES = -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm

all:
	g++ -Wall -g -ggdb -o particles  tools.cc particles.cc main.cc -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm


clean:
	rm particles particles.o main.o tools.o
