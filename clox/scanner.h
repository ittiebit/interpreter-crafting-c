#ifndef SCANNER_H
#define SCANNER_H

#include <stddef.h>
#include "../utils/hashmap.h"
#include "./token.h"

char * to_string(Token * token);
Token * create_token(TokenType type, char * lexeme, void * literal, size_t line);
void free_token(Token * token);

#define FALSE 0
#define TRUE 1

typedef struct Scanner {
  char * source;
  Token ** tokens;
  size_t token_list_size;
  size_t start;
  size_t current;
  size_t line;
  char * p_errors;
  map_t * keyword_map;
} Scanner;

Scanner * create_scanner(char * source, char * p_errors);
void free_scanner(Scanner * scanner);
Token ** scan_tokens(Scanner * scanner);
void scan_token(Scanner * scanner);
int is_at_end(Scanner * scanner);
void add_token(Scanner * scanner, TokenType type, void * literal);
void string(Scanner * scanner);
char peek_next(Scanner * scanner);
void number(Scanner * scanner);
void identifier(Scanner * scanner);
int is_digit(char c);
int is_alpha(char c);
int is_alpha_numeric(char c);

#include "../utils/hashmap.h"
#define HASHMAP_SIZE 16
// For testing
extern int keys[HASHMAP_SIZE];
extern char * keys_str[HASHMAP_SIZE];
map_t create_keywords_hashmap();

#endif //SCANNER_H
