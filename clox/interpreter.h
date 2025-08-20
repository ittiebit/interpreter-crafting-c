#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "./expr.h"
#include "./types.h"
#include "../utils/hashmap.h"

typedef struct Environment_t {
    map_t env_map;
} Environment;

typedef struct RuntimeError_t {
    Token * token;
    char * message;
} RuntimeError;

typedef struct Interpreter_t {
    char * has_runtime_error;
    Environment * env;
    RuntimeError * runtime_error;
} Interpreter;

Interpreter * create_interpreter(char * p_runtime_errors);
void free_interpreter(Interpreter * inter);
void interpret(Interpreter * inter, Stmt * head_stmt);
void try_throw_runtime_error(Interpreter * inter);
void set_runtime_error(Interpreter * inter, Token * token, char * message);

Environment * new_environment();
void free_environment(Environment * env);
void define(Environment * env, char * name, Value * val);
Value * get(Interpreter * inter, Token * name);
void assign(Interpreter * inter, Token * name, Value * val);

#endif //INTERPRETER_H
