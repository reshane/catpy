CC=gcc
CFLAGS= -std=c99
SRC=catpy.c
OBJ=catpy

TESTS=file_make.c
TESTO=file_make


all:
	$(CC) -o $(OBJ) $(SRC) $(CFLAGS)

test:
	$(CC) -o $(TESTO) $(TESTS) $(CFLAGS)
