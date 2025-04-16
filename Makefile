CC := gcc
CFLAGS := -g
#CFLAGS := -Wall -Wextra

all: main

main: main.c token.o scanner.o clox.h clox.o
	$(CC) $(CFLAGS) main.c -o main scanner.o token.o clox.o

