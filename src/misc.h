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

void fprint_type(FILE* fd, struct WType* type);

// int get_type_index(
