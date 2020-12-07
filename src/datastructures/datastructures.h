#pragma once

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../constants.h"
#include "../enums.h"
#include "../woah_error.h"

/* Generic datastructures */

// The growable array type, containing both the size of the buffer
// and the highest occupied index.
typedef struct Array {
    void** buffer;
    int buffer_len;
    int index;
}* Array;

Array make_array();

void array_add(Array array, void* val);

void* array_index(Array array, int index);

bool array_contains_string(Array array, char* string, size_t len);

/* Lexing and parsing */

struct Token {
    int token_type;
    int start_i;
    int end_i;
    int line_no;
    int col_no;
};

struct WType {
    int   type_form; // defined in /src/enums.h
    int   num;
    void* derivs;
};

struct WTypedef {
    
};

struct WFunction {
    char*              function_name;
    struct WTypedef*   parent_type;
    char**             arg_names;
    struct WType*      arg_types;
    struct WType       ret_type;
    struct WParseExpr* body;
};
