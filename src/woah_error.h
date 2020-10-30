#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h"

#define NUMLEN(x) (int)log10((float)x)
#define WSEL1(...) do {                                                 \
    fprintf(stderr, "Woah: Syntax Error in file '%s'\n=> ", filename);    \
    fprintf(stderr, __VA_ARGS__);                                       \
} while (0)


void print_indent(int indent_size);

void woah_syntax_error(int line_no, int col_no);
