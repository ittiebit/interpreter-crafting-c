#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./environment.h"
#include "./interpreter.h"
#include "./token.h"
#include "../utils/hashmap.h"

Environment * new_environment() {
    Environment * env = malloc(sizeof(Environment));
    if (env == NULL) {
        exit(1);
    }

    env->env_map = hashmap_new();
    if (env->env_map == NULL) {
        fprintf(stderr, "[INTERNAL ERROR] environment.c - new_environment(): hashmap_new() returned NULL");
        free(env);
        exit(1);
    }

    return env;
}

void free_environment(Environment * env) {
    if (env == NULL) {
        return;
    }
    if (env->env_map != NULL) {
        hashmap_free(env->env_map);
        env->env_map = NULL;
    }

    free(env);
}

void define(Environment * env, char * name, Value * val) {
    int map_err = hashmap_put(env->env_map, name, val);
    if (map_err != MAP_OK) {
        fprintf(stderr, "[INTERNAL ERROR] environment.c - define: hashmap_put error %i", map_err);
        exit(1);
    }
}

Value * get(char * p_has_runtime_error, Environment * env, Token * name) {
    any_t val = NULL;
    if (hashmap_get(env->env_map, name->lexeme, &val) == MAP_OK) {
        return (Value*)val;
    } else {
        char * message = strdup("Undefined variable '");
        strcat(message, name->lexeme);
        strcat(message, "'.");
        runtime_error(p_has_runtime_error, name, message);
        return NULL;
    }
}
