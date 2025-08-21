#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./interpreter.h" // <-- typedef struct Environment here
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

    env->enclosing = NULL;

    return env;
}

Environment * new_enclosing_environment(Environment * env) {
    Environment * enclosing_env = new_environment();
    if (enclosing_env == NULL) {
        exit(1);
    }

    enclosing_env->enclosing = env;

    return enclosing_env;
}

void free_environment(Environment * env) {
    if (env == NULL) {
        return;
    }
    if (env->enclosing != NULL) {
        free_environment(env->enclosing); //for enclosed environments
        env->enclosing = NULL;
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

Value * get(Interpreter * inter, Environment * env, Token * name) {
    any_t val = NULL;
    if (hashmap_get(env->env_map, name->lexeme, &val) == MAP_OK) {
        return (Value*)val;
    }

    if (env->enclosing != NULL) {
        return get(inter, env->enclosing, name);
    }

    size_t size = strlen("Undefined variable '") + strlen(name->lexeme)
        + strlen("'.") + 1;

    char * message = malloc(size);
    if (message == NULL) {
        perror("malloc");
        exit(1);
    }

    strcpy(message, "Undefined variable '");
    strcat(message, name->lexeme);
    strcat(message, "'.");

    set_runtime_error(inter, name, message);
    return NULL;
}

void assign(Interpreter * inter, Environment * env, Token * name, Value * val) {
    any_t arg = NULL;
    if (hashmap_get(env->env_map, name->lexeme, &arg) == MAP_OK) {
        hashmap_put(env->env_map, name->lexeme, val);
        return;
    }

    if (env->enclosing != NULL) {
        assign(inter, env->enclosing, name, val);
    }

    size_t size = strlen("Undefined variable '") + strlen(name->lexeme)
        + strlen("'.") + 1;

    char * message = malloc(size);
    if (!message) {
        perror("malloc");
        exit(1);
    }

    strcpy(message, "Undefined variable '");
    strcat(message, name->lexeme);
    strcat(message, "'.");

    set_runtime_error(inter, name, message);
}
