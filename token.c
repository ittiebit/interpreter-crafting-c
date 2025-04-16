#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./token.h"

char * to_string(Token * token) {
    const char * type_desc = " type: ";
    const char * lexeme_desc = " lexeme: ";
    const char * literal_str = " literal: ";

    size_t allocate_len = 8; // type size
    allocate_len += strlen(type_desc);
    if (token->lexeme != NULL) {
        allocate_len += strlen(lexeme_desc);
        allocate_len += strlen(token->lexeme);
    }
    if (token->literal != NULL) {
        allocate_len += strlen(type_desc);
        if (token->type == STRING) {
            allocate_len += strlen(token->literal);
        } else if (token->type == NUMBER) {
            allocate_len += sizeof(double);
        }
    }
    allocate_len += 2;

    char * str = malloc(sizeof(char) * allocate_len);

    size_t len = 0;
    len += sprintf(str+len, "%s%d", type_desc, token->type);
    if (token->lexeme != NULL) {
        len += sprintf(str+len, "%s%s", lexeme_desc, token->lexeme);
    }
    if (token->literal != NULL) {
        if (token->type == STRING) {
            len += sprintf(str+len, "%s%s", literal_str, (char*)token->literal);
        } else if (token->type == NUMBER) {
            double * number = (double*)token->literal;
            len += sprintf(str+len, "%s%f", literal_str, *number);
        }
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
    ptoken->lexeme = NULL;
    ptoken->literal = NULL;

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

void free_token(Token * token) {
    if (token->lexeme != NULL) {
        free(token->lexeme);
    }
    if (token->literal != NULL) {
        free(token->literal);
    }
    free(token);
    return;
}
