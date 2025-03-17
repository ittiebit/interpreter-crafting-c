#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./token.h"

char * to_string(Token * token) {
    size_t allocate_len = 8;
    allocate_len += strlen(token->lexeme);
    allocate_len += strlen(token->literal);
    allocate_len += 2;

    char * str = malloc(sizeof(char) * allocate_len);

    size_t len = 0;
    len += sprintf(str+len, "%d", token->type);
    len += sprintf(str+len, " %s", token->lexeme);
    len += sprintf(str+len, " %s", token->literal);

    return str;
}

Token * create_token(TokenType type, char * lexeme, char * literal, size_t line) {
    Token * ptoken = malloc(sizeof(Token));

    ptoken->type = type;

    ptoken->lexeme = malloc(sizeof(char) * strlen(lexeme));
    strcpy(ptoken->lexeme, lexeme);

    ptoken->literal = malloc(sizeof(char) * strlen(literal));
    strcpy(ptoken->literal, literal);

    ptoken->line = line;

    return ptoken;
}
