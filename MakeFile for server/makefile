CC=g++

CFLAGS= -c -Wall

all : game

game : Finalserver.o
	g++ Finalserver.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o game
30_scrolling.o : Finalserver.cpp
	$(CC) $(CFLAGS) Finalserver.cpp
	
clean:
	rm -rf *o game