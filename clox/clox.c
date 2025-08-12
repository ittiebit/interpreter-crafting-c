#include "clox.h"
#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void report(size_t line, char * where, char * message, char * had_error) {
    printf("[line %ld] Error %s: %s\n", line, where, message);

    if (had_error != NULL) {
        *had_error = 1;
    } else {
        fprintf(stderr, "ERROR clox.c - report(): char * had_error is NULL\n");
    }
    return;
}

void scan_error(size_t line, char * message, char * had_error) {
    report(line, "", message, had_error);
    return;
}

void cerror(Token * token, char * message, char * p_errors) {
    if (token->type == EOFF) {
        report(token->line, " at end", message, NULL);
    } else {
        char * where_str;
        if ((where_str = malloc(8 * sizeof(char) + strlen(token->lexeme))) != NULL) {
            where_str[0] = '\0';
            strcat(where_str, " at '");
            strcat(where_str, token->lexeme);
            strcat(where_str, "'");
        }

        report(token->line, where_str, message, p_errors);
    }
    return;
}
