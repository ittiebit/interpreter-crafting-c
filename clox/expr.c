#include <stdlib.h>
#include <string.h>
#include "./expr.h"
#include "./token.h"


Binary_expr * new_binary_expr(Expr * left, Token token, Expr * right) {
    if (left == NULL || right == NULL) {
        return NULL;
    }

    Binary_expr * expr = malloc(sizeof(Binary_expr));
    if (expr == NULL) {
        exit(1);
    }

    memcpy(&expr->token, &token, sizeof(token));
    expr->left = left;
    expr->right = right;

    return expr;
}
