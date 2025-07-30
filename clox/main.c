#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./scanner.h"
#include "./definitions.h"
#include "./parser.h"

void run_prompt();
void run(char * source);
void run_file(char * path);

int main(int argc, char ** argv) {
    if (argc > 2) {
        printf("Usage: clox [script]");
        exit(EXIT_FAILURE);
    } else if (argc == 2) {
        printf("%s\n", argv[1]);
        //run_file(argv[1]);
    } else {
        run_prompt();
    }

    return 0;
}

void run_prompt() {
    char line[SOURCE_BUF_SIZE];

    int interactive = isatty(fileno(stdin));

    if (!interactive) {
        fgets(line, sizeof(line), stdin);
        run(line);
        return;
    }

    while (interactive) {
        printf("> ");
        fgets(line, sizeof(line), stdin);
        if (line[0] == '\n') continue;
        run(line);
    }
    return;
}

void run(char * source) {
    Scanner * scanner = create_scanner(source);
    Token ** tokens = scan_tokens(scanner);

    Parser * parser = create_parser(tokens);
    Expr * expr = parse(parser);

    // // For now, just print the tokens.
    // for (int i = 0; i < 100; i++) {
    //     if (tokens[i] == NULL) {
    //         return;
    //     }
    //     printf("Token: %s\n", to_string(tokens[i]));
    // }

    for (int i = 0; i < 100; i++) {
        if (tokens[i] == NULL) {
            return;
        }
        free_token(tokens[i]);
    }

    free_parser(parser);

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
