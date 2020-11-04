#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h"

// Error handling macros

#define NUMLEN(x) (int)log10((float)x)
#define WSEL1(...) do {                                                 \
    fprintf(stderr, "Woah: Syntax Error in file '%s'\n=> ", filename);  \
    fprintf(stderr, __VA_ARGS__);                                       \
} while (0)
#define HANDLEMALLOCERR(_BUF, _CODE) do {                                   \
    if (_BUF == NULL) {                                                     \
        WSEL1("internal malloc() error (sorry): error code 0x%x\n", _CODE); \
        exit(1);                                                            \
    }                                                                       \
} while (0)

// Error codes
#define INDEX_OUT_OF_BOUNDS 1
#define NO_FILE_SUPPLIED    2
#define UNRECOGNISED_TOKEN  3
#define SYNTAX_ERROR        4

void print_indent(int indent_size);

void WSEPRINTLINE(int line_no, int col_no);
