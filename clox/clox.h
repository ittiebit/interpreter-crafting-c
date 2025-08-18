#ifndef CLOX_H
#define CLOX_H

#include "./token.h"
#include <stddef.h>

void scan_error(size_t line, char * message, char * had_error);
void report(size_t line, char * where, char * message, char * p_errors);
void cerror(Token * token, char * message, char * p_errors);

#endif //CLOX_H
