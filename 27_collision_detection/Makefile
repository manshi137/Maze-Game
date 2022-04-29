CC=g++

CFLAGS= -c -Wall

all : main

main : 27_collision_detection.o
	g++ 27_collision_detection.cpp -w -lSDL2 -lSDL2_image -o main
27_collision_detection.o : 27_collision_detection.cpp
	$(CC) $(CFLAGS) 27_collision_detection.cpp
	
clean:
	rm -rf *o main
