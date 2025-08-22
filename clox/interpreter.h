#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "./expr.h"
#include "./types.h"
#include "../utils/hashmap.h"

typedef struct ValueNode_t {
    struct ValueNode_t * next;
    Value * value;
} ValueNode;

typedef struct ValueList_t {
    ValueNode * head;
} ValueList;


typedef struct Environment_t {
    map_t env_map;
    struct Environment_t * enclosing;
    ValueList * value_list; // for freeing memory at the end
} Environment;


typedef struct EnvironmentNode_t {
    struct EnvironmentNode_t * next;
    Environment * env;
} EnvironmentNode;

typedef struct EnvironmentList_t {
    EnvironmentNode * head;
} EnvironmentList;

typedef struct RuntimeError_t {
    Token * token;
    char * message;
} RuntimeError;

typedef struct Interpreter_t {
    char * has_runtime_error;
    Environment * env; // global environment by default
    RuntimeError * runtime_error;
    EnvironmentList * env_list; // for freeing memory at the end
} Interpreter;

Interpreter * create_interpreter(char * p_runtime_errors);
void free_interpreter(Interpreter * inter);
void interpret(Interpreter * inter, Stmt * head_stmt);
Value * create_value(const void * val, size_t val_size, Value_Type type);
void free_value(Value * val);
void try_throw_runtime_error(Interpreter * inter);
void set_runtime_error(Interpreter * inter, Token * token, char * message);

Environment * new_environment(Interpreter * inter);
Environment * new_enclosing_environment(Interpreter * inter, Environment * env);
void free_environment(Environment * env);
void define(Environment * env, char * name, Value * val);
Value * get(Interpreter * inter, Environment * env, Token * name);
void assign(Interpreter * inter, Environment * env, Token * name, Value * val);

void add_value(ValueList ** value_list, Value * p_value);
void free_value_list(ValueList * value_list_head);
void add_environment(EnvironmentList ** environment_list, Environment * p_environment);
void free_environment_list(EnvironmentList * environment_list_head);

#endif //INTERPRETER_H
