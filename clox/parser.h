#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include "./expr.h"
#include "./token.h"

typedef struct Parser_t {
    size_t current;
    Token ** tokens;
    char * p_errors;
} Parser;

Parser * create_parser(Token ** tokens, char * p_errors);
void free_parser(Parser * parser);
Stmt * parse(Parser * parser);

#endif //PARSER_H
