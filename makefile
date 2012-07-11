all:
	g++ -Wall -g -ggdb -o particles camera.cc displayer.cc main-loop.cc input-handler.cc tools.cc particles.cc main.cc -lX11 -lXi -lXmu -lGLEW -lglut -lGL -lGLU -lm -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm particles

