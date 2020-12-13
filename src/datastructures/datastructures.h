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
    int token_type; /* As defined in /src/enums.h */
    int start_i;    /* The index of the token's first character in the source file */
    int end_i;      /* The index of the token's last character */
    int line_no;    /* The line number of the token */
    int col_no;     /* The column number of the token's firdst character */
};

/* The use of each field defined by the type form:  
 *
 * type form = TF_LIST: T[]
 *  - derivs = struct WType: T
 *  - num    = junk value
 *  - name   = "" ""
 *
 *  TF_POINTER: &T
 *  - derivs = struct WType: T
 *  - num    = junk
 *  - name   = ""  ""
 *
 *  TF_PARAMETRIC: T<x, y, ..>
 *  - derivs = struct WType[]: [x, y, ...]
 *  - num    = len(derivs)
 *  - name   = token for the name
 *
 *  TF_STRUCT: struct foo
 *  - derivs = (struct Token *)name_token
 *  - num    = junk
 *  - name   = ""   ""
 *
 *  TF_ATOMIC: T
 *  - derivs = junk
 *  - num    = index in the list of types
 *  - name   = junk.
 */
struct WType {
    struct Token* name;
    int           type_form; /* defined in /src/enums.h */
    int           num;
    void*         derivs;
};

struct WTypedef {
    
};

struct WFunction {
    struct Token*      function_name; /* The token for the function's name. */
    struct WTypedef*   parent_type;   /* For if the function is a method (NULL if not) */
    struct Token**     arg_names;     /* The (ordered) list of names of the arguments. */
    struct WType**     arg_types;     /* The corresponding types of each arguments. */
    struct WType*      ret_type;      /* The return type (NULL if none) of the function. */
    struct WParseExpr* body;          /* The function body's list of statements. */
    uint_fast32_t      arg_count;     /* The number of arguments. */
    uint_fast32_t      body_len;      /* The length of the function body array. */
};

struct WStruct {
    struct Token*  struct_name; /* Name of the struct. */
    struct Token** field_names; /* Names of the struct's fields. */
    struct WType** field_types; /* Types of the struct's fields. */
    uint_fast32_t  field_count; /* The number of fields the struct has. */
};
