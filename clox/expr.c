#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./expr.h"
#include "./token.h"


Expr * new_binary_expr(Expr * left, Token * operator, Expr * right) {
    if (left == NULL || right == NULL) {
        return NULL;
    }

    Expr * expr = malloc(sizeof(Expr));
    if (expr == NULL) {
        exit(1);
    }

    Binary_expr * binary_expr = malloc(sizeof(Binary_expr));
    if (binary_expr == NULL) {
        exit(1);
    }

    binary_expr->token = operator;
    binary_expr->left = left;
    binary_expr->right = right;

    expr->expr = binary_expr;
    expr->expr_type = EXPR_BINARY;

    return expr;
}

void free_binary_expr(Expr * expr) {
    Binary_expr * binary_expr = expr->expr;
    binary_expr->token = NULL;
    binary_expr->left = NULL;
    binary_expr->right = NULL;
    free(binary_expr);
    expr->expr = NULL;
    expr->expr_type = 0;
    free(expr);
}

Expr * new_unary_expr(Token * operator, Expr * right) {
    if (right == NULL) {
        return NULL;
    }

    Expr * expr = malloc(sizeof(Expr));
    if (expr == NULL) {
        exit(1);
    }

    Unary_expr * unary_expr = malloc(sizeof(Unary_expr));
    if (unary_expr == NULL) {
        exit(1);
    }

    unary_expr->token = operator;
    unary_expr->expr = right;

    expr->expr = unary_expr;
    expr->expr_type = EXPR_UNARY;

    return expr;
}

void free_unary_expr(Expr * expr) {
    Unary_expr * unary_expr = expr->expr;
    unary_expr->token = NULL;
    unary_expr->expr = NULL;
    free(unary_expr);
    expr->expr = NULL;
    expr->expr_type = 0;
    free(expr);
}

Expr * new_ternary_expr(Expr * left, Token * op_left, Expr * mid, Token * op_right, Expr * right) {
    if (left == NULL || mid == NULL || right == NULL) {
        return NULL;
    }

    Expr * expr = malloc(sizeof(Expr));
    if (expr == NULL) {
        exit(1);
    }

    Ternary_expr * ternary_expr = malloc(sizeof(Ternary_expr));
    if (ternary_expr == NULL) {
        exit(1);
    }

    ternary_expr->token_left = op_left;
    ternary_expr->token_right = op_right;
    ternary_expr->left = left;
    ternary_expr->mid = mid;
    ternary_expr->right = right;

    expr->expr = ternary_expr;
    expr->expr_type = EXPR_TERNARY;

    return expr;
}

void free_ternary_expr(Expr * expr) {
    Ternary_expr * ternary_expr = expr->expr;
    ternary_expr->token_left = NULL;
    ternary_expr->token_right = NULL;
    ternary_expr->left = NULL;
    ternary_expr->mid = NULL;
    ternary_expr->right = NULL;
    free(ternary_expr);
    expr->expr = NULL;
    expr->expr_type = 0;
    free(expr);
}

Expr * new_literal_expr(TokenType type, void * literal) {
    Expr * expr = malloc(sizeof(Expr));
    if (expr == NULL) {
        exit(1);
    }

    Literal_expr * literal_expr = malloc(sizeof(Literal_expr));
    if (literal_expr == NULL) {
        exit(1);
    }

    if (type == TRUE) {
        literal_expr->literal_type = LITERAL_TRUE;
        literal_expr->value = NULL;
    } else if (type == FALSE) {
        literal_expr->literal_type = LITERAL_FALSE;
        literal_expr->value = NULL;
    } else if (type == NIL) {
        literal_expr->literal_type = LITERAL_NIL;
        literal_expr->value = NULL;
    } else if (type == STRING) {
        literal_expr->literal_type = LITERAL_STRING;
        if ((literal_expr->value = malloc(sizeof(char) * strlen(literal))) == NULL) {
            exit(1);
        }
        strcpy(literal_expr->value, literal);
    } else if (type == NUMBER) {
        literal_expr->literal_type = LITERAL_NUMBER;
        if ((literal_expr->value = malloc(sizeof(double))) == NULL) {
            exit(1);
        }
        memcpy(literal_expr->value, literal, sizeof(double));
    } else {
        printf("Bad new literal expression\n");
        exit(1);
    }

    expr->expr = literal_expr;
    expr->expr_type = EXPR_LITERAL;

    return expr;
}

void free_literal_expr(Expr * expr) {
    Literal_expr * literal_expr = expr->expr;
    if (literal_expr->value != NULL) {
        free(literal_expr->value);
    }
    free(literal_expr);
    expr->expr = NULL;
    expr->expr_type = 0;
    free(expr);
}

Expr * new_grouping_expr(TokenType type, Expr * expr_val) {
    if (expr_val == NULL) {
        exit(1);
    }

    Expr * expr = malloc(sizeof(Expr));
    if (expr == NULL) {
        exit(1);
    }

    Grouping_expr * grouping_expr = malloc(sizeof(Grouping_expr));
    if (grouping_expr == NULL) {
        exit(1);
    }
    grouping_expr->expr = expr_val;
    grouping_expr->type = type;

    expr->expr = grouping_expr;
    expr->expr_type = EXPR_GROUPING;

    return expr;
}

void free_grouping_expr(Expr * expr) {
    Grouping_expr * grouping_expr = expr->expr;
    grouping_expr->expr = NULL; // surely dont have to free it
    free(grouping_expr);
    expr->expr = NULL;
    expr->expr_type = 0;
    free(expr);
}

void free_ast(Expr * expr) {
    // Print the syntax tree recursively DFS

    switch (expr->expr_type) {
    case EXPR_LITERAL:
        free_literal_expr(expr);
        break;
    case EXPR_GROUPING:
        free_grouping_expr(expr);
        free_ast(((Grouping_expr*)expr->expr)->expr);
        break;
    case EXPR_UNARY:
        free_unary_expr(expr);
        free_ast(((Unary_expr*)expr->expr)->expr);
        break;
    case EXPR_BINARY:
        free_binary_expr(expr);
        free_ast(((Binary_expr*)expr->expr)->left);
        free_ast(((Binary_expr*)expr->expr)->right);
        break;
    case EXPR_OPERATOR:
        //free_operator_expr(expr);
        // TODO later
        break;
    default:
        return;
    }
}

/*
*   Statements
*/

Stmt * new_stmt(Stmt_type stmt_type, Expr * expr) {
    if (expr == NULL) {
        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_stmt(): Expr * expr is NULL\n");
        exit(1);
    }
    Stmt * stmt = malloc(sizeof(Stmt));
    if (stmt == NULL) {
        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_stmt(): Failed to allocate Stmt * stmt\n");
        exit(1);
    }

    stmt->stmt_type = stmt_type;
    stmt->expr = expr;
    stmt->next_stmt = NULL;
    stmt->prev_stmt = NULL;

    return stmt;
}

void add_stmt(Stmt * head_stmt, Stmt * new_stmt) {
    if (head_stmt == NULL || new_stmt == NULL) {
        return;
    }

    if (head_stmt->next_stmt == NULL) {
        head_stmt->next_stmt = new_stmt;
        head_stmt->prev_stmt = new_stmt;
        new_stmt->next_stmt = head_stmt;
        new_stmt->prev_stmt = head_stmt;
    } else {
        Stmt * tail = head_stmt->prev_stmt;
        new_stmt->prev_stmt = tail;
        new_stmt->next_stmt = head_stmt;
        tail->next_stmt = new_stmt;
        head_stmt->prev_stmt = new_stmt;
    }
    new_stmt->next_stmt = NULL;
}

/* Free the AST under stmt->expr before! */
void free_stmt(Stmt * stmt) {
    if (stmt == NULL) {
        return;
    }
    stmt->expr = NULL; // should be freed by freeing the ast
    stmt->stmt_type = 0;
}

void free_stmt_list(Stmt * stmt) {
    if (stmt == NULL) {
        return;
    }

    Stmt * cur_stmt = stmt;

    if (cur_stmt->next_stmt == NULL) {
        free(cur_stmt);
        return;
    }

    while (cur_stmt->next_stmt != NULL) {
        cur_stmt = cur_stmt->next_stmt;
        free(cur_stmt->prev_stmt);
        free_ast(cur_stmt->prev_stmt->expr);
        cur_stmt->prev_stmt = NULL;
    }
    free(cur_stmt);
    free_ast(cur_stmt->expr);
}
