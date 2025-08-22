#ifndef _AST_PRINTER_H
#define _AST_PRINTER_H

#include "../clox/expr.h"

#define INDENT_DEPTH_INC 4

void print_ast(Expr * expr, size_t depth);
void print_binary_expr(Expr * expr, size_t depth);
void print_unary_expr(Expr * expr, size_t depth);
void print_literal_expr(Expr * expr, size_t depth);
void print_grouping_expr(Expr * expr, size_t depth);
void print_operator_expr(Expr * expr, size_t depth);
void print_ternary_expr(Expr * expr, size_t depth);
void print_variable_expr(Expr * exp, size_t depthr);
void print_assign_expr(Expr * expr, size_t depth);

void print_single_stmt(Stmt * stmt, size_t depth);
void print_stmt_list(Stmt * stmt, size_t depth);
void print_print_stmt(PrintStmt * stmt, size_t depth);
void print_expr_stmt(ExprStmt * stmt, size_t depth);
void print_var_stmt(VarStmt * stmt, size_t depth);
void print_block_stmt(BlockStmt * stmt, size_t depth);

#endif //_AST_PRINTER_H
