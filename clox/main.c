#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./scanner.h"
#include "./definitions.h"
#include "./parser.h"
#include "../utils/ast-printer.h"
#include "expr.h"

void run_prompt();
void run(char * source);
void run_file(char * path);

int main(int argc, char ** argv) {
    if (argc > 2) {
        printf("Usage: clox [script]\n");
        exit(EXIT_FAILURE);
    } else if (argc == 2) {
        printf("Running file %s...\n", argv[1]);
        run_file(argv[1]);
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
    char had_error = 0;
    char * p_errors = &had_error;

    Scanner * scanner = create_scanner(source, p_errors);
    Token ** tokens = scan_tokens(scanner);

    // // Print the tokens.
    // for (int i = 0; i < SOURCE_BUF_SIZE; i++) {
    //     if (tokens[i] == NULL) {
    //         return;
    //     }
    //     printf("Token: %s\n", to_string(tokens[i]));
    // }

    Parser * parser = create_parser(tokens, p_errors);
    Expr * expr = parse(parser);

    /* DEBUGGING */

    print_ast(expr, 0);

    /*************/

    if (had_error != 0) {
        fprintf(stderr, "had_error flag set!\n");
    }


    for (int i = 0; i < SOURCE_BUF_SIZE; i++) {
        if (tokens[i] == NULL) {
            return;
        }
        free_token(tokens[i]);
    }
    free_scanner(scanner);
    free_parser(parser);
    free_ast(expr);

    return;
}

void run_file(char * path) {
    FILE * fptr = fopen(path, "rb");

    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    char * bytes = malloc(sizeof(char)*size);

    fread(bytes, size, sizeof(char), fptr);

    run(bytes);
}
