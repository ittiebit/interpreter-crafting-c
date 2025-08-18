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

typedef struct CloxCtx_t {
    Interpreter * inter;
    Parser * parser;
    Scanner * scanner;
    Token ** tokens;
    CloxErrors errors;
} CloxCtx;

void run_prompt();
void run_file(char * path);
void run(char * source, CloxCtx * ctx);

CloxCtx * create_ctx(char * source);
void free_ctx(CloxCtx * ctx);
void free_token_list(Token ** tokens);

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

    CloxCtx * ctx = create_ctx(NULL);

    int interactive = isatty(fileno(stdin));

    if (!interactive) {
        fgets(line, sizeof(line), stdin);
        run(line, ctx);
        free_ctx(ctx);
        return;
    }

    while (interactive) {
        printf("> ");
        fgets(line, sizeof(line), stdin);
        if (line[0] == '\n') continue;
        run(line, ctx);
    }
    free_ctx(ctx);
    return;
}

void run(char * source, CloxCtx * ctx) {
    if (ctx == NULL) {
        return;
    }
    ctx->errors.scan_errors = VALUE_FALSE;
    ctx->errors.parse_errors = VALUE_FALSE;
    ctx->errors.runtime_errors = VALUE_FALSE;

    if (ctx->scanner == NULL) {
        ctx->scanner = create_scanner(source, &ctx->errors.scan_errors);
    } else {
        ctx->scanner->source = source;
    }
    ctx->scanner->tokens = scan_tokens(ctx->scanner);
    ctx->tokens = ctx->scanner->tokens;

    if (ctx->tokens[0] == NULL || ctx->tokens[0]->type == EOFF) {
        return;
    }

    if (ctx->parser == NULL) {
        ctx->parser = create_parser(ctx->tokens, &ctx->errors.parse_errors);
    } else {
        ctx->parser->tokens = ctx->tokens;
    }
    Stmt * stmt = parse(ctx->parser);

    if (ctx->errors.parse_errors != VALUE_FALSE || ctx->errors.scan_errors != VALUE_FALSE) {
        return;
    }

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

    interpret(ctx->inter, stmt);

    #ifdef DEBUG
    if (*errors.runtime_errors != VALUE_FALSE) {
        fprintf(stderr, "[DEBUG] had_scan_error flag set!\n");
    }
    #endif

    free_token_list(ctx->tokens);
    free_scanner(ctx->scanner);
    ctx->scanner = NULL;
    free_parser(ctx->parser);
    ctx->parser = NULL;

    return;
}

void run_file(char * path) {
    FILE * fptr = fopen(path, "rb");

    fseek(fptr, 0L, SEEK_END);
    size_t size = ftell(fptr);
    rewind(fptr);

    char * bytes = malloc(sizeof(char)*size);

    fread(bytes, size, sizeof(char), fptr);

    CloxCtx * ctx = create_ctx(bytes);
    run(bytes, ctx);

    free_ctx(ctx);
    if (ctx->errors.scan_errors || ctx->errors.parse_errors) {
        exit(65);
    }
    if (ctx->errors.runtime_errors) {
        exit(70);
    }
}

CloxCtx * create_ctx(char * source) {
    CloxCtx * ctx = malloc(sizeof(CloxCtx));
    if (ctx == NULL) {
        exit(1);
    }

    CloxErrors errors = {
        .scan_errors = VALUE_FALSE,
        .parse_errors = VALUE_FALSE,
        .runtime_errors = VALUE_FALSE,
    };
    ctx->errors = errors;

    ctx->scanner = create_scanner(source, &ctx->errors.scan_errors);
    if (ctx->scanner == NULL) {
        exit(1);
    }

    ctx->tokens = ctx->scanner->tokens;
    if (ctx->tokens == NULL) {
        exit(1);
    }

    ctx->inter = create_interpreter(&ctx->errors.runtime_errors);
    if (ctx->inter == NULL) {
        exit(1);
    }

    ctx->parser = create_parser(ctx->tokens, &ctx->errors.parse_errors);
    if (ctx->parser == NULL) {
        exit(1);
    }

    return ctx;
}

void free_ctx(CloxCtx * ctx) {
    if (ctx == NULL) {
        return;
    }

    ctx->errors.parse_errors = 0;
    ctx->errors.scan_errors = 0;
    ctx->errors.runtime_errors = 0;

    if (ctx->scanner != NULL) {
        free_scanner(ctx->scanner);
        ctx->scanner = NULL;
    }

    free_token_list(ctx->tokens);
    ctx->tokens = NULL;

    if (ctx->inter == NULL) {
        free_interpreter(ctx->inter);
        ctx->inter = NULL;
    }

    if (ctx->parser == NULL) {
        free_parser(ctx->parser);
        ctx->parser = NULL;
    }
}

void free_token_list(Token ** tokens) {
    if (tokens != NULL) {
        for (int i = 0; i < SOURCE_BUF_SIZE; i++) {
            if (tokens[i] == NULL) {
                return;
            }
            //free_token(tokens[i]);
            tokens[i] = NULL;
        }
    }
}
