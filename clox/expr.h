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
} Literal_expr;

typedef struct Grouping_expr_t {
    Expr * expr;
} Grouping_expr;

typedef struct Unary_expr_t {
    Token token;
    Expr * expr;
} Unary_expr;

typedef struct Binary_expr_t {
    Token token;
    Expr * left;
    Expr * right;
} Binary_expr;

typedef struct Operator_expr_t {
    Token token;
} Operator_expr_t;

#endif
