/* This links all of the parsing stuff together */
#pragma once

#include "../datastructures/datastructures.h"
#include "./parse_type.h"

int traverse_block(Array prog, int start, int end, int open, int close);

struct WFunction* collect_block_function(Array tokens_array, int index);

int collect_blocks(Array tokens_array, Array* blocks);
