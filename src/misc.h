/* This file contains general utilities that aren't necessary for particular
 * stages of the compilation process.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "./parse/parse_type.h"

#define fprintln_slice(__FD, __ARRAY, __START, __END) do {  \
    fprint_slice(__FD, __ARRAY, __START, __END);            \
    fprintf(__FD, "\n");                                    \
} while (0)

void fprint_slice(FILE* fd, char* array, int start, int end);

void fprint_token(FILE* fd, struct Token* token, char* source);

void fprint_type(FILE* fd, struct WType* type);

void fprint_parse_expression(FILE* fd, struct ParseExpr* expr, char* source);

int get_szbits(struct WType* type, Array* blocks);

int get_szbytes(struct WType* type, Array* blocks);

int get_szwords(struct WType* type, Array* blocks);
