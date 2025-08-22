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
            print_ast(((GroupingExpr*)expr->expr)->expr, depth++);
            break;
        case EXPR_UNARY:
            print_unary_expr(expr, depth++);
            print_ast(((UnaryExpr*)expr->expr)->expr, depth++);
            break;
        case EXPR_BINARY:
            print_binary_expr(expr, depth++);
            print_ast(((BinaryExpr*)expr->expr)->left, depth);
            print_ast(((BinaryExpr*)expr->expr)->right, depth);
            break;
        case EXPR_TERNARY:
            print_ternary_expr(expr, depth++);
            print_ast(((TernaryExpr*)expr->expr)->left, depth);
            print_ast(((TernaryExpr*)expr->expr)->mid, depth);
            print_ast(((TernaryExpr*)expr->expr)->right, depth);
            break;
        case EXPR_OPERATOR:
            print_operator_expr(expr, depth++);
            // TODO later
            break;
        case EXPR_ASSIGN:
            print_assign_expr(expr, depth++);
            break;
        case EXPR_VARIABLE:
            print_variable_expr(expr, depth++);
            break;
        default:
            return;
    }
}

void print_binary_expr(Expr * expr, size_t depth) {
    BinaryExpr * binary_expr = (BinaryExpr*)expr->expr;

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
        case COMMA:         printf(",\n"); break;
        default:            printf("undefined\n"); break;
    }

    //printf_with_indent(depth, "LEX: %s\n", binary_expr->token->lexeme);
    //printf_with_indent(depth, "LIT: NOT IMPL\n");
}

void print_ternary_expr(Expr * expr, size_t depth) {
    TernaryExpr * ternary_expr = (TernaryExpr*)expr->expr;

    printf_with_indent(depth, "[Ternary] ");

    TokenType type_left = ternary_expr->token_left->type;
    TokenType type_right = ternary_expr->token_right->type;
    if (type_left == QUESTION && type_right == COLON) {
        printf("? :\n");
    } else {
        printf("undefined\n");
    }

    //printf_with_indent(depth, "LEX: %s\n", binary_expr->token->lexeme);
    //printf_with_indent(depth, "LIT: NOT IMPL\n");
}

void print_unary_expr(Expr * expr, size_t depth) {
    UnaryExpr * unary_expr = (UnaryExpr*)expr->expr;
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
    LiteralExpr * literal_expr = (LiteralExpr*)expr->expr;
    LiteralType type = literal_expr->literal_type;

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
            printf("'%s'\n", (char*)literal_expr->value);
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
    GroupingExpr * grouping_expr = (GroupingExpr*)expr->expr;
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


void print_variable_expr(Expr * expr, size_t depth) {
    VariableExpr * variable_expr = (VariableExpr*)expr->expr;

    printf_with_indent(depth, "[Variable] ");
    printf("'%s'\n", variable_expr->name->lexeme);
}

void print_assign_expr(Expr * expr, size_t depth) {
    AssignExpr * assign_expr = (AssignExpr*)expr->expr;

    printf_with_indent(depth, "[Assign] ");
    printf("'%s'\n", assign_expr->name->lexeme);
    print_ast(assign_expr->value, depth+1);
}

void print_stmt_list(Stmt * head_stmt, size_t depth) {
    if (head_stmt == NULL) {
        return;
    }

    Stmt * cur_stmt = head_stmt;

    print_single_stmt(cur_stmt, depth);
    if (cur_stmt->next_stmt != NULL && cur_stmt->next_stmt != head_stmt) {
        cur_stmt = cur_stmt->next_stmt;
    }
    while (cur_stmt != head_stmt) {
        print_single_stmt(cur_stmt, depth);
        cur_stmt = cur_stmt->next_stmt;
    }
}

void print_single_stmt(Stmt * stmt, size_t depth) {
    if (stmt == NULL) {
        return;
    }
    switch (stmt->stmt_type) {
        case PRINT_STMT:
            print_print_stmt(((PrintStmt*)stmt->stmt), depth+1);
            break;
        case EXPR_STMT:
            print_expr_stmt(((ExprStmt*)stmt->stmt), depth+1);
            break;
        case VAR_STMT:
            print_var_stmt(((VarStmt*)stmt->stmt), depth+1);
            break;
        case BLOCK_STMT:
            print_block_stmt(((BlockStmt*)stmt->stmt), depth+1);
            break;
        default:
            return;
    }
}

void print_print_stmt(PrintStmt * stmt, size_t depth) {
    if (stmt == NULL) {
        return;
    }

    printf_with_indent(depth, "[PrintStmt]\n");
    if (stmt->expr != NULL) {
        print_ast(stmt->expr, depth+1);
    }
}

void print_expr_stmt(ExprStmt * stmt, size_t depth) {
    if (stmt == NULL) {
        return;
    }

    printf_with_indent(depth, "[ExprStmt]\n");
    if (stmt->expr != NULL) {
        print_ast(stmt->expr, depth+1);
    }
}

void print_var_stmt(VarStmt * stmt, size_t depth) {
    if (stmt == NULL) {
        return;
    }

    printf_with_indent(depth, "[VarStmt] ");
    printf("'%s'\n", stmt->name->lexeme);
    if (stmt->initializer != NULL) {
        print_ast(stmt->initializer, depth+1);
    }
}

void print_block_stmt(BlockStmt * stmt, size_t depth) {
    if (stmt == NULL) {
        return;
    }

    printf_with_indent(depth, "[BlockStmt]\n");
    if (stmt->head_stmt != NULL) {
        print_stmt_list(stmt->head_stmt, depth+1);
    }
}

