CC=g++
CFLAGS=-Wall -std=c++11

all: main.cpp LinkedList.h Vector.h
	$(CC)	main.cpp	$(CFLAGS)	-o	run

clean:
	rm -f *.o
	rm -f run