#include <stdlib.h>
#include "expr.h"
#include "token.h"
#include "parser.h"

char p_match(Parser * parser, TokenType token_type);
Token p_advance(Parser * parser);
char p_isAtEnd(Parser * parser);
Token p_peek(Parser * parser);
Token p_previous(Parser * parser);
Expr * expression(Parser * parser);
Expr * equality(Parser * parser);
Expr * comparison(Parser * parser);
Expr * term(Parser * parser);
Expr * factor(Parser * parser);
Expr * unary(Parser * parser);
Expr * primary(Parser * parser);

char p_match(Parser * parser, TokenType token_type) {
    if (p_peek(parser).type == token_type) {
        p_advance(parser);
        return 1;
    }

    return 0;
}

Token p_advance(Parser * parser) {
    if (p_isAtEnd(parser)) {
        (parser->current)++;
    }
    return p_previous(parser);
}

char p_isAtEnd(Parser * parser) {
    return p_peek(parser).type == EOFF;
}

Token p_peek(Parser * parser) {
    return *(parser->tokens[parser->current]);
}

Token p_previous(Parser * parser) {
    return *(parser->tokens[parser->current - 1]);
}


Expr * expression(Parser * parser) {
    return equality(parser);
}

Expr * equality(Parser * parser) {
    Expr * expr = comparison(parser);

    while (p_match(parser, BANG_EQUAL) || p_match(parser, EQUAL_EQUAL)) {
        Token operator = p_previous(parser);
        Expr * right = comparison(parser);
        expr->expr = new_binary_expr(expr, operator, right);
        if (expr->expr == NULL) {
            exit(1);
        }
    }

    return expr;
}

Expr * comparison(Parser * parser) {
    Expr * expr = term(parser);

    while (p_match(parser, GREATER) || p_match(parser, GREATER_EQUAL) || p_match(parser, LESS) || p_match(parser, LESS_EQUAL)) {
        Token operator = p_previous(parser);
        Expr * right = term(parser);
        expr->expr = new_binary_expr(expr, operator, right);
        if (expr->expr == NULL) {
            exit(1);
        }
    }

    return expr;
}

Expr * term(Parser * parser) {
    Expr * expr = factor(parser);

    while (p_match(parser, MINUS) || p_match(parser, PLUS)) {
        Token operator = p_previous(parser);
        Expr * right = factor(parser);
        expr->expr = new_binary_expr(expr, operator, right);
        if (expr->expr == NULL) {
            exit(1);
        }
    }

    return expr;
}

Expr * factor(Parser * parser) {
    Expr * expr = unary(parser);

    while (p_match(parser, SLASH) || p_match(parser, STAR)) {
        Token operator = p_previous(parser);
        Expr * right = unary(parser);
        expr->expr = new_binary_expr(expr, operator, right);
        if (expr->expr == NULL) {
            exit(1);
        }
    }

    return expr;
}
