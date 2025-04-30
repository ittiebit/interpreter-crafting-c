#ifndef SCANNER_H_
#define SCANNER_H_

#include "hashmap.h"
#include <stddef.h>

// Tokens

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

typedef struct Token {
    TokenType type;
    char * lexeme;
    void * literal; //??? what type
    size_t line;
} Token;

char * to_string(Token * token);
Token * create_token(TokenType type, char * lexeme, void * literal, size_t line);
void free_token(Token * token);

#define SCANNER_TOKEN_LIST_BEGIN_SIZE 100

#define FALSE 0
#define TRUE 1

typedef struct Scanner {
  char * source;
  Token ** tokens;
  size_t token_list_size;
  size_t start;
  size_t current;
  size_t line;
  char had_error;
  map_t * keyword_map;
} Scanner;

Scanner * create_scanner(char * source);
Token ** scan_tokens(Scanner * scanner);
void scan_token(Scanner * scanner);
int is_at_end(Scanner * scanner);
void add_token(Scanner * scanner, TokenType type, void * literal);
char advance(Scanner * scanner);
int match(Scanner * scanner, char expected);
char peek(Scanner * scanner);
void string(Scanner * scanner);
char peek_next(Scanner * scanner);
void number(Scanner * scanner);
void identifier(Scanner * scanner);
int is_digit(char c);
int is_alpha(char c);
int is_alpha_numeric(char c);

#endif //SCANNER_H_
