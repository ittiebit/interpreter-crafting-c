#ifndef ERROR_TYPE_H
#define ERROR_TYPE_H

typedef struct CloxErrors_t {
    char scan_errors;
    char parse_errors;
    char runtime_errors;
} CloxErrors;

#endif //ERROR_TYPE_H
