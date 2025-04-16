#include "./clox.h"
#include <stdio.h>
#include <stdlib.h>

void cerror(size_t line, char * message, char * had_error) {
    report(line, "", message, had_error);
    return;
}

void report(size_t line, char * where, char * message, char * had_error) {
    printf("[line %ld] Error %s: %s\n", line, where, message);

    // NOTE: What to do with had_error????
    if (had_error != NULL) {
        *had_error = 1;
    } else {
        printf("NOTE: had_error is not implemented yet?\n");
    }
    return;
}

