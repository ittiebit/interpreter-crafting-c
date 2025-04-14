#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "./token.h"
#include "./scanner.h"
#include "./definitions.h"

void run_prompt();
void run(char * source);
void run_file(char * path);
void error(size_t line, char * message, int * had_error);
void report(size_t line, char * where, char * message, int * had_error);

int main(int argc, char ** argv) {
    if (argc > 2) {
        printf("Usage: clox [script]");
        exit(EXIT_FAILURE);
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }

    return 0;
}

void run_prompt() {

    while (1) {
        printf("> ");
        char line[SOURCE_BUF_SIZE];
        fgets(line, sizeof(line), stdin);
        if (line[0] == '\n') continue;
        run(line);
    }

    return;
}

void run(char * source) {
    Scanner * scanner = create_scanner(source);
    Token ** tokens = scan_tokens(scanner);

    // For now, just print the tokens.
    for (int i = 0; i < 100; i++) {
        printf("%s", to_string(tokens[i]));
    }

    return;
}

void run_file(char * path) {
    FILE * fptr = fopen(path, "rb");

    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    char * bytes = malloc(sizeof(char)*size);

    fread(bytes, size, 1, fptr);

    run(bytes);
}
