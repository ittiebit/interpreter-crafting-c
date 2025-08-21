#ifndef EXPR_H
#define EXPR_H

#include "./token.h"

typedef enum ExprType_t {
    EXPR_LITERAL,
    EXPR_GROUPING,
    EXPR_UNARY,
    EXPR_BINARY,
    EXPR_TERNARY,
    EXPR_OPERATOR,
    EXPR_VARIABLE,
    EXPR_ASSIGN,
} ExprType;

typedef struct Expr_t {
    ExprType expr_type;
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

typedef enum LiteralType_t {
    LITERAL_NUMBER = 0,
    LITERAL_STRING,
    LITERAL_TRUE,
    LITERAL_FALSE,
    LITERAL_NIL,
} LiteralType;

typedef struct LiteralExpr_t {
    LiteralType literal_type;
    void * value;
} LiteralExpr;

typedef struct GroupingExpr_t {
    Expr * expr;
    TokenType type;
} GroupingExpr;

typedef struct UnaryExpr_t {
    Token * token;
    Expr * expr;
} UnaryExpr;

typedef struct BinaryExpr_t {
    Token * token;
    Expr * left;
    Expr * right;
} BinaryExpr;

typedef struct TernaryExpr_t {
    Token * token_left;
    Token * token_right;
    Expr * left;
    Expr * mid;
    Expr * right;
} TernaryExpr;

typedef struct OperatorExpr_t {
    Token * token;
} OperatorExpr;

typedef struct VariableExpr_t {
    Token * name;
} VariableExpr;

typedef struct AssignExpr_t {
    Token * name;
    Expr * value;
} AssignExpr;

Expr * new_binary_expr(Expr * left, Token * op, Expr * right);
Expr * new_unary_expr(Token * op, Expr * right);
Expr * new_ternary_expr(Expr * left, Token * op_left, Expr * mid, Token * op_right, Expr * right);
Expr * new_literal_expr(TokenType type, void * literal);
Expr * new_grouping_expr(TokenType type, Expr * expr);
Expr * new_variable_expr(Token * token);
Expr * new_assign_expr(Token * name, Expr * value);
void free_binary_expr(Expr * expr);
void free_unary_expr(Expr * expr);
void free_ternary_expr(Expr * expr);
void free_literal_expr(Expr * expr);
void free_grouping_expr(Expr * expr);
void free_variable_expr(Expr * expr);
void free_assign_expr(Expr * expr);
void free_ast(Expr * expr);


/*
*   Statements
*/

typedef enum StmtType_t {
    EXPR_STMT,
    PRINT_STMT,
    VAR_STMT,
    ASSIGN_STMT,
    BLOCK_STMT,
} StmtType;

typedef struct Stmt_t {
    StmtType stmt_type;
    void * stmt;
    struct Stmt_t * next_stmt;
    struct Stmt_t * prev_stmt;
} Stmt;

typedef struct PrintStmt_t {
    Expr * expr;
} PrintStmt;

typedef struct ExprStmt_t {
    Expr * expr;
} ExprStmt;

typedef struct VarStmt_t {
    Token * name;
    Expr * initializer;
} VarStmt;

typedef struct BlockStmt_t {
    Stmt * head_stmt; //linked list of statements
} BlockStmt;

//Stmt * new_stmt(StmtType stmt_type, Expr * expr);
void free_stmt(Stmt * stmt);
void add_stmt(Stmt ** pp_head_stmt, Stmt * new_stmt);
void free_stmt_list(Stmt * stmt);

Stmt * new_print_stmt(Expr * expr);
Stmt * new_expr_stmt(Expr * expr);
Stmt * new_var_stmt(Token * name, Expr * initializer);
Stmt * new_block_stmt(Stmt * stmt);
void free_print_stmt(PrintStmt * stmt);
void free_expr_stmt(ExprStmt * stmt);
void free_var_stmt(VarStmt * stmt);
void free_block_stmt(BlockStmt * stmt);

#endif //EXPR_H
