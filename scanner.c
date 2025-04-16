#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./scanner.h"
#include "./token.h"
#include "./token_types.h"
#include "./clox.h"
#include "definitions.h"

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

Scanner * alloc_null_scanner() {
    Scanner * pscan = malloc(sizeof(Scanner));
    if (pscan == NULL) {
        fprintf(stderr, "ERROR in scanner.c - alloc_null_scanner(): malloc() failure\nexiting...\n");
        exit(1);
    }

    pscan->source = NULL;
    pscan->tokens = NULL;

    return pscan;
}

Scanner * create_scanner(char * source) {
    Scanner * pscan = alloc_null_scanner();

    if (pscan == NULL) {
        exit(1);
    }

    //pscan->source = malloc(SOURCE_BUF_SIZE * sizeof(char));
    //memcpy(pscan->source, source, SOURCE_BUF_SIZE * sizeof(char));

    pscan->source = source;

    pscan->tokens = malloc(sizeof(Token *) * SCANNER_TOKEN_LIST_BEGIN_SIZE);
    if (pscan->tokens == NULL) {
        fprintf(stderr, "ERROR in scanner.c - create_scanner(): malloc() failure\nexiting...\n");
        exit(1);
    }

    pscan->token_list_size = 0;

    pscan->start = 0;
    pscan->current = 0;
    pscan->line = 1;

    return pscan;
}

Token ** scan_tokens(Scanner * scanner) {
    while (!is_at_end(scanner)) {
        scanner->start = scanner->current;
        scan_token(scanner);
    }

    ++scanner->token_list_size;
    *(scanner->tokens+scanner->token_list_size-1) = create_token(EOFF, "", NULL, scanner->line);

    return scanner->tokens;
}

void scan_token(Scanner * scanner) {
    char c = advance(scanner);

    if (c == NULL_CHAR) {
        printf("NULL_CHAR\n");
        return;
    }

    //printf("scanning token: %c\n", c);
    switch (c) {
        case '(': add_token(scanner, LEFT_PAREN, NULL); break;
        case ')': add_token(scanner, RIGHT_PAREN, NULL); break;
        case '{': add_token(scanner, LEFT_BRACE, NULL); break;
        case '}': add_token(scanner, RIGHT_BRACE, NULL); break;
        case ',': add_token(scanner, COMMA, NULL); break;
        case '.': add_token(scanner, DOT, NULL); break;
        case '-': add_token(scanner, MINUS, NULL); break;
        case '+': add_token(scanner, PLUS, NULL); break;
        case ';': add_token(scanner, SEMICOLON, NULL); break;
        case '*': add_token(scanner, STAR, NULL); break; 

        case '!': add_token(scanner, match(scanner, '=') ? BANG_EQUAL : BANG, NULL); break;
        case '=': add_token(scanner, match(scanner, '=') ? EQUAL_EQUAL : EQUAL, NULL); break;
        case '<': add_token(scanner, match(scanner, '=') ? LESS_EQUAL : LESS, NULL); break;
        case '>': add_token(scanner, match(scanner, '=') ? GREATER_EQUAL : GREATER, NULL); break;

        case '"': string(scanner); break;

        case '/':
            if (match(scanner, '/')) {
                // A comment goes until the end of the line.
                while (peek(scanner) != '\n' && !is_at_end(scanner)) advance(scanner);
            } else {
                add_token(scanner, SLASH, NULL);
            }
            break;

        case ' ':
            //printf("!!! space\n");
        case '\r':
            //printf("!!! \\r\n");
        case '\t':
            //printf("!!! \\t\n");
            // Ignore whitespace.
            break;
        case '\n':
            //printf("!!! \\n\n");
            ++(scanner->line);
            break;

        case 'o':
            if (match(scanner, 'r')) {
                add_token(scanner, OR, NULL);
            }
            break;

        default:
            if (is_digit(c)) {
                //printf("Unexptected digit: %c\n", c);
                number(scanner);
            } else if (is_alpha(c)) {
                //printf("Unexptected alpha: %c\n", c);
                identifier(scanner);
            } else {
                printf("Unexpected character: %c\n", c);
                cerror(scanner->line, "Unexpected character.", &scanner->had_error);
                return;
            }
            break;
    }
    return;
}

int is_at_end(Scanner * scanner) {
    return scanner->current >= strlen(scanner->source); //is this -1 bad?
}

char advance(Scanner * scanner) {
    //printf("Advancing current... %ld\n", scanner->current);

    size_t cur_char_pos = scanner->current;
    char cur_char = scanner->source[cur_char_pos];

    // At end of line string
    if (cur_char_pos >= strlen(scanner->source) || cur_char == '\0') {
        return NULL_CHAR;
    }

    scanner->current++;

    return cur_char;
}

void add_token(Scanner * scanner, TokenType type, void * literal) {
    char * text = malloc((scanner->current - scanner->start) * sizeof(char));

    memcpy(text, scanner->source+scanner->start, scanner->current-scanner->start);

    ++scanner->token_list_size;
    *(scanner->tokens+scanner->token_list_size-1) = create_token(type, text, literal, scanner->line);
}

int match(Scanner * scanner, char expected) {
    if (is_at_end(scanner)) return FALSE;

    if (scanner->source[scanner->current] != expected) return FALSE;

    ++scanner->current;
    return TRUE;
  }


char peek(Scanner * scanner) {
    if (is_at_end(scanner)) return '\0';

    return scanner->source[scanner->current];
}

void string(Scanner * scanner) {
    while (peek(scanner) != '"' && !is_at_end(scanner)) {
        if (peek(scanner) == '\n') scanner->line++;
        advance(scanner);
    }

    if (is_at_end(scanner)) {
        cerror(scanner->line, "Unterminated string.", NULL);
        return;
    }

    // The closing ".
    advance(scanner);

    size_t str_size = scanner->current-scanner->start-1;
    // Trim the surrounding quotes.
    char * value = malloc(str_size * sizeof(char));
    memcpy(value, scanner->source+scanner->start+1, str_size);
    add_token(scanner, STRING, value);
}

void number(Scanner * scanner) {
    while (is_digit(peek(scanner))) advance(scanner);

    // Look for a fractional part.
    if (peek(scanner) == '.' && is_digit(peek_next(scanner))) {
      // Consume the "."
      advance(scanner);

      while (is_digit(peek(scanner))) advance(scanner);
    }

    size_t number_str_size = scanner->current-scanner->start;
    char * number_str = malloc(number_str_size * sizeof(char));

    memcpy(number_str, scanner->source+scanner->start, number_str_size);

    double val = atof(number_str);

    add_token(scanner, NUMBER, &val);
}

void identifier(Scanner * scanner) {
    while (is_alpha_numeric(peek(scanner))) advance(scanner);

    add_token(scanner, IDENTIFIER, NULL);
}

char peek_next(Scanner * scanner) {
    if (scanner->current + 1 >= strlen(scanner->source)) return '\0';
    return scanner->source[scanner->current + 1];
}

int is_digit(char c) {
    return c >= '0' && c <= '9';
}

int is_alpha(char c) {
    return (c >= 'a' && c <= 'z') ||
    (c >= 'A' && c <= 'Z') ||
    c == '_';
}

int is_alpha_numeric(char c) {
    return is_alpha(c) || is_digit(c);
}
