#include "woah_error.h"

void woah_syntax_error(int line_no, int col_no)
{
    int i = 0;

    for (int line_index = 1; line_index < line_no; line_index++) {
        while (content_buffer[i] != '\n') {
            i++;
        }
        i++;
    }

    int margin_indent = NUMLEN(line_no) + 3;

    // Line 1 of error message
    print_indent(margin_indent);
    fprintf(stderr, "|\n");
    // Line 2
    fprintf(stderr, " %d |", line_no);
    while (content_buffer[i] != '\n') {
        fprintf(stderr, "%c", content_buffer[i]);
        i++;
    }
    fprintf(stderr, "\n");
    // Line 3
    print_indent(margin_indent);
    fprintf(stderr, "|");
    print_indent(col_no);
    fprintf(stderr, "^\n");

    exit(1);
}

void print_indent(int indent_size)
{
    for (int i = 0; i < indent_size; i++) {
        fprintf(stderr, " ");
    }
}
