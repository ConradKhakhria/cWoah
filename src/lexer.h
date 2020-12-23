/* This file contains function headers for tokenising source code.
 *
 * These functions use the struct Token*, which is defined in
 * /src/datastructures/datastructures.h
 */

#pragma once

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "datastructures/datastructures.h"
#include "enums.h"
#include "woah_error.h"

// Expands to condition for matching a keyword
#define match_word_token(_STR, _LEN, _TOK)              \
    else if (state->index - token_start == _LEN         \
    && !strncmp(&source[token_start], _STR, _LEN)) {    \
        state->token->token_type = _TOK;                \
    }       

// Expands to match a single character symbol in tokenise_sym()
#define match_sym(_CHAR, _TOKEN_TYPE_NAME)          \
    case _CHAR:                                     \
        state->token->token_type = _TOKEN_TYPE_NAME;\
        break


// Checks if _CHAR is in [_A, _B]
#define char_match2(_CHAR, _A, _B)     (_CHAR == _A || _CHAR == _B)

// Checks if _CHAR is in [_A, _B, _C]
#define char_match3(_CHAR, _A, _B, _C) (_CHAR == _A || _CHAR == _B || _CHAR == _C)

// Checks if _A <= _CHAR <= _B
#define char_range(_A, _CHAR, _B) (_A <= _CHAR && _CHAR <= _B)

Array generate_tokens(char* source, int source_len);

int eq_type(char* source, int index);

int lt_type(char* source, int index);

int gt_type(char* source, int index);

int add_type(char* source, int index);

int hyp_type(char* source, int index);

int ast_type(char* source, int index);

int fwd_slash_type(char* source, int index);

bool token_compare(struct Token* a, struct Token* b);
