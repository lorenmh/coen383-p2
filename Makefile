CC=gcc
CFLAGS=-std=c99
CMD=$(CC) $(CFLAGS)

all: main.o minheap.o
	$(CMD) main.o minheap.o -o app.bin

main.o: main.c
	$(CMD) -c main.c

minheap.o: minheap.c
	$(CMD) -c minheap.c
