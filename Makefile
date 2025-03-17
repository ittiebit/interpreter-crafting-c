CC := gcc
FLAGS := -g

all: main

main: main.c token.o token.h token_types.h scanner.o scanner.h clox.o clox.h
	$(CC) $(FLAGS) main.c -o main scanner.o token.o

