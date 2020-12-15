#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../constants.h"
#include "../datastructures/datastructures.h"
#include "../enums.h"
#include "../lexer.h"
#include "parse.h"

struct WType* parse_type(Array tokens_array, int start, int end);

void parse_parametric_type(Array tokens_array, struct WType* type, int start, int end);

bool types_equal(struct WType* a, struct WType* b);
