#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "./expr.h"
#include "./types.h"
#include "./environment.h"

typedef struct Interpreter_t {
    char * has_runtime_error;
    Environment * env;
} Interpreter;

Interpreter * create_interpreter(char * p_runtime_errors);
void free_interpreter(Interpreter * inter);
void interpret(Interpreter * inter, Stmt * head_stmt);
void runtime_error(char * p_has_runtime_error, Token * token, const char * message);

#endif //INTERPRETER_H
