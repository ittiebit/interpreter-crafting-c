#ifndef ERROR_TYPE_H
#define ERROR_TYPE_H

typedef struct Clox_errors_t {
    char * scan_errors;
    char * parse_errors;
    char * runtime_errors;
} Clox_errors;

#endif //ERROR_TYPE_H
