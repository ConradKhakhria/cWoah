#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../../include/HashMap/hashmap.h"
#include "../constants.h"
#include "../datastructures/datastructures.h"
#include "../enums.h"
#include "parse_block_collect.h"
#include "parse_expression.h"


struct WParseExpr* parse_statement(Array tokens_array, Array* blocks, int start, int end, bool prev_if);

struct WParseExpr** parse_function(Array tokens_array, Array* blocks, Map vars);
