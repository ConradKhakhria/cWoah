#include "parse.h"

// Gets the index of the next token that's not inside a bracketed block
int traverse_block(Array prog, int start, int end, int open, int close)
{
    struct Token **tokens = (struct Token **)prog->buffer;
    int bracket_level     = 1;

    for (;start < end && bracket_level > 0; start++) {
        printf("%d, ", bracket_level);

        if (tokens[start]->token_type == T_OPEN_CURLY_BRKT) {
            bracket_level++;
        } else if (tokens[start]->token_type == T_CLOSE_CURLY_BRKT) {
            bracket_level--;
        }
    }

    return (bracket_level > 0) ? -1 : start;
}

Array collect_blocks(Array tokens)
{
    Array functions = make_array();
    int index = 0;

    while (index < tokens->index) {
        
    }

    return functions;
}
