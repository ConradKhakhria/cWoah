#pragma once

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "constants.h"
#include "enums.h"

// Growable array
typedef struct Array {
    void **buffer;
    int buffer_len;
    int index;
} * Array;

Array makeArray();
void arrayAdd(Array array, void *val);
void * arrayIndex(Array array, int index);

// Token structure (for lexer)
struct Token {
    int token_type;
    int start_i;
    int end_i;
    int line_no;
};

// Parse tree

typedef struct ParseNode {
    
} * ParseNode;
