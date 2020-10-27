#pragma once

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../constants.h"
#include "../enums.h"

/* Generic datastructures */

// Growable array
typedef struct Array {
    void **buffer;
    int buffer_len;
    int index;
} * Array;

Array makeArray();
void arrayAdd(Array array, void *val);
void * arrayIndex(Array array, int index);


/* Lexing and parsing */

struct Token {
    int token_type;
    int start_i;
    int end_i;
    int line_no;
};

// TODO
struct WType {

};

struct WTypedef {
    
};

struct WFunction {
    char *function_name;
    struct WTypedef *parent_type;
    char **arg_names;
    struct WType *arg_types;
    struct WType ret_type;
    struct WParseExpr *body;
};
