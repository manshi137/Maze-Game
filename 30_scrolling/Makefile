CC=g++

CFLAGS= -c -Wall

all : main

main : 30_scrolling.o
	g++ 30_scrolling.cpp -w -lSDL2 -lSDL2_image -o main
30_scrolling.o : 30_scrolling.cpp
	$(CC) $(CFLAGS) 30_scrolling.cpp
	
clean:
	rm -rf *o main
