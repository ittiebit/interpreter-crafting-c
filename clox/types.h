#ifndef TYPES_H
#define TYPES_H

#define VALUE_TRUE 1
#define VALUE_FALSE 0
typedef char boolean;

typedef enum Value_Type_t {
    VAL_NIL,
    VAL_BOOLEAN,
    VAL_DOUBLE,
    VAL_STRING,
} Value_Type;

typedef struct Value_t {
    void * value;
    Value_Type type;
} Value;

#endif //TYPES_H
