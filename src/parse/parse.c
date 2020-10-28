#include "parse.h"

// Gets the index of the next token that's not inside a bracketed block
int traverse_block(Array prog, int start, int open, int close)
{
    struct Token **tokens = (struct Token **)prog->buffer;
    int bracket_level     = 1;
    int end = prog->index;

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

// Returns a ptr to a Wtype struct describing a type annotation
struct WType *parse_type(Array tokens_array, int start, int end)
{
    struct Token **tokens = (struct Token **)tokens_array->buffer;
    struct WType *type    = malloc(sizeof(struct WType *));

    if (tokens[end - 1]->token_type == T_OPEN_CURLY_BRKT
     && tokens[end]->token_type == T_CLOSE_SQ_BRKT) {
        type->type_form = TF_LIST;
        type->derivs    = parse_type(tokens_array, start, end - 2);
    } else if (tokens[start]->token_type == T_AMPERSAND) {
        type->type_form = TF_POINTER;
        type->derivs    = parse_type(tokens_array, start + 1, end);
    } else if (tokens[start + 1]->token_type == T_LT && tokens[end]->token_type == T_GT) {
        type->type_form = TF_PARAMETRIC;
        void **derivs   = malloc(sizeof(struct WType *) * (end - start));

        // Step through the parameter types
        int i = start + 2, derivs_index = 0;
        int j;

        // Iterates on each parameter type
        while (i < end) {
            j = i;

            while (j < end && tokens[j]->token_type != T_COMMA) {
                if (tokens[j]->token_type == T_LT) { // Skip large parametric blocks
                    j = traverse_block(tokens_array, j + 1, T_LT, T_GT);

                    if (j == -1) { // In case of mismatched triangle brackets
                        fprintf(
                            stderr, 
                            "Woah: Syntax Error: mismatched angle brackets in parametric type annotation\n"
                        );
                        woah_syntax_error(tokens[i]->line_no, tokens[i]->col_no);
                    }
                }

                j++;
            }

            if (tokens[j]->token_type != T_COMMA) { // i.e. !(j < end) short circuit
                fprintf(stderr, "Woah: Syntax Error: incomplete type annotation\n");
                woah_syntax_error(tokens[i]->line_no, tokens[i]->col_no);
            }

            derivs[derivs_index++] = parse_type(tokens_array, i, j);
            i = j + 1; // As j lands on an instance of T_COMMA
        }

    } //TODO: implement case for all atomic forms (structs and basic)

    return type;
}

Array collect_blocks(Array tokens)
{
    Array functions = make_array();
    int index = 0;

    while (index < tokens->index) {
        
    }
}
