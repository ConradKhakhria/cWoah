#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h"

#define NUMLEN(x) (int)log10((float)x)

void print_indent(int indent_size);

void woah_syntax_error(int line_no, int col_no);
