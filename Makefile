CC = gcc
SRC = src/util.c src/memory.c src/parse.c src/main.c src/files.c src/error.c 
OBJ = as 

all:
	$(CC) $(SRC) -o $(OBJ) -ggdb3 -pedantic -ansi -Wall -Iinclude
