#pragma once

#include <stdbool.h>
#include <stdlib.h>

#include "../constants.h"
#include "../datastructures/datastructures.h"
#include "../enums.h"
#include "parse_block_collect.h"

// index in int bracket_depths[2]
#define RND 0
#define SQU 1

bool compound_expression(Array tokens_array, int* operators, int operator_count,
                         struct ParseExpr* expr, int start, int end);

bool compound_boolean_expression(Array tokens_array, struct ParseExpr* expr, int start, int end);

bool compound_math_expression(Array tokens_array, struct ParseExpr* expr, int start, int end);

bool parse_function_call(Array tokens_array, struct ParseExpr* expr,
                         struct ParseExpr* parent, int start, int end);

bool parse_list_index(Array tokens_array, struct ParseExpr* expr, int start, int end);

bool parse_macro_use(Array tokens_array, struct ParseExpr* expr, int start, int end);

struct ParseExpr* parse_general_expression(Array tokens_array, struct ParseExpr* prev, int start, int end);
