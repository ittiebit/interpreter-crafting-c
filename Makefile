CC := gcc
CFLAGS := -g
#CFLAGS := -Wall -Wextra

all: main

main: main.c token.o scanner.o clox.h clox.o utils/hashmap.o
	$(CC) $(CFLAGS) main.c -o main scanner.o token.o clox.o utils/hashmap.o

