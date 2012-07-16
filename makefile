all:
	g++ -Wall -g -ggdb -o particles camera.cc displayer.cc main-loop.cc input-handler.cc plane.cc tools.cc particles.cc particle-emittor.cc particle-emittor-para.cc main.cc -lGLEW -lglut -lGL -lGLU -lm -lsfml-graphics -lsfml-window -lsfml-system -ltbb

clean:
	rm particles
