CC=gcc
CFLAGS= -std=c99
SRC=catpy.c
OBJ=catpy

all:
	$(CC) -o $(OBJ) $(SRC) $(CFLAGS)
