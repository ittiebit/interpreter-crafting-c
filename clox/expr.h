#ifndef EXPR_H
#define EXPR_H

#include "./token.h"

typedef enum Expr_type_t {
    EXPR_LITERAL,
    EXPR_GROUPING,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_TERNARY,
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
                   | "+"  | "-"  | "*" | "/" | ",";
*/

typedef enum Literal_type_t {
    LITERAL_NUMBER = 0,
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
    TokenType type;
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

typedef struct Ternary_expr_t {
    Token * token_left;
    Token * token_right;
    Expr * left;
    Expr * mid;
    Expr * right;
} Ternary_expr;

typedef struct Operator_expr_t {
    Token * token;
} Operator_expr_t;


Expr * new_binary_expr(Expr * left, Token * op, Expr * right);
Expr * new_unary_expr(Token * op, Expr * right);
Expr * new_ternary_expr(Expr * left, Token * op_left, Expr * mid, Token * op_right, Expr * right);
Expr * new_literal_expr(TokenType type, void * literal);
Expr * new_grouping_expr(TokenType type, Expr * expr);
void free_binary_expr(Expr * expr);
void free_unary_expr(Expr * expr);
void free_ternary_expr(Expr * expr);
void free_literal_expr(Expr * expr);
void free_grouping_expr(Expr * expr);
void free_ast(Expr * expr);


/*
*   Statements
*/

typedef enum Stmt_type_t {
    EXPR_STMT,
    PRINT_STMT,
} Stmt_type;

typedef struct Stmt_t {
    Stmt_type stmt_type;
    Expr * expr;
    struct Stmt_t * next_stmt;
    struct Stmt_t * prev_stmt;
} Stmt;

Stmt * new_stmt(Stmt_type stmt_type, Expr * expr);
void free_stmt(Stmt * stmt);
void add_stmt(Stmt * head_stmt, Stmt * new_stmt);
void free_stmt_list(Stmt * stmt);

#endif //EXPR_H
