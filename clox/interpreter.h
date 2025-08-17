#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "./expr.h"
#include "./types.h"

typedef struct Interpreter_t {
    char * has_runtime_error;
} Interpreter;

Interpreter * create_interpreter(char * p_runtime_errors);
void free_interpreter(Interpreter * inter);
void interpret(Interpreter * inter, Stmt * head_stmt);

#endif //INTERPRETER_H
