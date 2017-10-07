CC=gcc
CFLAGS=-std=c99
CMD=$(CC) $(CFLAGS)

COMPILE=$(CMD) -g -c

FILES=Main.o MinHeap.o Process.o RandomNumberGenerator.o Output.o Scheduler.o

all: MinHeap.o Process.o RandomNumberGenerator.o Output.o Scheduler.o Main.o
	$(CMD) $(FILES) -g -o app.bin

clean:
	rm *.o app.bin

MinHeap.o: MinHeap.c
	$(COMPILE) MinHeap.c

Process.o: Process.c
	$(COMPILE) Process.c

Output.o: Output.c
	$(COMPILE) Output.c

Scheduler.o: Scheduler.c
	$(COMPILE) Scheduler.c

RandomNumberGenerator.o: RandomNumberGenerator.c
	$(COMPILE) RandomNumberGenerator.c

Main.o: Main.c
	$(COMPILE) Main.c

