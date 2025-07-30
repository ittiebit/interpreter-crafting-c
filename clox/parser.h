#ifndef PARSER_H_
#define PARSER_H_

#include <stddef.h>
#include "expr.h"
#include "token.h"

typedef struct Parser_t {
    size_t current;
    Token ** tokens;
} Parser;

Parser * create_parser(Token ** tokens);
void free_parser(Parser * parser);
Expr * parse(Parser * parser);

#endif // PARSER_H_
