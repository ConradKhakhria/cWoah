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

// Returns a ptr to a Wtype struct describing a type annotation
struct WType *parse_type(Array tokens_array, int start, int end)
{
    struct Token **tokens = (struct Token **)tokens_array->buffer;
    struct WType *type    = malloc(sizeof(struct WType *));

    printf("Guess who: start = %d, end = %d\n", start, end);

    MALLOCERR(type, 5);

    if (end - start > 1 && tokens[end - 2]->token_type == T_OPEN_SQ_BRKT
    &&  tokens[end - 1]->token_type == T_CLOSE_SQ_BRKT) {
        printf("Found TF_LIST\n");


        type->type_form = TF_LIST;
        type->derivs    = parse_type(tokens_array, start, end - 2);
    } else if (tokens[start]->token_type == T_AMPERSAND) {
        printf("Found TF_POINTER\n");

        type->type_form = TF_POINTER;
        type->derivs    = parse_type(tokens_array, start + 1, end);
    } else if (tokens[start + 1]->token_type == T_LT && tokens[end - 1]->token_type == T_GT) {
        printf("Found TF_PARAMETRIC\n");


        type->type_form = TF_PARAMETRIC;
        void **derivs   = malloc(sizeof(struct WType *) * (end - start));

        MALLOCERR(derivs, 6);

        // Step through the parameter types
        int i = start + 2, derivs_index = 0;

        while (i < end - 1) { // Iterate on each new parameter entry
            if (tokens[i + 1]->token_type == T_LT) { // If parameter is itself parametric
                int parameter_list_skip = traverse_block(tokens_array, i + 2, end, T_LT, T_GT);

                if (parameter_list_skip == -1) {
                    WSEL1("mismatched angle brackets in parametric type annotation\n");
                    woah_syntax_error(tokens[i]->line_no, tokens[i]->col_no);
                } else { // parameter_list_skip is the index of the '>'
                    derivs[derivs_index++] = parse_type(tokens_array, i, parameter_list_skip - 1);
                }

                i = parameter_list_skip + 1;
            } else {
                int j = i;

                while (j < end && tokens[j]->token_type != T_COMMA) {
                    j++;
                }

                derivs[derivs_index++] = parse_type(tokens_array, i, j);
                i = j + 1;
            }
        }

        type->derivs = derivs;
        type->num    = derivs_index + 1;
    } else if (tokens[start]->token_type == T_STRUCT) {
        printf("Found TF_STRUCT\n");

        type->type_form = TF_STRUCT;

        if (tokens[start + 1]->token_type != T_NAME) {
            WSEL1("expected a name after 'struct'\n");
            woah_syntax_error(tokens[start + 1]->line_no, tokens[start + 1]->col_no);
        }

        type->derivs = tokens[start + 1];
    } else if (end - start == 1 && tokens[start]->token_type == T_NAME) {
        printf("Found TF_ATOMIC\n");


        type->type_form = TF_ATOMIC;
    } else {
        WSEL1("unrecognised syntax in type annotation: token '");

        for (int _p_in = tokens[start]->start_i; _p_in < tokens[start]->end_i; _p_in++) {
            fprintf(stderr, "%c", program_source_buffer[_p_in]);
        }

        fprintf(stderr, "'\n");
        woah_syntax_error(tokens[start]->line_no, tokens[start]->col_no);
    }

    return type;
}

Array collect_blocks(Array tokens)
{
    Array functions = make_array();
    int index = 0;

    while (index < tokens->index) {
        
    }
}
