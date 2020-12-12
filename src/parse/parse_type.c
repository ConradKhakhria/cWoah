#include "parse_type.h"

// Returns a ptr to a Wtype struct describing a type annotation
struct WType* parse_type(Array tokens_array, int start, int end)
{
   /* Parses a type annotation
    *
    * Parameters
    * ----------
    * - Array tokens_array: the list of all tokens in the program.
    * 
    * - int start: the index of the first token in the type annotation.
    * 
    * - int end:   the index of the last token in the type annotation plus 1.
    * 
    * Returns
    * -------
    * A pointer to a potentially nested struct WType (defined in
    * /src/datastructures/datastructures.h).
    * 
    * Notes
    * -----
    * This function isn't fully functional yet as there are some problems in
    * parsing parametric type annotations e.g. 'HashMap<int, int[]>', so the
    * function prints what it encounters.
    */
    struct Token** tokens = (struct Token **)tokens_array->buffer;
    struct WType* type    = malloc(sizeof(struct WType *));

    HANDLEMALLOCERR(type, 5);

    if (end - start > 1 && tokens[end - 2]->token_type == T_OPEN_SQ_BRKT
    &&  tokens[end - 1]->token_type == T_CLOSE_SQ_BRKT) {
        printf("Found TF_LIST\n");


        type->type_form = TF_LIST;
        type->derivs    = parse_type(tokens_array, start, end - 2);
    } else if (tokens[start]->token_type == T_AMPERSAND) {
        printf("Found TF_POINTER\n");

        type->type_form = TF_POINTER;

        if (tokens[start + 1]->token_type == T_OPEN_BRKT
         && tokens[end - 1]->token_type == T_CLOSE_BRKT) {
            type->derivs = parse_type(tokens_array, start + 2, end - 1);
        } else {
            type->derivs = parse_type(tokens_array, start + 1, end);
        }
    } else if (tokens[start + 1]->token_type == T_LT && tokens[end - 1]->token_type == T_GT) {
        printf("Found TF_PARAMETRIC\n");

        parse_parametric_type(tokens_array, type, start + 2, start + end - 2);
    } else if (tokens[start]->token_type == T_STRUCT) {
        printf("Found TF_STRUCT\n");

        type->type_form = TF_STRUCT;

        if (tokens[start + 1]->token_type != T_NAME) {
            WSEPRINTMESG("expected a name after 'struct'\n");
            WSEPRINTLINE(tokens[start + 1]->line_no, tokens[start + 1]->col_no);
            exit(SYNTAX_ERROR);
        }

        type->derivs = tokens[start + 1];
    } else if (end - start == 1 && tokens[start]->token_type == T_NAME) {
        printf("Found TF_ATOMIC\n");

        type->type_form = TF_ATOMIC;
    } else {
        WSEPRINTMESG("unrecognised syntax in type annotation: token '");

        for (int _p_in = tokens[start]->start_i; _p_in < tokens[start]->end_i; _p_in++) {
            fprintf(stderr, "%c", program_source_buffer[_p_in]);
        }

        fprintf(stderr, "'\n");
        WSEPRINTLINE(tokens[start]->line_no, tokens[start]->col_no);
        exit(SYNTAX_ERROR);
    }

    return type;
}

struct WType* new_parse_type(Array tokens_array, int start, int end)
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

    HANDLEMALLOCERR(type, TYPE_STRUCT);

    if (start == end && tokens[start]->token_type == T_NAME) {
        type->type_form = TF_ATOMIC;
        type->num       = 42; // Will implement later.
    } else if (tokens[end - 1]->token_type == T_OPEN_SQ_BRKT
      && tokens[end]->token_type == T_CLOSE_SQ_BRKT) {
        type->type_form = TF_LIST;
        type->derivs    = (struct WType *)new_parse_type(tokens_array, start, end - 2);
    } else if (tokens[start]->token_type == T_AMPERSAND) {
        type->type_form = TF_POINTER;
        type->derivs    = (struct WType *)new_parse_type(tokens_array, start + 1, end);
    } else if (tokens[start + 1]->token_type == T_LT
      && tokens[end]->token_type == T_GT) {
        parse_parametric_type(tokens_array, type, start + 2, end);
    } else if (tokens[start]->token_type == T_STRUCT) {
        type->type_form = TF_STRUCT;
        type->derivs    = (struct Token *)tokens[start + 1];
    } else {
        WSEPRINTMESG("unrecognised syntax in type annotation: token '");

        for (int _p_in = tokens[start]->start_i; _p_in < tokens[start]->end_i; _p_in++) {
            fprintf(stderr, "%c", program_source_buffer[_p_in]);
        }

        fprintf(stderr, "'\n");
        WSEPRINTLINE(tokens[start]->line_no, tokens[start]->col_no);
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

    HANDLEMALLOCERR(derivs, PARAMETRIC_TYPE_DERIVS);

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

        derivs[derivs_index] = new_parse_type(tokens_array, tokens_index, slice_end - 1);
        derivs_index += 1;

        tokens_index = slice_end + 1;
    }

    type->type_form = TF_PARAMETRIC;
    type->derivs    = derivs;
    type->num       = derivs_index;
}
