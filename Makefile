CC = gcc
SRC = util.c memory.c parse.c main.c files.c error.c 
OBJ = as 

all:
	$(CC) $(SRC) -o $(OBJ) -g -pedantic -ansi -Wall
