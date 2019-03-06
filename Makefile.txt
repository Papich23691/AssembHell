CC = gcc
SRC = memory.c parse.c main.c files.c
OBJ = as 

all:
	$(CC) $(SRC) -o $(OBJ) -g -pedantic -ansi -Wall
