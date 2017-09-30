CC=gcc
CFLAGS=-std=c99
ODIR=bin
CMD=$(CC) $(CFLAGS)

# on MacOS, gcc is symlinked to clang, weirdness may occur as a result

all: main.o
	$(CMD) main.o -o app.bin

main.o:
	$(CMD) -c main.c
