#ifndef PARSER_H_
#define PARSER_H_

#include <stddef.h>
#include "token.h"

typedef struct Parser_t {
    size_t current;
    Token ** tokens;
} Parser;

#endif // PARSER_H_
