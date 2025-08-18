#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../utils/hashmap.h"
#include "./token.h"
#include "./types.h"

typedef struct Environment_t {
    map_t env_map;
} Environment;

Environment * new_environment();
void free_environment(Environment * env);
void define(Environment * env, char * name, Value * val);
Value * get(char * p_has_runtime_error, Environment * env, Token * name);

#endif // ENVIRONMENT_H
