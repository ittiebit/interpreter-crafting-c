#include <stdarg.h>
#include <stdio.h>

#include "./ast-printer.h"
#include "../clox/expr.h"

void printf_with_indent(size_t depth, char * fstring, ...);

void printf_with_indent(size_t depth, char * fstring, ...) {
    va_list args;
    va_start(args, fstring);

    for (size_t i = 0; i < depth*INDENT_DEPTH_INC; i++) {
        printf(" ");
    }

    vprintf(fstring, args);
    va_end(args);
}

void print_ast(Expr * expr, size_t depth) {
    // Print the syntax tree recursively DFS
    if (expr == NULL) {
        return;
    }

    switch (expr->expr_type) {
        case EXPR_LITERAL:
            print_literal_expr(expr, depth++);
            break;
        case EXPR_GROUPING:
            print_grouping_expr(expr, depth++);
            print_ast(((Grouping_expr*)expr->expr)->expr, depth++);
            break;
        case EXPR_UNARY:
            print_unary_expr(expr, depth++);
            print_ast(((Unary_expr*)expr->expr)->expr, depth++);
            break;
        case EXPR_BINARY:
            print_binary_expr(expr, depth++);
            print_ast(((Binary_expr*)expr->expr)->left, depth);
            print_ast(((Binary_expr*)expr->expr)->right, depth);
            break;
        case EXPR_OPERATOR:
            print_operator_expr(expr, depth++);
            // TODO later
            break;
        default:
            return;
    }
}

void print_binary_expr(Expr * expr, size_t depth) {
    Binary_expr * binary_expr = (Binary_expr*)expr->expr;

    printf_with_indent(depth, "[Binary] ");

    TokenType type = binary_expr->token->type;
    switch (type) {
        case MINUS:         printf("-\n"); break;
        case PLUS:          printf("+\n"); break;
        case SLASH:         printf("/\n"); break;
        case STAR:          printf("*\n"); break;
        case BANG_EQUAL:    printf("!=\n"); break;
        case EQUAL:         printf("=\n"); break;
        case EQUAL_EQUAL:   printf("==\n"); break;
        case GREATER:       printf(">\n"); break;
        case GREATER_EQUAL: printf(">=\n"); break;
        case LESS:          printf("<\n"); break;
        case LESS_EQUAL:    printf("<=\n"); break;
        default:            printf("undefined\n"); break;
    }

    //printf_with_indent(depth, "LEX: %s\n", binary_expr->token->lexeme);
    //printf_with_indent(depth, "LIT: NOT IMPL\n");
}

void print_unary_expr(Expr * expr, size_t depth) {
    Unary_expr * unary_expr = (Unary_expr*)expr->expr;
    TokenType type = unary_expr->token->type;

    printf_with_indent(depth, "[Unary] ");

    switch (type) {
        case BANG:          printf("!\n"); break;
        case MINUS:         printf("-\n"); break;
        default:            printf("undefined\n"); break;
    }

    //printf_with_indent(depth, "LEX: %s\n", unary_expr->token->lexeme);
    //printf_with_indent(depth, "LIT: NOT IMPL\n");
}

void print_literal_expr(Expr * expr, size_t depth) {
    Literal_expr * literal_expr = (Literal_expr*)expr->expr;
    Literal_type type = literal_expr->literal_type;

    printf_with_indent(depth, "[Literal] ");

    switch (type) {
        case LITERAL_TRUE:
            printf("true\n");
            break;
        case LITERAL_FALSE:
            printf("false\n");
            break;
        case LITERAL_NIL:
            printf("NIL\n");
            break;
        case LITERAL_STRING:
            printf("%s\n", (char*)literal_expr->value);
            break;
        case LITERAL_NUMBER:
            printf("%f\n", *(double*)literal_expr->value);
            break;
        default:
            printf("undefined\n");
            break;
    }
}

void print_grouping_expr(Expr * expr, size_t depth) {
    Grouping_expr * grouping_expr = (Grouping_expr*)expr->expr;
    TokenType type = grouping_expr->type;

    printf_with_indent(depth, "[Grouping] ");

    switch (type) {
        case LEFT_PAREN:
            printf("()\n");
            break;
        default:
            printf("undefined\n");
            break;
    }

    //printf_with_indent(depth, "LEX: %s\n", grouping_expr->expr);
    //printf_with_indent(depth, "LIT: NOT IMPL\n");
}

// TODO later
void print_operator_expr(Expr * expr, size_t depth) {
    printf_with_indent(depth, "[Operator]\n");
    return;
}

