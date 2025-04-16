#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./token.h"

char * to_string(Token * token) {
    size_t allocate_len = 8;
    if (token->lexeme != NULL) {
        allocate_len += strlen(token->lexeme);
    }
    if (token->literal != NULL) {
        allocate_len += strlen(token->literal);
    }
    allocate_len += 2;

    char * str = malloc(sizeof(char) * allocate_len);

    size_t len = 0;
    len += sprintf(str+len, "%d", token->type);
    if (token->lexeme != NULL) {
        len += sprintf(str+len, " %s", token->lexeme);
    }
    if (token->literal != NULL) {
        len += sprintf(str+len, " %s", (char*)token->literal);
    }

    //printf("\ttype %d\n", token->type);
    //printf("\tlexeme %s\n", token->lexeme);

    return str;
}

Token * create_token(TokenType type, char * lexeme, void * literal, size_t line) {
    Token * ptoken = malloc(sizeof(Token));
    if (ptoken == NULL) {
        printf("Error: Failed to allocate token!\n");
        exit(3);
    }

    ptoken->type = type;

    if (lexeme != NULL) {
        ptoken->lexeme = malloc(sizeof(char) * strlen(lexeme));
        if (ptoken->lexeme == NULL) {
            printf("Error: Failed to allocate token->lexeme!\n");
            exit(3);
        }
        strcpy(ptoken->lexeme, lexeme);
    }

    if (literal != NULL) {
        ptoken->literal = malloc(sizeof(char) * strlen(literal));
        if (ptoken->literal == NULL) {
            printf("Error: Failed to allocate token->literal!\n");
            exit(3);
        }
        memcpy(ptoken->literal, literal, sizeof(literal));
    }

    ptoken->line = line;

    return ptoken;
}
