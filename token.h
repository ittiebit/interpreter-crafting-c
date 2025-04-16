#ifndef TOKEN_H_
#define TOKEN_H_

#include <stddef.h>
#include "./token_types.h"

typedef struct Token {
    TokenType type;
    char * lexeme;
    void * literal; //??? what type
    size_t line;
} Token;

char * to_string(Token * token);
Token * create_token(TokenType type, char * lexeme, void * literal, size_t line);
void free_token(Token * token);

#endif //TOKEN_H_
