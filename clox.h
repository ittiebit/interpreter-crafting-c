#ifndef CLOX_H_
#define CLOX_H_

#include <stddef.h>

void error(size_t line, char * message, int * had_error);
void report(size_t line, char * where, char * message, int * had_error);

#endif
