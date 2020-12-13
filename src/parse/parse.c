#include "parse.h"

int traverse_block(Array prog, int start, int end, int t_open, int t_close)
{
   /* Gets the index of the next token that's not inside a bracketed block
    *
    * Parameters
    * ----------
    * - Array prog: the Array of tokens that the program consists of.
    * 
    * - int start:  The index of the first element inside a bracketed
    *               block. For example in the block:
    * 
    *               for i: int in 0 : array.len() {
    *                   io.println("{}", array[i]);    
    *               }
    * 
    *               the first token inside the block would be T_NAME 'io'.
    * 
    * - int end:    The index of the end of the array slice. NOTE: this means
    *               that traverse_block() will not travel to or beyond this
    *               index and will return -1 if it is reached.
    * 
    * - int open:   The TokenType of the opening delimiter. In the above example,
    *               int open = T_OPEN_CURLY_BRKT and int close = T_CLOSE_CURLY_BRKT.
    * 
    * - int close:  ""          ""          ""          ""
    * 
    * Returns: the index of the closing token delimiting the block
    * ------- 
    */
    struct Token** tokens = (struct Token **)prog->buffer;
    int bracket_depth     = 1;
    int index = start;

    while (index < end && bracket_depth > 0) {
        if (tokens[index]->token_type == t_open) {
            bracket_depth += 1;
        } else if (tokens[index]->token_type == t_close) {
            bracket_depth -= 1;
        }

        index += 1;
    }

    if (bracket_depth > 0) { // i.e. unbalanced brackets.
        return -1;
    } else {
        return index - 1; // As it goes one beyond the last close.
    }
}

struct WFunction* collect_block_function(Array tokens_array, int index)
{
    struct WFunction* function = malloc(sizeof(struct WFunction));
    struct Token**    tokens   = (struct Token **)tokens_array->buffer;

    HANDLEMALLOCERR(function, COLLECT_BLOCK_FUNCTION_STRUCT);

    function->arg_names = malloc(sizeof(struct Token *) * 10);
    function->arg_types = malloc(sizeof(struct WType *) * 10);
    function->arg_count = 0;
    function->body_len  = 0;

    HANDLEMALLOCERR(function->arg_names, COLLECT_BLOCK_FUNCTION_ARG_NAMES);
    HANDLEMALLOCERR(function->arg_types, COLLECT_BLOCK_FUNCTION_ARG_TYPES);

    // This gets the function name.
    if (tokens[index + 1]->token_type == T_NAME) {
        // is the function a method?
        if (tokens[index + 2]->token_type == T_FULL_STOP) {
            WSEPRINTMESG("Function methods aren't implemented yet :(\n");
            WSEPRINTLINE(tokens[index + 2]->line_no, tokens[index + 2]->col_no);
            exit(SYNTAX_ERROR);

            index += 5;
        } else {
            function->function_name = tokens[index + 1];

            index += 3;
        }
    } else {
        WSEPRINTMESG("Syntax Error: bad function name\n");
        WSEPRINTLINE(tokens[index + 1]->line_no, tokens[index + 1]->col_no);
        exit(SYNTAX_ERROR);
    }

    // This gets the function arguments.
    if (tokens[index - 1]->token_type != T_OPEN_BRKT) {
        WSEPRINTMESG("Expected a list of arguments after the function name\n");
        WSEPRINTLINE(tokens[index - 1]->line_no, tokens[index - 1]->col_no);
        exit(SYNTAX_ERROR);
    }

    // Each iteration lands on a new argument.
    while (index < tokens_array->index && tokens[index]->token_type != T_CLOSE_BRKT) {
        int type_end = index + 2;

        if (tokens[index]->token_type != T_NAME) {
            WSEPRINTMESG("Expected an argument name.\n");
            WSEPRINTLINE(tokens[index]->line_no, tokens[index]->col_no);
            exit(SYNTAX_ERROR);
        }

        if (tokens[index + 1]->token_type != T_COLON) {
            WSEPRINTMESG("Expected argument type annotation after name\n");
            WSEPRINTLINE(tokens[index + 1]->line_no, tokens[index + 1]->col_no);
            exit(SYNTAX_ERROR);
        }

        index += 2;

        // In case theres a *lot* of arguments
        if (function->arg_count > 0 && function->arg_count % 10 == 0) {
            struct Token** new_arg_names = realloc(
                function->arg_names,
                function->arg_count + 10
            );

            struct WType** new_arg_types = realloc(
                function->arg_types,
                function->arg_count + 10
            );

            HANDLEMALLOCERR(new_arg_names, COLLECT_BLOCK_FUNCTION_NAMES_REALLOC);
            HANDLEMALLOCERR(new_arg_types, COLLECT_BLOCK_FUNCTION_TYPES_REALLOC);

            function->arg_names = new_arg_names;
            function->arg_types = new_arg_types;
        }

        // Get to the next argument or end of argument list
        while (tokens[type_end]->token_type != T_COMMA
        &&     tokens[type_end]->token_type != T_CLOSE_BRKT) {
            type_end += 1;
        }

        function->arg_names[function->arg_count] = tokens[index];
        function->arg_types[function->arg_count] = parse_type(
            tokens_array, index, type_end - 1
        );

        function->arg_count += 1;

        if (tokens[type_end]->token_type == T_CLOSE_BRKT) {
            index = type_end;
        } else {
            index = type_end + 1;
        }
    }

    // Get return type
    index += 1;

    if (tokens[index]->token_type != T_ARROW) {
        WSEPRINTMESG("Expected return type.\n");
        WSEPRINTLINE(tokens[index]->line_no, tokens[index]->col_no);
        exit(SYNTAX_ERROR);
    }

    int function_body_start = index;

    while (tokens[function_body_start]->token_type != T_OPEN_CURLY_BRKT) {
        function_body_start += 1;
    }

    function->ret_type = parse_type(tokens_array, index + 1, function_body_start - 1);
    function->body     = (struct WParseExpr *)(function_body_start + 1);

    // The above is a bit ugly - essentially this gives the index in tokens_array
    // of the start of the function body, rather than lumping the parsing of the
    // function body into this function.

    return function;
}

int collect_blocks(Array tokens_array, Array* blocks)
{
   /* This will collect all of the blocks defined in the source program.
    *
    * Parameters
    * ----------
    * - Array tokens:    the Array containing the program tokens.
    * 
    * - Array* blocks:   the list of Array types to modify
    * 
    * Modifies
    * -------
    * A list containing:
    * - Array functions: all functions defined in the program.
    * - Array structs:   all structs.
    * - Array types:     all typedefs.
    * - Array module:    in case the source file is defined as (part of) a module,
    *                    this contains all of the above to be exported.
    * - Array globals:   all global variables used in the program.
    * 
    * Returns
    * -------
    * 0 if successful, an error code if any (non-fatal) errors occur
    */
    struct Token** tokens = (struct Token **)tokens_array->buffer;
    int index  = 0;

    // Each iteration lands on a new top-level block
    // NOTE: this might in the future need to be modified to collect or ignore
    // any other kinds of top-level declaration (for example generics which
    // I'm not going to implement yet)
    while (index < tokens_array->index) {
        switch (tokens[index]->token_type) {
            case T_FN:
                array_add(blocks[0], collect_block_function(tokens_array, index));
                break;

            case T_NEWLINE:
                index += 1;
                continue;   // Skip this token completely.

            default:
                WSEPRINTMESG("Unrecognised syntax in top-level block\n");
                WSEPRINTLINE(tokens[index]->line_no, tokens[index]->col_no);
                exit(SYNTAX_ERROR);
        }

        // Set index to the start of the next block.
        while (tokens[index]->token_type != T_OPEN_CURLY_BRKT) {
            index += 1;
        }

        index = traverse_block(
            tokens_array, index + 1, tokens_array->index,
            T_OPEN_CURLY_BRKT, T_CLOSE_CURLY_BRKT
        );

        if (index == -1) {
            WSEPRINTMESG("Unbalanced brackets in top-level block.\n");
            exit(SYNTAX_ERROR);
        } else {
            index += 1; // as traverse_block() returns the index of the closing '}'
        }
    }

    return 0;
}
