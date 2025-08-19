#include "./clox.h"
#include "./token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void report(size_t line, char * where, char * message, char * p_had_error) {
    fprintf(stderr, "[line %ld] Error %s: %s\n", line, where, message);

    if (p_had_error != NULL) {
        memset(p_had_error, 1, sizeof(char));
    } else {
        fprintf(stderr, "INTERNAL ERROR clox.c - report(): char * had_error is NULL\n");
    }
    return;
}

void scan_error(size_t line, char * message, char * p_had_error) {
    report(line, "", message, p_had_error);
    return;
}

void cerror(Token * token, char * message, char * p_had_error) {
    if (token->type == EOFF) {
        report(token->line, " at end", message, p_had_error);
    } else {
        char * where_str;
        if ((where_str = malloc(8 * sizeof(char) + strlen(token->lexeme))) != NULL) {
            where_str[0] = '\0';
            strcat(where_str, " at '");
            strcat(where_str, token->lexeme);
            strcat(where_str, "'");
        }

        report(token->line, where_str, message, p_had_error);
    }
    return;
}
