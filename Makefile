CC := gcc
CFLAGS := -g
#CFLAGS := -Wall -Wextra

all: main

main: main.c token.o token.h token_types.h scanner.o scanner.h clox.o clox.h
	$(CC) $(CFLAGS) main.c -o main scanner.o token.o

