#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./interpreter.h" // <-- typedef struct Environment here
#include "./token.h"
#include "../utils/hashmap.h"

Environment * new_environment(Interpreter * inter) {
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

    env->value_list = NULL;
    env->enclosing = NULL;

    add_environment(&inter->env_list, env);
    return env;
}

Environment * new_enclosing_environment(Interpreter * inter, Environment * env) {
    Environment * enclosing_env = new_environment(inter);
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
    if (env->value_list != NULL) {
        free_value_list(env->value_list);
        free(env->value_list);
        env->value_list = NULL;
    }
    if (env->env_map != NULL) {
        hashmap_free(env->env_map);
        env->env_map = NULL;
    }
    env->enclosing = NULL;
    free(env);
}

void define(Environment * env, char * name, Value * val) {
    int map_err = hashmap_put(env->env_map, name, val);
    add_value(&env->value_list, val);
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
        add_value(&env->value_list, val);
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






void add_value(ValueList ** value_list, Value * p_value) {
    if (value_list == NULL || p_value == NULL) {
        return;
    }

    if (*value_list == NULL) {
        *value_list = malloc(sizeof(ValueList));
        if (*value_list == NULL) {
            exit(1);
        }
        (*value_list)->head = NULL;
    }

    ValueNode * cur = (*value_list)->head;

    if (cur == NULL) {
        cur = malloc(sizeof(ValueNode));
        if (cur == NULL) exit(1);
        cur->value = p_value;
        cur->next = NULL;
        (*value_list)->head = cur;
        return;
    }

    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = malloc(sizeof(ValueNode));
    if (cur->next == NULL) exit(1);
    cur->next->value = p_value;
    cur->next->next = NULL;
}

void free_value_list(ValueList * value_list) {
    if (value_list == NULL) {
        return;
    }

    ValueNode * cur = value_list->head;
    while (cur != NULL) {
        ValueNode * next = cur->next;
        free_value(cur->value);
        free(cur);
        cur = next;
    }
}


void add_environment(EnvironmentList ** environment_list, Environment * p_environment) {
    if (environment_list == NULL || p_environment == NULL) {
        return;
    }

    if (*environment_list == NULL) {
        *environment_list = malloc(sizeof(EnvironmentList));
        if (*environment_list == NULL) {
            exit(1);
        }
        (*environment_list)->head = NULL;
    }

    EnvironmentNode * cur = (*environment_list)->head;

    if (cur == NULL) {
        cur = malloc(sizeof(EnvironmentNode));
        if (cur == NULL) exit(1);
        cur->env = p_environment;
        cur->next = NULL;
        (*environment_list)->head = cur;
        return;
    }

    while (cur->next != NULL) {
        cur = cur->next;
    }
    cur->next = malloc(sizeof(EnvironmentNode));
    if (cur->next == NULL) exit(1);
    cur->next->env = p_environment;
    cur->next->next = NULL;
}

void free_environment_list(EnvironmentList * environment_list) {
    if (environment_list == NULL) {
        return;
    }

    EnvironmentNode * cur = environment_list->head;
    while (cur != NULL) {
        EnvironmentNode * next = cur->next;
        free_environment(cur->env);
        free(cur);
        cur = next;
    }
}
