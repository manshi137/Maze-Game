CC=g++

CFLAGS= -c -Wall

all : game

game : Finalclient.o
	g++ Finalclient.cpp -w -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o game
30_scrolling.o : Finalclient.cpp
	$(CC) $(CFLAGS) Finalclient.cpp
	
clean:
	rm -rf *o game
