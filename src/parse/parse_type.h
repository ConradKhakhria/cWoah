#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../constants.h"
#include "../datastructures/datastructures.h"
#include "../enums.h"
#include "../lexer.h"


int traverse_block(Array prog, int start, int end, int open, int close);

Array collect_blocks(Array tokens);

struct WType *parse_type(Array tokens_array, int start, int end);

void parse_parametric_type(Array tokens_array, struct WType *type, int start, int end);
