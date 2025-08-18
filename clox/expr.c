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

    BinaryExpr * binary_expr = malloc(sizeof(BinaryExpr));
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
    BinaryExpr * binary_expr = expr->expr;
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

    UnaryExpr * unary_expr = malloc(sizeof(UnaryExpr));
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
    UnaryExpr * unary_expr = expr->expr;
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

    TernaryExpr * ternary_expr = malloc(sizeof(TernaryExpr));
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
    TernaryExpr * ternary_expr = expr->expr;
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

    LiteralExpr * literal_expr = malloc(sizeof(LiteralExpr));
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
    LiteralExpr * literal_expr = expr->expr;
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

    GroupingExpr * grouping_expr = malloc(sizeof(GroupingExpr));
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
    GroupingExpr * grouping_expr = expr->expr;
    grouping_expr->expr = NULL; // surely dont have to free it
    free(grouping_expr);
    expr->expr = NULL;
    expr->expr_type = 0;
    free(expr);
}

Expr * new_variable_expr(Token * token) {
    if (token == NULL) {
        exit(1);
    }

    Expr * expr = malloc(sizeof(Expr));
    if (expr == NULL) {
        exit(1);
    }

    VariableExpr * variable_expr = malloc(sizeof(VariableExpr));
    if (variable_expr == NULL) {
        exit(1);
    }
    variable_expr->name = token;
    expr->expr = variable_expr;
    expr->expr_type = EXPR_VARIABLE;

    return expr;
}

void free_variable_expr(Expr * expr) {
    VariableExpr * variable_expr = expr->expr;
    variable_expr->name = NULL;
    free(variable_expr);
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
        free_ast(((GroupingExpr*)expr->expr)->expr);
        break;
    case EXPR_UNARY:
        free_unary_expr(expr);
        free_ast(((UnaryExpr*)expr->expr)->expr);
        break;
    case EXPR_BINARY:
        free_binary_expr(expr);
        free_ast(((BinaryExpr*)expr->expr)->left);
        free_ast(((BinaryExpr*)expr->expr)->right);
        break;
    case EXPR_OPERATOR:
        //free_operator_expr(expr);
        // TODO later
        break;
    case EXPR_VARIABLE:
        free_variable_expr(expr);
        break;
    default:
        return;
    }
}

/*
*   Statements
*/

//Stmt * new_stmt(StmtType stmt_type, Expr * expr) {
//    if (expr == NULL) {
//        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_stmt(): Expr * expr is NULL\n");
//        exit(1);
//    }
//    Stmt * stmt = malloc(sizeof(Stmt));
//    if (stmt == NULL) {
//        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_stmt(): Failed to allocate Stmt * stmt\n");
//        exit(1);
//    }
//
//    switch (stmt_type) {
//        case PRINT_STMT:
//            if ((stmt->stmt = malloc(sizeof(PrintStmt))) == NULL) {
//                fprintf(stderr, "[INTERNAL ERROR] expr.c - new_stmt(): Failed to allocate PrintStmt\n");
//            }
//            stmt = new_print_stmt()
//            break;
//        case EXPR_STMT:
//            if ((stmt->stmt = malloc(sizeof(ExprStmt))) == NULL) {
//                fprintf(stderr, "[INTERNAL ERROR] expr.c - new_stmt(): Failed to allocate ExprStmt\n");
//            }
//            break;
//        case VAR_STMT:
//            if ((stmt->stmt = malloc(sizeof(VarStmt))) == NULL) {
//                fprintf(stderr, "[INTERNAL ERROR] expr.c - new_stmt(): Failed to allocate VarStmt\n");
//            }
//            break;
//    }
//    stmt->stmt_type = stmt_type;
//    stmt->next_stmt = NULL;
//    stmt->prev_stmt = NULL;
//
//    return stmt;
//}

Stmt * new_print_stmt(Expr * expr) {
    PrintStmt * print_stmt = malloc(sizeof(PrintStmt));
    if (print_stmt == NULL) {
        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_print_stmt(): Failed to allocate PrintStmt\n");
        return NULL;
    }
    print_stmt->expr = expr;

    Stmt * stmt = malloc(sizeof(Stmt));
    if (print_stmt == NULL) {
        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_print_stmt(): Failed to allocate Stmt\n");
        return NULL;
    }
    stmt->stmt = print_stmt;
    stmt->stmt_type = PRINT_STMT;
    stmt->next_stmt = NULL;
    stmt->prev_stmt = NULL;
    return stmt;
}


void free_print_stmt(PrintStmt * stmt) {
    if (stmt->expr == NULL) {
        return;
    }
    if (stmt->expr != NULL) {
        free_ast(stmt->expr);
        stmt->expr = NULL;
    }
    free(stmt);
}

Stmt * new_expr_stmt(Expr * expr) {
    ExprStmt * expr_stmt = malloc(sizeof(ExprStmt));
    if (expr_stmt == NULL) {
        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_expr_stmt(): Failed to allocate ExprStmt\n");
        return NULL;
    }
    expr_stmt->expr = expr;

    Stmt * stmt = malloc(sizeof(Stmt));
    if (expr_stmt == NULL) {
        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_expr_stmt(): Failed to allocate Stmt\n");
        return NULL;
    }
    stmt->stmt = expr_stmt;
    stmt->stmt_type = EXPR_STMT;
    stmt->next_stmt = NULL;
    stmt->prev_stmt = NULL;
    return stmt;
}

void free_expr_stmt(ExprStmt * stmt) {
    if (stmt == NULL) {
        return;
    }
    if (stmt->expr != NULL) {
        free_ast(stmt->expr);
        stmt->expr = NULL;
    }
    free(stmt);
}

Stmt * new_var_stmt(Token * name, Expr * initializer) {
    VarStmt * var_stmt = malloc(sizeof(VarStmt));
    if (var_stmt == NULL) {
        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_var_stmt(): Failed to allocate VarStmt\n");
        return NULL;
    }
    var_stmt->initializer = initializer;
    var_stmt->name = name;

    Stmt * stmt = malloc(sizeof(Stmt));
    if (var_stmt == NULL) {
        fprintf(stderr, "[INTERNAL ERROR] expr.c - new_var_stmt(): Failed to allocate Stmt\n");
        return NULL;
    }
    stmt->stmt = var_stmt;
    stmt->stmt_type = VAR_STMT;
    return stmt;
}

void free_var_stmt(VarStmt * stmt) {
    if (stmt == NULL) {
        return;
    }
    if (stmt->initializer != NULL) {
        free_ast(stmt->initializer);
        stmt->initializer = NULL;
    }
    free(stmt);
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

void free_stmt(Stmt * stmt) {
    if (stmt == NULL) {
        return;
    }
    switch (stmt->stmt_type) {
        case PRINT_STMT:
            break;
        case EXPR_STMT:
            if (((ExprStmt*)stmt->stmt)->expr != NULL) {
                free_ast(((ExprStmt*)stmt->stmt)->expr);
                ((ExprStmt*)stmt->stmt)->expr = NULL;
            }
            break;
        case VAR_STMT:
            if (((VarStmt*)stmt->stmt)->initializer != NULL) {
                free_ast(((VarStmt*)stmt->stmt)->initializer);
                ((VarStmt*)stmt->stmt)->initializer = NULL;
                free(((VarStmt*)stmt->stmt)->name);
                ((VarStmt*)stmt->stmt)->name = NULL;
            }
            break;
    }
    if (stmt->stmt != NULL) {
        free(stmt->stmt);
        stmt->stmt = NULL;
    }
    stmt->stmt_type = 0;
    free(stmt);
}

void free_stmt_list(Stmt * stmt) {
    if (stmt == NULL) {
        return;
    }

    Stmt * cur_stmt = stmt;

    if (cur_stmt->next_stmt == NULL) {
        free_stmt(cur_stmt);
        return;
    }

    while (cur_stmt->next_stmt != NULL) {
        cur_stmt = cur_stmt->next_stmt;
        free_stmt(cur_stmt->prev_stmt);
        cur_stmt->prev_stmt = NULL;
    }
    free_stmt(cur_stmt);
}
