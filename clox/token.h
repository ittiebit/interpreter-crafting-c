#ifndef TOKEN_H_
#define TOKEN_H_

#include <stddef.h>

#define NULL_CHAR '\0'
typedef enum TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  EOFF
} TokenType;

typedef struct Token_t {
    TokenType type;
    char * lexeme;
    void * literal; //??? what type
    size_t line;
} Token;

#endif // TOKEN_H_
