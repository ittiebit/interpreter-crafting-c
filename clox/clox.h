#ifndef CLOX_H_
#define CLOX_H_

#include "token.h"
#include <stddef.h>

void scan_error(size_t line, char * message, char * had_error);
void report(size_t line, char * where, char * message, char * had_error);
void error(Token * token, char * message);

#endif
