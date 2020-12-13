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

/* A struct WFunction contains all of the necessary information about a function.
 *
 * Elements
 * --------
 * - struct Token* function_name:
 *      The token for the function's name.
 * 
 * - struct WTypedef* parent_name:
 *      The type of which this function is a method (NULL if it isn't one).
 * 
 * - struct Token** arg_names:
 *      The (ordered) list of names of the arguments.
 * 
 * - struct WType* arg_types:
 *      The corresponding types of each arguments.
 * 
 * - struct WType ret_type:
 *      The return type (NULL if none) of the function.
 * 
 * - struct WParseExpr* body:
 *      The list of statements comprising the function's body
 * 
 * - uint_fast32_t arg_count:
 *      The number of arguments.
 * 
 * - uint_fast32_t body_len:
 *      The length of the function body array.
 */
struct WFunction {
    struct Token*      function_name;
    struct WTypedef*   parent_type;
    struct Token**     arg_names;
    struct WType**     arg_types;
    struct WType*      ret_type;
    struct WParseExpr* body;
    uint_fast32_t      arg_count;
    uint_fast32_t      body_len;
};
