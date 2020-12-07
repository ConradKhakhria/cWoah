#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h"

/* Error handling macros */

// This gets the length of a number when it's printed in decimal
#define NUMLEN(x) (int)log10((float)x)

// Prints the details of an error before the erroneous line is printed
// with WSEPRINTLINE()
#define WSEPRINTMESG(...) do {                                          \
    fprintf(stderr, "Woah: Syntax Error in file '%s'\n=> ", filename);  \
    fprintf(stderr, __VA_ARGS__);                                       \
} while (0)

// The program will panic if there are any errors allocating buffers. The
// _CODE parameter allows us to see where in the program the error occured
#define HANDLEMALLOCERR(_BUF, _CODE) do {                                   \
    if (_BUF == NULL) {                                                     \
        WSEPRINTMESG("internal malloc() error (sorry): error code 0x%x\n", _CODE); \
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
