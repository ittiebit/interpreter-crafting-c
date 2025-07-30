#include <stdio.h>
#include <stdlib.h>
#include "clox.h"
#include "expr.h"
#include "token.h"
#include "parser.h"

char p_match(Parser * parser, TokenType token_type);
char p_check(Parser * parser, TokenType type);
Token * p_advance(Parser * parser);
char p_isAtEnd(Parser * parser);
Token * p_peek(Parser * parser);
Token * p_previous(Parser * parser);

Token * p_consume(Parser * parser, TokenType type, char * message);
void * p_error(Token * token, char * message);

Expr * expression(Parser * parser);
Expr * equality(Parser * parser);
Expr * comparison(Parser * parser);
Expr * term(Parser * parser);
Expr * factor(Parser * parser);
Expr * unary(Parser * parser);
Expr * primary(Parser * parser);

Parser * create_parser(Token ** tokens) {
    if (tokens == NULL) {
        return NULL;
    }
    Parser * parser = malloc(sizeof(Parser));

    if (parser == NULL) {
        exit(1);
    }

    parser->tokens = tokens;

    return parser;
}

void free_parser(Parser * parser) {
    if (parser == NULL) {
        return;
    }

    parser->tokens = NULL;
    parser->current = 0;

    free(parser);
}

Expr * parse(Parser * parser) {
    Expr * expr = NULL;
    if ((expr = expression(parser)) != NULL) {
        return expr;
    } else {
        return NULL;
    }
}

char p_match(Parser * parser, TokenType token_type) {
    if (p_peek(parser)->type == token_type) {
        p_advance(parser);
        return 1;
    }

    return 0;
}

char p_check(Parser * parser, TokenType type) {
    if (p_isAtEnd(parser)) return 0;
    return p_peek(parser)->type == type;
  }

Token * p_advance(Parser * parser) {
    if (!p_isAtEnd(parser)) {
        parser->current++;
    }
    return p_previous(parser);
}

char p_isAtEnd(Parser * parser) {
    return p_peek(parser)->type == EOFF;
}

Token * p_peek(Parser * parser) {
    return parser->tokens[parser->current];
}

Token * p_previous(Parser * parser) {
    return parser->tokens[parser->current - 1];
}

Token * p_consume(Parser * parser, TokenType type, char * message) {
    if (p_check(parser, type)) return p_advance(parser);

    return p_error(p_peek(parser), message); // TODO: Error enum/type or something?
}

void * p_error(Token * token, char * message) {
    error(token, message);
    return NULL;
}

void synchronize(Parser * parser) {
    p_advance(parser);

    while (p_isAtEnd(parser)) {
        if (p_previous(parser)->type == SEMICOLON) {
            return;
        }

        // Finding beginning of statement
        switch (p_peek(parser)->type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
        }
        p_advance(parser);
    }
}

Expr * expression(Parser * parser) {
    return equality(parser);
}

Expr * equality(Parser * parser) {
    Expr * expr = comparison(parser);

    while (p_match(parser, BANG_EQUAL) || p_match(parser, EQUAL_EQUAL)) {
        Token * operator = p_previous(parser);
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
        Token * operator = p_previous(parser);
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
        Token * operator = p_previous(parser);
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
        Token * operator = p_previous(parser);
        Expr * right = unary(parser);
        expr->expr = new_binary_expr(expr, operator, right);
        if (expr->expr == NULL) {
            exit(1);
        }
    }

    return expr;
}

Expr * unary(Parser * parser) {
    if (p_match(parser, BANG) || p_match(parser, MINUS)) {
        Token * operator = p_previous(parser);
        Expr * right = unary(parser);
        return (Expr*)new_unary_expr(operator, right);
    }

    return primary(parser);
}

Expr * primary(Parser * parser) {
    if (p_match(parser, TRUE)) {
        return (Expr*)new_literal_expr(TRUE, NULL); // True
    } else if (p_match(parser, FALSE)) {
        return (Expr*)new_literal_expr(FALSE, NULL); // False
    } else if (p_match(parser, NIL)) {
        return (Expr*)new_literal_expr(NIL, NULL); // NULL
    }

    if (p_match(parser, NUMBER) || p_match(parser, STRING)) {
        Token * token = p_previous(parser);
        return (Expr*)new_literal_expr(token->type, token->literal);
    }

    if (p_match(parser, LEFT_PAREN)) {
        Expr * expr = expression(parser);
        p_consume(parser, RIGHT_PAREN, "Expect ')' after expression.");
        return (Expr*)new_grouping_expr(expr);
    }

    p_error(p_peek(parser), "Expect expression.");

    return NULL;
}

