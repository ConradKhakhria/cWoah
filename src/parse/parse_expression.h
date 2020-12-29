#include <stdbool.h>
#include <stdlib.h>

#include "../constants.h"
#include "../datastructures/datastructures.h"
#include "../enums.h"

// index in int bracket_depths[2]
#define RND 0
#define SQU 1

bool compound_boolean_expression(Array tokens_array, struct BoolExpr* expr, int start, int end);

struct BoolExpr* parse_boolean_expression(Array tokens_array, int start, int end);

struct MathExpr* parse_math_expression(Array tokens_array, int start, int end);
