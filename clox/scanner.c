#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./scanner.h"
#include "./clox.h"
#include "../utils/hashmap.h"
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

map_t create_keywords_hashmap();

#define HASHMAP_SIZE 16

int keys[HASHMAP_SIZE] = {
        AND,        CLASS,      ELSE,   FALSE,  FOR,
        FUN,        IF,         NIL,    OR,     PRINT,
        RETURN,     SUPER,      THIS,   TRUE,   VAR,
        WHILE
    };

char * keys_str[HASHMAP_SIZE] = {
        "and",      "class",    "else", "false", "for",
        "fun",      "if",       "nil",  "or",    "print",
        "return",   "super",    "this", "true",  "var",
        "while"
    };

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
        fprintf(stderr, "ERROR in scanner.c - create_scanner(): pscan->tokens = malloc() failure\nexiting...\n");
        exit(1);
    }

    pscan->token_list_size = 0;

    pscan->start = 0;
    pscan->current = 0;
    pscan->line = 1;

    pscan->keyword_map = NULL;
    pscan->keyword_map = create_keywords_hashmap();

    if (pscan->keyword_map == NULL) {
        fprintf(stderr, "ERROR in scanner.c - create_scanner(): pscan->keyword_map = create_keywords_hashmap() failure\nexiting...\n");
        exit(1);
    }

    return pscan;
}

Token ** scan_tokens(Scanner * scanner) {
    map_t keyword_map = create_keywords_hashmap();

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

        //case 'o':
        //    if (match(scanner, 'r')) {
        //        add_token(scanner, OR, NULL);
        //    }
        //    break;

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
    return scanner->current >= strlen(scanner->source);
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
        printf("char: %c at %ld\n", scanner->source[scanner->current], scanner->current);
    }

    if (is_at_end(scanner)) {
        cerror(scanner->line, "Unterminated string.", NULL);
        return;
    }

    // The closing ".
    advance(scanner);

    size_t str_size = scanner->current-scanner->start-2; // why -2???????
    // Trim the surrounding quotes.
    char * value = malloc(str_size * sizeof(char));
    memcpy(value, scanner->source+scanner->start+1, str_size);

    printf("string value %s with size %ld\n", value, str_size);

    add_token(scanner, STRING, value);
}

void number(Scanner * scanner) {
    while (is_digit(peek(scanner))) {
        printf("char: %c at %ld\n", scanner->source[scanner->current], scanner->current);

        advance(scanner);
    }

    // Look for a fractional part.
    if (peek(scanner) == '.' && is_digit(peek_next(scanner))) {
        // Consume the "."
        printf("char: %c at %ld\n", scanner->source[scanner->current], scanner->current);
        advance(scanner);

        while (is_digit(peek(scanner))) {
            printf("char: %c at %ld\n", scanner->source[scanner->current], scanner->current);
            advance(scanner);
        }
    }

    printf("%ld\n", scanner->start);

    size_t number_str_size = scanner->current-scanner->start;
    char * number_str = malloc(number_str_size * sizeof(char));
    // memcpy(number_str, scanner->source+scanner->start, number_str_size);
    strcpy(number_str, scanner->source+scanner->start);


    double * val = malloc(sizeof(double));
    *val = atof(number_str);

    add_token(scanner, NUMBER, val);
}

void identifier(Scanner * scanner) {
    while (is_alpha_numeric(peek(scanner))) advance(scanner);

    char * text = malloc((scanner->current - scanner->start) * sizeof(char));
    memcpy(text, scanner->source+scanner->start, scanner->current-scanner->start);

    any_t any_type = NULL;
    any_t * pany_type = &any_type;
    int code = hashmap_get(scanner->keyword_map, text, pany_type);
    if (code == MAP_FULL) {
        printf("Error in scanner.c - identifier(): hashmap_get returned code %i. Hashmap is full.\n", code);
        exit(1);
    }
    if (code == MAP_OMEM) {
        printf("Error in scanner.c - identifier(): hashmap_get returned code %i. Out of memory.\n", code);
        exit(1);
    }

    TokenType type;

    if (any_type == NULL || code == MAP_MISSING) {
        type = IDENTIFIER;
    } else {
        type = *(TokenType*)any_type;
    }

    printf("Identifier '%s' has type %i\n", text, type);

    add_token(scanner, type, NULL);
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

map_t create_keywords_hashmap() {
    map_t keywords = hashmap_new();
    for (size_t i = 0; i < HASHMAP_SIZE; i++) {
        hashmap_put(keywords, keys_str[i], &keys[i]);
    }
    return keywords;
}

char * to_string(Token * token) {
    const char * type_desc = " type: ";
    const char * lexeme_desc = " lexeme: ";
    const char * literal_str = " literal: ";

    size_t allocate_len = 8; // type size
    allocate_len += strlen(type_desc);
    if (token->lexeme != NULL) {
        allocate_len += strlen(lexeme_desc);
        allocate_len += strlen(token->lexeme);
    }
    if (token->literal != NULL) {
        allocate_len += strlen(type_desc);
        if (token->type == STRING) {
            allocate_len += strlen(token->literal);
        } else if (token->type == NUMBER) {
            allocate_len += sizeof(double);
        }
    }
    allocate_len += 2;

    char * str = malloc(sizeof(char) * allocate_len);

    size_t len = 0;
    len += sprintf(str+len, "%s%d", type_desc, token->type);
    if (token->lexeme != NULL) {
        len += sprintf(str+len, "%s%s", lexeme_desc, token->lexeme);
    }
    if (token->literal != NULL) {
        if (token->type == STRING) {
            len += sprintf(str+len, "%s%s", literal_str, (char*)token->literal);
        } else if (token->type == NUMBER) {
            double * number = (double*)token->literal;
            len += sprintf(str+len, "%s%f", literal_str, *number);
        }
    }

    //printf("\ttype %d\n", token->type);
    //printf("\tlexeme %s\n", token->lexeme);

    return str;
}

Token * create_token(TokenType type, char * lexeme, void * literal, size_t line) {
    Token * ptoken = malloc(sizeof(Token));
    if (ptoken == NULL) {
        printf("Error: Failed to allocate token!\n");
        exit(3);
    }
    ptoken->lexeme = NULL;
    ptoken->literal = NULL;

    ptoken->type = type;

    if (lexeme != NULL) {
        ptoken->lexeme = malloc(sizeof(char) * strlen(lexeme));
        if (ptoken->lexeme == NULL) {
            printf("Error: Failed to allocate token->lexeme!\n");
            exit(3);
        }
        strcpy(ptoken->lexeme, lexeme);
    }

    if (literal != NULL) {
        ptoken->literal = malloc(sizeof(char) * strlen(literal));
        if (ptoken->literal == NULL) {
            printf("Error: Failed to allocate token->literal!\n");
            exit(3);
        }
        memcpy(ptoken->literal, literal, sizeof(literal));
    }

    ptoken->line = line;

    return ptoken;
}

void free_token(Token * token) {
    if (token->lexeme != NULL) {
        free(token->lexeme);
    }
    if (token->literal != NULL) {
        free(token->literal);
    }
    free(token);
    return;
}
