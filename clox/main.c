#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "./scanner.h"
#include "./definitions.h"
#include "./parser.h"
#include "./expr.h"
#include "./types.h"
#include "./interpreter.h"
#include "./error-type.h"

#ifdef DEBUG
#include "../utils/ast-printer.h"
#endif

void run_prompt();
void run_file(char * path);
Clox_errors run(char * source);

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

Clox_errors run(char * source) {
    boolean had_scan_error = VALUE_FALSE;
    boolean had_parse_error = VALUE_FALSE;
    boolean had_runtime_error = VALUE_FALSE;

    Clox_errors errors = {
        .scan_errors = &had_scan_error,
        .parse_errors = &had_parse_error,
        .runtime_errors = &had_runtime_error,
    };

    Scanner * scanner = create_scanner(source, errors.scan_errors);
    Token ** tokens = scan_tokens(scanner);
    free_scanner(scanner);

    if (tokens[0] == NULL || tokens[0]->type == EOFF) {
        return errors;
    }

    Parser * parser = create_parser(tokens, errors.parse_errors);
    Expr * ast = parse(parser);

    /* DEBUGGING */
    #ifdef DEBUG
    print_ast(ast, 0);

    if (*errors.scan_errors != VALUE_FALSE) {
        fprintf(stderr, "[DEBUG] had_scan_error flag set!\n");
    }
    if (*errors.parse_errors != VALUE_FALSE) {
        fprintf(stderr, "[DEBUG] had_parse_error flag set!\n");
    }
    #endif
    /*************/

    free_parser(parser);

    Interpreter * inter = create_interpreter(errors.runtime_errors);
    interpret(inter, ast);

    #ifdef DEBUG
    if (*errors.runtime_errors != VALUE_FALSE) {
        fprintf(stderr, "[DEBUG] had_scan_error flag set!\n");
    }
    #endif

    for (int i = 0; i < SOURCE_BUF_SIZE; i++) {
        if (tokens[i] == NULL) {
            return errors;
        }
        free_token(tokens[i]);
        tokens[i] = NULL;
    }
    free_ast(ast);
    free_interpreter(inter);

    return errors;
}

void run_file(char * path) {
    FILE * fptr = fopen(path, "rb");

    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    char * bytes = malloc(sizeof(char)*size);

    fread(bytes, size, sizeof(char), fptr);

    Clox_errors errors = run(bytes);
    if (*errors.scan_errors || *errors.parse_errors) {
        exit(65);
    }
    if (*errors.runtime_errors) {
        exit(70);
    }
}
