#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./types.h"
#include "../clox/expr.h"
#include "./token.h"
#include "./interpreter.h"

Value *  evaluate(Interpreter * inter, Expr * expr);
void execute(Interpreter * inter, Stmt * stmt);
Value * accept_expr(Interpreter * inter, Expr * expr);
void accept_stmt(Interpreter * inter, Stmt * stmt);
Value * create_value(const void * val, size_t val_size, Value_Type type);
void free_value(Value * val);
void * is_truthy(Interpreter * inter, Value * val);
boolean is_equal(Value * a, Value * b);
boolean equals(Value * a, Value * b); // compare object values
void check_number_operand(Interpreter * inter, Token * operator, Value * operand);
char * stringify(Value * val);

void set_runtime_error(Interpreter * inter, Token * token, char * message);
void free_runtime_error_struct(RuntimeError * runtime_error);

Value * visit_binary_expr(Interpreter * inter, Expr * expr);
Value * visit_unary_expr(Interpreter * inter, Expr * expr);
Value * visit_literal_expr(Interpreter * inter, Expr * expr);
Value * visit_grouping_expr(Interpreter * inter, Expr * expr);
Value * visit_operator_expr(Interpreter * inter, Expr * expr);
Value * visit_ternary_expr(Interpreter * inter, Expr * expr);
Value * visit_variable_expr(Interpreter * inter, Expr * expr);

void visit_expression_stmt(Interpreter * inter, Stmt * stmt);
void visit_print_stmt(Interpreter * inter, Stmt * stmt);
void visit_var_stmt(Interpreter * inter, Stmt * stmt);

Interpreter * create_interpreter(char * p_has_runtime_error) {
    Interpreter * inter;
    if ((inter= malloc(sizeof(Interpreter))) == NULL) {
        exit(1);
    }
    if (p_has_runtime_error != NULL) {
        inter->has_runtime_error = p_has_runtime_error;
    }
    inter->env = new_environment();
    if (inter->env == NULL) {
        free_interpreter(inter);
        exit(1);
    }
    return inter;
}

void free_interpreter(Interpreter * inter) {
    if (inter == NULL) {
        return;
    }
    if (inter->runtime_error != NULL) {
        free_runtime_error_struct(inter->runtime_error);
        inter->runtime_error = NULL;
    }
    free(inter);
    inter = NULL;
}

void interpret(Interpreter * inter, Stmt * head_stmt) {
    if (inter == NULL || head_stmt == NULL) {
        return;
    }

    Stmt * cur_stmt = head_stmt;

    execute(inter, cur_stmt);
    try_throw_runtime_error(inter);

    while (cur_stmt->next_stmt != NULL) {
        cur_stmt = cur_stmt->next_stmt;
        execute(inter, cur_stmt);
        try_throw_runtime_error(inter);
    }
}

/* Returns a heap allocated string */
char * stringify(Value * val) {
    if (val->value == NULL || val->type == VAL_NIL)  {
        return strdup("nil");
    }

    if (val->type == VAL_STRING) {
        return strdup((char*)val->value);
    }

    if (val->type == VAL_BOOLEAN) {
        if (*(boolean*)val->value) {
            return strdup("true");
        } else {
            return strdup("false");
        }
    }

    char * str;

    if (val->type == VAL_DOUBLE) {
        str = malloc(sizeof(char) * 50);
        if (str == NULL) {
            return NULL;
        }
        if (*(double*)val->value == floor(*(double*)val->value)) {
            sprintf(str, "%ld", (long)*(double*)val->value);
        } else {
            sprintf(str, "%f", *(double*)val->value);
        }
        return str;
    }

    return NULL;
}

Value * create_value(const void * val, size_t val_size, Value_Type type) {
    Value * value = NULL;
    if ((value = malloc(sizeof(Value))) == NULL) {
        exit(1);
    }
    // For handling NIL/NULL type
    if (val == NULL) {
        value->value = NULL;
        value->type = type;
        return value;
    }
    if (val_size == 0) {
        fprintf(stderr, "[INTERNAL ERROR] interpreter.c - create_value(): val_size should be greater than 0\n");
        exit(1);
    }
    if (type == VAL_STRING) {
        value->value = strdup(val);
    } else {
        if ((value->value = malloc(val_size)) == NULL) {
            exit(1);
        }
        memcpy(value->value, val, val_size);
    }
    value->type = type;
    return value;
}

void free_value(Value * val) {
    if (val == NULL) {
        return;
    }
    if (val->value != NULL) {
        free(val->value);
        val->value = NULL;
    }
    free(val);
    val = NULL;
}

Value * evaluate(Interpreter * inter, Expr * expr) {
    return accept_expr(inter, expr);
}

void execute(Interpreter * inter, Stmt * stmt) {
    if (inter == NULL || stmt == NULL) {
        return;
    }
    accept_stmt(inter, stmt);
}

void accept_stmt(Interpreter * inter, Stmt * stmt) {
    if (inter == NULL || stmt == NULL) {
        return;
    }

    switch (stmt->stmt_type) {
        case PRINT_STMT:
            visit_print_stmt(inter, stmt);
            break;
        case VAR_STMT:
            visit_var_stmt(inter, stmt);
            break;
        case EXPR_STMT:
        default:
            visit_expression_stmt(inter, stmt);
            return;
    }

    return;
}

void * is_truthy(Interpreter * inter, Value * val) {
    if (val->type == VAL_BOOLEAN) {
        return val;
    }
    boolean truthiness;
    if (val->value == NULL) {
        truthiness = VALUE_FALSE;
        val = create_value(&truthiness, sizeof(boolean), VAL_BOOLEAN);
        return val;
    }
    truthiness = VALUE_TRUE;
    val = create_value(&truthiness, sizeof(boolean), VAL_BOOLEAN);
    return val;
}

boolean is_equal(Value * a, Value * b) {
    if (a == NULL && b == NULL) return VALUE_TRUE;
    if (a == NULL) return VALUE_FALSE;

    return equals(a, b);
}

boolean equals(Value * a, Value * b) {
    if ((a == NULL) != (b == NULL)) {
        return VALUE_FALSE;
    }
    if (a->type != b->type) {
        return VALUE_FALSE;
    }
    if (a->type == VAL_STRING && b->type == VAL_STRING) {
        return strcmp((char*)a->value, (char*)b->value);
    }
    if (a->type == VAL_BOOLEAN && b->type == VAL_BOOLEAN) {
        return (*(boolean*)a->value) == (*(boolean*)b->value);
    }
    if (a->type == VAL_DOUBLE && b->type == VAL_DOUBLE) {
        return (*(double*)a->value) == (*(double*)b->value);
    }
    if (a->type == VAL_NIL && b->type == VAL_NIL) {
        return VALUE_TRUE;
    }
    return VALUE_FALSE;
}

void check_number_operand(Interpreter * inter, Token * operator, Value * operand) {
    if (operand->type == VAL_DOUBLE) return;
    set_runtime_error(inter, operator, strdup("Operand must be a number."));
}

void check_number_operands(Interpreter * inter, Token * operator, Value * left, Value * right) {
    if (left->type == VAL_DOUBLE && right->type == VAL_DOUBLE) return;
    set_runtime_error(inter, operator, strdup("Operand must be a numbers."));
}


void free_runtime_error_struct(RuntimeError * runtime_error) {
    if (runtime_error == NULL) {
        return;
    }

    if (runtime_error->message != NULL) {
        free(runtime_error->message);
        runtime_error->message = NULL;
    }

    runtime_error->token = NULL;

    free(runtime_error);
}

void set_runtime_error(Interpreter * inter, Token * token, char * message) {
    RuntimeError * runtime_error;
    if (inter->runtime_error != NULL) {
        if ((runtime_error = malloc(sizeof(RuntimeError))) == NULL) {
            return;
            //exit(1);
        }
    }

    if (message == NULL) {
        message = strdup("");
    }

    runtime_error->message = strdup(message);
    runtime_error->token = token;

    *inter->has_runtime_error = VALUE_TRUE;
    inter->runtime_error = runtime_error;
}

void try_throw_runtime_error(Interpreter * inter) {
    if (inter == NULL || *inter->has_runtime_error == VALUE_FALSE || inter->runtime_error == NULL || inter->runtime_error->message == NULL) {
        return;
    }
    fprintf(stderr, "[Runtime Error] %s\n", inter->runtime_error->message);
    *inter->has_runtime_error = VALUE_FALSE;
}


Value * accept_expr(Interpreter * inter, Expr * expr) {
    if (expr == NULL) {
        return NULL;
    }

    switch (expr->expr_type) {
        case EXPR_LITERAL:
            return visit_literal_expr(inter, expr);
            break;
        case EXPR_GROUPING:
            return visit_grouping_expr(inter, expr);
            break;
        case EXPR_UNARY:
            return visit_unary_expr(inter, expr);
            break;
        case EXPR_BINARY:
            return visit_binary_expr(inter, expr);
            break;
        case EXPR_TERNARY:
            return visit_ternary_expr(inter, expr);
            break;
        case EXPR_OPERATOR:
            return visit_operator_expr(inter, expr);
            break;
        case EXPR_VARIABLE:
            return visit_variable_expr(inter, expr);
            break;
        default:
            return NULL;
            break;
    }

    return NULL;
}

Value * visit_binary_expr(Interpreter * inter, Expr * expr) {
    BinaryExpr * binary_expr = (BinaryExpr*)expr->expr;

    Value * right_val = evaluate(inter, binary_expr->right);
    Value * left_val = evaluate(inter, binary_expr->left);

    TokenType type = binary_expr->token->type;

    // Math operations
    double right_num = *(double*)right_val->value;
    double left_num = *(double*)left_val->value;
    double num_result = 0;
    switch (type) {
        case MINUS:
            check_number_operand(inter, binary_expr->token, right_val);
            num_result = left_num - right_num;
            return create_value(&num_result, sizeof(double), VAL_DOUBLE);
        case PLUS:
            // Concatenate two strings
            if (right_val->type == VAL_STRING && left_val->type == VAL_STRING) {
                char * left_str = (char*)left_val->value;
                char * right_str = (char*)right_val->value;
                char * concatenated = malloc(sizeof(char) * (strlen(left_str)+strlen(right_str)+1));
                if (concatenated  == NULL) {
                    exit(1);
                }
                strcpy(concatenated, left_str);
                strcat(concatenated, right_str);
                return create_value(concatenated, sizeof(strlen(concatenated)), VAL_STRING);
            } else if (right_val->type == VAL_DOUBLE && left_val->type == VAL_DOUBLE) {
                num_result = left_num + right_num;
                return create_value(&num_result, sizeof(double), VAL_DOUBLE);
            }
            set_runtime_error(inter, binary_expr->token, strdup("Operands must be two numbers or two strings."));
            break;
        case SLASH:
            check_number_operands(inter, binary_expr->token, left_val, right_val);
            num_result = left_num / right_num;
            return create_value(&num_result, sizeof(double), VAL_DOUBLE);
        case STAR:
            check_number_operands(inter, binary_expr->token, left_val, right_val);
            num_result = left_num * right_num;
            return create_value(&num_result, sizeof(double), VAL_DOUBLE);
        default: break;
    }

    boolean bool_result = VALUE_FALSE;
    switch (type) {
        case EQUAL:
            bool_result = left_num == right_num;
            return create_value(&bool_result, sizeof(boolean), VAL_BOOLEAN);
            break;
        case GREATER:
            check_number_operands(inter, binary_expr->token, left_val, right_val);
            bool_result = left_num > right_num;
            return create_value(&bool_result, sizeof(boolean), VAL_BOOLEAN);
            break;
        case LESS:
            check_number_operands(inter, binary_expr->token, left_val, right_val);
            bool_result = left_num < right_num;
            return create_value(&bool_result, sizeof(boolean), VAL_BOOLEAN);
            break;
        case LESS_EQUAL:
            check_number_operands(inter, binary_expr->token, left_val, right_val);
            bool_result = left_num <= right_num;
            return create_value(&bool_result, sizeof(boolean), VAL_BOOLEAN);
            break;
        case GREATER_EQUAL:
            check_number_operands(inter, binary_expr->token, left_val, right_val);
            bool_result = left_num >= right_num;
            return create_value(&bool_result, sizeof(boolean), VAL_BOOLEAN);
            break;
        default: break;
    }

    boolean result = VALUE_FALSE;
    switch (type) {
        case BANG_EQUAL:
            result = !is_equal(left_val, right_val);
            return create_value(&result, sizeof(boolean), VAL_BOOLEAN);
        case EQUAL_EQUAL:
            result = is_equal(left_val, right_val);
            return create_value(&result, sizeof(boolean), VAL_BOOLEAN);
            break;
        case COMMA:
            // ???
            break;
        default: break;
    }

    return NULL;
}

Value * visit_ternary_expr(Interpreter * inter, Expr * expr) {
    return NULL;

    TernaryExpr * ternary_expr = (TernaryExpr*)expr->expr;

    TokenType type_left = ternary_expr->token_left->type;
    TokenType type_right = ternary_expr->token_right->type;
    if (type_left == QUESTION && type_right == COLON) {
    } else {
    }

    return NULL;
}

Value * visit_unary_expr(Interpreter * inter, Expr * expr) {
    UnaryExpr * unary_expr = (UnaryExpr*)expr->expr;
    TokenType type = unary_expr->token->type;

    Value * right_val = evaluate(inter, unary_expr->expr);

    double num;
    boolean boolean;

    switch (type) {
        case BANG:
            boolean = !*(char*)is_truthy(inter, right_val->value);
            return create_value(&boolean, sizeof(boolean), VAL_BOOLEAN);
        case MINUS:
            num = -(*(double*)right_val->value);
            return create_value(&num, sizeof(double), VAL_DOUBLE);
        default:
            return NULL;
            break;
    }

    return NULL;
}

Value * visit_literal_expr(Interpreter * inter, Expr * expr) {
    LiteralExpr * literal_expr = (LiteralExpr*)expr->expr;
    LiteralType type = literal_expr->literal_type;

    double num;
    boolean boolean;

    switch (type) {
        case LITERAL_TRUE:
            boolean = VALUE_TRUE;
            return create_value(&boolean, sizeof(boolean), VAL_BOOLEAN);
            break;
        case LITERAL_FALSE:
            boolean = VALUE_FALSE;
            return create_value(&boolean, sizeof(boolean), VAL_BOOLEAN);
            break;
        case LITERAL_NIL:
            return create_value(NULL, sizeof(char), VAL_NIL);
            break;
        case LITERAL_STRING:
            return create_value(literal_expr->value, strlen((char*)literal_expr->value+1), VAL_STRING);
            break;
        case LITERAL_NUMBER:
            return create_value(literal_expr->value, sizeof(double), VAL_DOUBLE);
            break;
        default:
            return NULL;
            break;
    }

    return NULL;
}

Value * visit_grouping_expr(Interpreter * inter, Expr * expr) {
    GroupingExpr * grouping_expr = (GroupingExpr*)expr->expr;

    return evaluate(inter, grouping_expr->expr);

    //TokenType type = grouping_expr->type;

    //switch (type) {
    //    case LEFT_PAREN:
    //        printf("()\n");
    //        break;
    //    default:
    //        printf("undefined\n");
    //        break;
    //}
    return NULL;
}

Value * visit_operator_expr(Interpreter * inter, Expr * expr) {
    return NULL;
}

Value * visit_variable_expr(Interpreter * inter, Expr * expr) {
    VariableExpr * var_expr = (VariableExpr*)expr->expr;
    return get(inter, var_expr->name);
}

void visit_expression_stmt(Interpreter * inter, Stmt * stmt) {
    evaluate(inter, ((ExprStmt*)stmt->stmt)->expr);
    return;
}

void visit_print_stmt(Interpreter * inter, Stmt * stmt) {
    Value * val = evaluate(inter, ((PrintStmt*)stmt->stmt)->expr);
    if (val != NULL) {
        fprintf(stdout, "%s\n", stringify(val));
    }
    return;
}

void visit_var_stmt(Interpreter * inter, Stmt * stmt) {
    VarStmt * var_stmt = (VarStmt*)stmt->stmt;
    Value * val = NULL;
    if (var_stmt->initializer != NULL) {
        val = evaluate(inter, var_stmt->initializer);
    }
    define(inter->env, var_stmt->name->lexeme, val);
    return;
}
