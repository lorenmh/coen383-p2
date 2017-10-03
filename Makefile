CC=gcc
CFLAGS=-std=c99
CMD=$(CC) $(CFLAGS)

all: main.o minheap.o Process.o RandomNumberGenerator.o
	$(CMD) main.o minheap.o Process.o RandomNumberGenerator.o -o app.bin

main.o: main.c
	$(CMD) -c main.c

Process.o: Process.c
	$(CMD) -c Process.c

RandomNumberGenerator.o: RandomNumberGenerator.c
	$(CMD) -c RandomNumberGenerator.c

main.o: main.c
	$(CMD) -c main.c

minheap.o: minheap.c
	$(CMD) -c minheap.c
