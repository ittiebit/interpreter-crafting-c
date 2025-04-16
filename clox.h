#ifndef CLOX_H_
#define CLOX_H_

#include <stddef.h>

void cerror(size_t line, char * message, char * had_error);
void report(size_t line, char * where, char * message, char * had_error);

#endif
