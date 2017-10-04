CC=gcc
CFLAGS=-std=c99
CMD=$(CC) $(CFLAGS)

FILES=Main.o MinHeap.o Process.o RandomNumberGenerator.o Output.o Scheduler.o -o app.bin

all: Main.o MinHeap.o Process.o RandomNumberGenerator.o Output.o Scheduler.o
	$(CMD) $(FILES) -o app.bin

linux: Main.o MinHeap.o Process.o RandomNumberGenerator.o Output.o Scheduler.o
	$(CMD) $(FILES) -DLINUX -o app.bin

Process.o: Process.c
	$(CMD) -c Process.c

Output.o: Output.c
	$(CMD) -c Output.c

Scheduler.o: Scheduler.c
	$(CMD) -c Scheduler.c

RandomNumberGenerator.o: RandomNumberGenerator.c
	$(CMD) -c RandomNumberGenerator.c

Main.o: Main.c
	$(CMD) -c Main.c

Minheap.o: MinHeap.c
	$(CMD) -c MinHeap.c
