#include "./clox.h"
#include <stdio.h>

void error(size_t line, char * message, int * had_error) {
    report(line, "", message, had_error);
}

void report(size_t line, char * where, char * message, int * had_error) {
    printf("[line %ld] Error %s: %s", line, where, message);

    // NOTE: What to do with had_error????
    if (had_error != NULL) {
        *had_error = 1;
    } else {
        printf("NOTE: had_error is not implemented yet!\n");
    }
}

