/* This file contains general utilities that aren't necessary for particular
 * stages of the compilation process.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "./parse/parse_type.h"

#define println_slice(...) do { \
    print_slice(__VA_ARGS__);   \
    printf("\n");               \
} while (0)

void print_slice(char* array, int start, int end);

void print_type(struct WType* type);

// int get_type_index(
