#ifndef EXPR_H_
#define EXPR_H_

#include "./token.h"

typedef enum Expr_type_t {
    EXPR_LITERAL,
    EXPR_GROUPING,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_OPERATOR,
} Expr_type;

typedef struct Expr_t {
    Expr_type expr_type;
    void * expr;
} Expr;

/*
    expression     → literal
                   | unary
                   | binary
                   | grouping ;

    literal        → NUMBER | STRING | "true" | "false" | "nil" ;
    grouping       → "(" expression ")" ;
    unary          → ( "-" | "!" ) expression ;
    binary         → expression operator expression ;
    operator       → "==" | "!=" | "<" | "<=" | ">" | ">="
                   | "+"  | "-"  | "*" | "/" ;
*/

typedef enum Literal_type_t {
    LITERAL_NUMBER,
    LITERAL_STRING,
    LITERAL_TRUE,
    LITERAL_FALSE,
    LITERAL_NIL,
} Literal_type;

typedef struct Literal_expr_t {
    Literal_type literal_type;
    void * value;
} Literal_expr;

typedef struct Grouping_expr_t {
    Expr * expr;
} Grouping_expr;

typedef struct Unary_expr_t {
    Token * token;
    Expr * expr;
} Unary_expr;

typedef struct Binary_expr_t {
    Token * token;
    Expr * left;
    Expr * right;
} Binary_expr;

typedef struct Operator_expr_t {
    Token * token;
} Operator_expr_t;


Binary_expr * new_binary_expr(Expr * left, Token * op, Expr * right);
Unary_expr * new_unary_expr(Token * op, Expr * token);
Literal_expr * new_literal_expr(TokenType type, void * literal);
Grouping_expr * new_grouping_expr(Expr * expr);
void free_binary_expr(Binary_expr * expr);
void free_unary_expr(Unary_expr * expr);
void free_literal_expr(Literal_expr * expr);
void free_grouping_expr(Grouping_expr * expr);

#endif
