/* This file contains functions and macros for displaying error messages
 * and associated information, as well as exit codes for various errors.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "constants.h"

/* Error handling macros */

// This gets the length of a number when it's printed in decimal
#define NUMLEN(x) (int)log10((float)x)

// Prints the details of an error before the erroneous line is printed
// with error_println()
#define error_message(...) do {                                         \
    fprintf(stderr, "Woah: Syntax Error in file '%s'\n=> ", filename);  \
    fprintf(stderr, __VA_ARGS__);                                       \
} while (0)

// The program will panic if there are any errors allocating buffers. The
// _CODE parameter allows us to see where in the program the error occured
#define malloc_error(_BUF, _CODE) do {                              \
    if (_BUF == NULL) {                                             \
        error_message(                                              \
            "internal malloc() error (sorry): error code 0x%x\n",   \
            _CODE                                                   \
        );                                                          \
        exit(1);                                                    \
    }                                                               \
} while (0)

// Error codes
enum ErrorCodes {
    INDEX_OUT_OF_BOUNDS = 1, // so not to exit(0)
    NO_FILE_SUPPLIED,
    UNRECOGNISED_TOKEN,
    SYNTAX_ERROR
};

void error_println(int line_no, int col_no);

static void print_indent(int indent_size);
