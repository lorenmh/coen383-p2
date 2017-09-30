CC=gcc
CFLAGS=-std=c99
CMD=$(CC) $(CFLAGS)

all: main.o
	$(CMD) main.o -o app.bin

main.o:
	$(CMD) -c main.c
