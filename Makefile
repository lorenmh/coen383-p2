CC=gcc
CFLAGS=-std=c99
CMD=$(CC) $(CFLAGS)

all: Main.o MinHeap.o Process.o RandomNumberGenerator.o Output.o Scheduler.o
	$(CMD) Main.o minheap.o Process.o RandomNumberGenerator.o Output.o Scheduler.o -o app.bin

main.o: Main.c
	$(CMD) -c Main.c

Process.o: Process.c
	$(CMD) -c Process.c

Output.o: Output.c
	$(CMD) -c Output.c

Scheduler.o: Scheduler.c
	$(CMD) -c Scheduler.c

RandomNumberGenerator.o: RandomNumberGenerator.c
	$(CMD) -c RandomNumberGenerator.c

main.o: main.c
	$(CMD) -c main.c

minheap.o: MinHeap.c
	$(CMD) -c MinHeap.c
