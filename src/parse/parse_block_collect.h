/* These functions collect the program's top-level blocks (functions,
 * structs, imports, module declarations, etc) for the Array blocks[5]
 * defined in /src/main.c . All structures used are defined in
 * /src/datastructures/datastructures.h .
 */

#pragma once

#include "../datastructures/datastructures.h"
#include "./parse_type.h"

int traverse_block(Array prog, int start, int end, int open, int close);

struct WFunction* collect_block_function(Array tokens_array, int index);

struct WStruct* collect_block_struct(Array tokens_array, int index);

int collect_blocks(Array tokens_array, Array* blocks);
