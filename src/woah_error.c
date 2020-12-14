/* This file contains functions and macros for displaying error messages
 * and associated information. 
 */

#include "woah_error.h"

void error_println(int line_no, int col_no)
{
    /* Prints the line and column number of the token that caused an error */
    int i = 0;

    for (int line_index = 1; line_index < line_no; line_index++) {
        while (program_source_buffer[i] != '\n') {
            i++;
        }
        i++;
    }

    int margin_indent = NUMLEN(line_no) + 3;
    int line_indent   = 0;

    // Line 1 of error message
    print_indent(margin_indent);
    fprintf(stderr, "|\n");

    // Line 2
    fprintf(stderr, " %d | ", line_no);

    while (program_source_buffer[i] == ' ') {
        i++;
        line_indent++;
    }

    while (program_source_buffer[i] != '\n' && program_source_buffer[i] != '\x00') {
        fprintf(stderr, "%c", program_source_buffer[i]);
        i++;
    }
    fprintf(stderr, "\n");

    // Line 3
    print_indent(margin_indent);
    fprintf(stderr, "|");
    print_indent(col_no - line_indent);
    fprintf(stderr, "^\n");
}

void print_indent(int indent_size)
{
    /* So that the margin of the line being shown is right. */
    for (int i = 0; i < indent_size; i++) {
        fprintf(stderr, " ");
    }
}
