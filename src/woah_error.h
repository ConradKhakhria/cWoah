#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h"

#define NUMLEN(x) (int)log10((float)x)
#define WSEL1(...) do {                                                 \
    fprintf(stderr, "Woah: Syntax Error in file '%s'\n=> ", filename);  \
    fprintf(stderr, __VA_ARGS__);                                       \
} while (0)
#define MALLOCERR(_BUF, _CODE) do {                                         \
    if (_BUF == NULL) {                                                     \
        WSEL1("internal malloc() error (sorry): error code 0x%x\n", _CODE); \
        exit(1);                                                            \
    }                                                                       \
} while (0)

void print_indent(int indent_size);

void woah_syntax_error(int line_no, int col_no);
