#include "parse_type.h"

struct WType* parse_type(Array tokens_array, int start, int end)
{
   /* Recursive descent parser for type annotations
    *
    * Parameters
    * ---------
    * - Array tokens_array: the list of tokens for the whole program
    * 
    * - int start: the index of the first token in the type annotation
    * 
    * - int end: the index of the last token in the type annotation
    * 
    * Returns
    * -------
    * A pointer to a potentially nested struct WType (defined in
    * /src/datastructures/datastructures.h).
    * 
    * Notes
    * -----
    * This is an attempt to fix parse_type(), and if successful will replace it.
    */
    struct WType* type    = malloc(sizeof(struct WType));
    struct Token** tokens = (struct Token **)tokens_array->buffer;

    malloc_error(type, TYPE_STRUCT);

    if (start == end) {
        type->type_form = TF_ATOMIC;

        if (T_i8 <= tokens[start]->token_type && tokens[start]->token_type <= T_char) {
            type->num = tokens[start]->token_type;
        } else if (tokens[start]->token_type == T_NAME) {
            type->num  = T_NAME;
            type->name = tokens[start];
        } else {
            error_message("Expected type name.\n");
            error_println(tokens[start]->line_no, tokens[start]->col_no);
            exit(-SYNTAX_ERROR);
        }
    } else if (tokens[end - 1]->token_type == T_OPEN_SQ_BRKT
      && tokens[end]->token_type == T_CLOSE_SQ_BRKT) {
        type->type_form = TF_LIST;
        type->derivs    = (struct WType *)parse_type(tokens_array, start, end - 2);
    } else if (tokens[start]->token_type == T_AMPERSAND) {
        type->type_form = TF_POINTER;
        type->derivs    = (struct WType *)parse_type(tokens_array, start + 1, end);
    } else if (tokens[start + 1]->token_type == T_LT
      && tokens[end]->token_type == T_GT) {
        parse_parametric_type(tokens_array, type, start + 2, end);
    } else if (tokens[start]->token_type == T_STRUCT) {
        type->type_form = TF_STRUCT;
        type->derivs    = (struct Token *)tokens[start + 1];
    } else {
        error_message("unrecognised syntax in type annotation: token '");

        for (int _p_in = tokens[start]->start_i; _p_in < tokens[start]->end_i; _p_in++) {
            fprintf(stderr, "%c", program_source_buffer[_p_in]);
        }

        fprintf(stderr, "'\nPerhaps you have unbalanced angle brackets, or " \
                           "are using a keyword as a type name.\n");

        error_println(tokens[start]->line_no, tokens[start]->col_no);
        exit(SYNTAX_ERROR);
    }

    return type;
}

void parse_parametric_type(Array tokens_array, struct WType* type, int start, int end)
{
   /* Parses a parametric type.
    *
    * This is actually quite difficult which is why it needs its own function.
    * 
    * Parameters
    * ----------
    * - Array tokens_array: the program tokenised.
    * 
    * - struct WType* type: the TF_PARAMETRIC type struct to modify.
    * 
    * - int start: index of the first parameter in the type annotation.
    * 
    * - int end:   index of the closing angle bracket in the type annotation.
    * 
    * Modifies
    * --------
    * struct WType* type - rather than returning it, parse_parametric_type just
    * adds in the struct fields that haven't been covered yet
    */
    struct Token** tokens = (struct Token **)tokens_array->buffer;
    void** derivs    = malloc(sizeof(struct WType *) * (end - start));
    int derivs_index = 0;
    int tokens_index = start;

    malloc_error(derivs, PARAMETRIC_TYPE_DERIVS);

    type->name      = tokens[start - 2];
    type->type_form = TF_PARAMETRIC;

    // each iteration should land on a new parameter.
    while (tokens_index < end) {
        int slice_end     = tokens_index;
        int bracket_depth = 0;

        // slice_end will land on the delimiting comma or the end of
        // the parametric type altogether.
        while (bracket_depth > 0 || (slice_end < end && tokens[slice_end]->token_type != T_COMMA)) {
            if (tokens[slice_end]->token_type == T_LT) {
                bracket_depth += 1;
            } else if (tokens[slice_end]->token_type == T_GT) {
                bracket_depth -= 1;
            }

            slice_end += 1;
        }

        derivs[derivs_index] = parse_type(tokens_array, tokens_index, slice_end - 1);
        derivs_index += 1;

        tokens_index = slice_end + 1;
    }

    type->derivs    = derivs;
    type->num       = derivs_index;
}

bool types_equal(struct WType* a, struct WType* b)
{
   /* Recursively assesses whether two type structs are equal
    *
    * Parameters
    * ----------
    * struct WType* a/b: the types to be compared.
    */
    struct WType** a_derivs = a->derivs; // This could be entirely wrong.
    struct WType** b_derivs = b->derivs;

    if (a->type_form != b->type_form) {
        return false;
    }

    switch (a->type_form) {
        case TF_LIST:
        case TF_POINTER:
            return types_equal(a->derivs, b->derivs);

        case TF_PARAMETRIC:
            if (a->num != b->num) {
                return false;
            }

            for (int i = 0; i < a->num; i++) {
                if (!types_equal(a_derivs[i], b_derivs[i])) {
                    return false;
                }
            }

            return true;

        case TF_STRUCT:
            return types_equal(a->derivs, b->derivs);

        case TF_ATOMIC:
            return (bool)(a->num == b->num);

        default:
            error_message("Internal error in function types_equal()\n");
            exit(-1);
    }
}

int get_atomic_type(struct Token* token)
{
    char blank_string[1000];

    for (int i = 0; i < INBUILT_TYPES_COUNT; i++) {
        memset(blank_string, 0, 1000);

        memcpy(
            blank_string,
            &program_source_buffer[token->start_i],
            token->end_i - token->start_i
        );

        if (!strcmp(blank_string, keywords[i])) {
            return i;
        }
    }

    return -1;
}
