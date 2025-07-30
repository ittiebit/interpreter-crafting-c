#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./expr.h"
#include "./token.h"


Binary_expr * new_binary_expr(Expr * left, Token * operator, Expr * right) {
    if (left == NULL || right == NULL) {
        return NULL;
    }

    Binary_expr * expr = malloc(sizeof(Binary_expr));
    if (expr == NULL) {
        exit(1);
    }

    expr->token = operator;
    expr->left = left;
    expr->right = right;

    return expr;
}

void free_binary_expr(Binary_expr * expr) {
    expr->token = NULL;
    expr->left = NULL;
    expr->right = NULL;
    free(expr);
}

Unary_expr * new_unary_expr(Token * operator, Expr * token) {
    if (token == NULL) {
        return NULL;
    }

    Unary_expr * expr = malloc(sizeof(Unary_expr));
    if (expr == NULL) {
        exit(1);
    }

    expr->token = operator;

    return expr;
}

void free_unary_expr(Unary_expr * expr) {
    expr->token = NULL;
    expr->expr = NULL;
    free(expr);
}

Literal_expr * new_literal_expr(TokenType type, void * literal) {
    Literal_expr * expr = malloc(sizeof(Literal_expr));
    if (expr == NULL) {
        exit(1);
    }

    if (type == TRUE) {
        expr->literal_type = LITERAL_TRUE;
        expr->value = NULL;
    } else if (type == FALSE) {
        expr->literal_type = LITERAL_FALSE;
        expr->value = NULL;
    } else if (type == NIL) {
        expr->literal_type = LITERAL_NIL;
        expr->value = NULL;
    } else if (type == STRING) {
        expr->literal_type = LITERAL_STRING;
        if ((expr->value = malloc(sizeof(char) * strlen(literal))) == NULL) {
            exit(1);
        }
        memcpy(expr->value, literal, sizeof(char) * strlen(literal));
    } else if (type == NUMBER) {
        expr->literal_type = LITERAL_NUMBER;
        if ((expr->value = malloc(sizeof(*literal))) == NULL) {
            exit(1);
        }
        memcpy(expr->value, literal, sizeof(*literal));
    } else {
        printf("Bad new literal expression\n");
        exit(1);
    }

    return expr;
}

void free_literal_expr(Literal_expr * expr) {
    if (expr->value != NULL) {
        free(expr->value);
    }
    free(expr);
}

Grouping_expr * new_grouping_expr(Expr * expr) {
    if (expr == NULL) {
        exit(1);
    }

    Grouping_expr * grouping_expr = malloc(sizeof(Grouping_expr));
    grouping_expr->expr = expr;

    return grouping_expr;
}

void free_grouping_expr(Grouping_expr * expr) {
    expr->expr = NULL;
    free(expr);
}
