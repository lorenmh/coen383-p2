CC=gcc
CFLAGS=-std=c99
CMD=$(CC) $(CFLAGS)

all: Main.o MinHeap.o Process.o RandomNumberGenerator.o
	$(CMD) Main.o minheap.o Process.o RandomNumberGenerator.o -o app.bin

main.o: Main.c
	$(CMD) -c Main.c

Process.o: Process.c
	$(CMD) -c Process.c

RandomNumberGenerator.o: RandomNumberGenerator.c
	$(CMD) -c RandomNumberGenerator.c

main.o: main.c
	$(CMD) -c main.c

minheap.o: MinHeap.c
	$(CMD) -c MinHeap.c
