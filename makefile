all:
	g++ -Wall -g -ggdb -o particles camera.cc tools.cc particles.cc plane.cc main.cc -lX11 -lXi -lXmu -lglut -lGL -lGLU -lm


clean:
	rm particles
