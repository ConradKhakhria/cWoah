/* This file contains general utilities that aren't necessary for particular
 * stages of the compilation process.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "./parse/parse_type.h"

void print_slice(char* array, int start, int end);

void print_type(struct WType* type);
