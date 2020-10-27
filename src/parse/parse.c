#include "parse.h"

// Gets the index of the next token that's not inside a bracketed block
int traverse_block(struct Token *prog[], int start, int end)
{
    int bracket_counter = 1;
    int start_line_no   = prog[start]->line_no;
    int end_line_no     = prog[end]->line_no;

    while (start < end && bracket_counter > 0) {
        switch (prog[start]->token_type) {
        case T_OPEN_CURLY_BRKT:
            bracket_counter++;    
            break;
        case T_CLOSE_CURLY_BRKT:
            bracket_counter--;
            break;
        default:
            break;
        }
        start++;
    }

    if (bracket_counter > 0) {
        fprintf(
            stderr,
            "Woah: Error: Mismatched brackets between lines %d and %d",
            start_line_no, end_line_no
        );
        exit(1);
    }

    return start;
}


