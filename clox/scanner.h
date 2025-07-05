#ifndef SCANNER_H_
#define SCANNER_H_

#include <stddef.h>
#include "../utils/hashmap.h"
#include "./token.h"

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
