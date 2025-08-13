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

#endif //_AST_PRINTER_H
