#include "parse_type.h"

// Returns a ptr to a Wtype struct describing a type annotation
struct WType *parse_type(Array tokens_array, int start, int end)
{
    struct Token **tokens = (struct Token **)tokens_array->buffer;
    struct WType *type    = malloc(sizeof(struct WType *));

    printf("Guess who: start = %d, end = %d\n", start, end);

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

struct WType *new_parse_type(Array tokens_array, int start, int end)
{
   /* Recursive descent parser for type annotations
    *
    * Arguments
    * ---------
    * - Array tokens_array: the list of tokens for the whole program
    * - int start: the index of the first token in the type annotation
    * - int end: the index of the last token in the type annotation
    */
    struct WType *type    = malloc(sizeof(struct WType));
    struct Token **tokens = (struct Token **)tokens_array->buffer;

    HANDLEMALLOCERR(type, 5);

    if (end - start > 1 && tokens[end - 1]->token_type == T_OPEN_SQ_BRKT
    && tokens[end - 1]->token_type == T_CLOSE_SQ_BRKT) { // Type[]
        type->type_form = TF_LIST;
        type->derivs    = new_parse_type(tokens_array, start, end - 2);
    } else if (tokens[start]->token_type == T_AMPERSAND) {
        type->type_form = TF_POINTER;

        if (tokens[start + 1]->token_type == T_OPEN_BRKT && tokens[end]->token_type == T_CLOSE_BRKT) {
            type->derivs = new_parse_type(tokens_array, start + 2, end - 1);
        } else {
            type->derivs = new_parse_type(tokens_array, start + 1, end);
        }
    } else if (tokens[start + 1]->token_type == T_LT && tokens[end]->token_type == T_GT) {
        parse_parametric_type(tokens_array, type, start + 2, end);
    } else if (tokens[start]->token_type == T_STRUCT) {
        if (tokens[start + 1]->token_type != T_NAME) {
            WSEPRINTMESG("Expected name after struct declaration\n");
            WSEPRINTLINE(tokens[start + 1]->line_no, tokens[start + 1]->col_no);
        } else if (tokens[start + 2]->token_type == T_LT) {
            WSEPRINTMESG("Parametric structs haven't really been worked out yet..\n");
            WSEPRINTLINE(tokens[start + 2]->line_no, tokens[start + 2]->col_no);
        } else if (end - start != 2) {
            WSEPRINTMESG("Improper struct declaration\n");
            WSEPRINTLINE(tokens[start]->line_no, tokens[start]->col_no);
        }

        int struct_name_len = tokens[start + 2]->end_i - tokens[start + 2]->start_i;

        type->type_form = TF_STRUCT;
        type->derivs    = malloc(sizeof(char) * struct_name_len);
        memcpy(type->derivs, program_source_buffer, struct_name_len);
    } else if (start == end && tokens[start]->token_type == T_NAME) {
        type->type_form = TF_ATOMIC;
    } else {
        WSEPRINTMESG("Completely unrecognised syntax in type annotation\n");
        WSEPRINTLINE(tokens[start]->line_no, tokens[start]->col_no);
    }

    return type;
}

void parse_parametric_type(Array tokens_array, struct WType *type, int start, int end)
{
    struct Token **tokens = (struct Token **)tokens_array->buffer;
    void **derivs         = malloc(sizeof(struct WType *) * (end - start));

    type->type_form = TF_PARAMETRIC;

    HANDLEMALLOCERR(derivs, 6);

    printf("end index in parametric type: %d\n", end);

    // Step through the parameter types
    int i = start, derivs_index = 0;

    while (i < end) { // Iterate on each new parameter entry
        if (tokens[i + 1]->token_type == T_LT) { // If parameter is itself parametric
            int parameter_list_skip = traverse_block(tokens_array, i + 2, end, T_LT, T_GT);

            if (parameter_list_skip == -1) {
                WSEPRINTMESG("mismatched angle brackets in parametric type annotation\n");
                WSEPRINTLINE(tokens[i]->line_no, tokens[i]->col_no);
                exit(SYNTAX_ERROR);
            } else if (tokens[parameter_list_skip]->token_type != T_COMMA && parameter_list_skip < end) {
                WSEPRINTMESG("weird syntax in parametric type annotation\n");
                WSEPRINTLINE(tokens[i]->line_no, tokens[i]->col_no);
                exit(SYNTAX_ERROR);
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
}
