/* This file contains functions for tokenising source code. */

#include "lexer.h"

struct LexerState {
    struct Token* token;
    int index;
    int line_no;
    int col_no;
};

bool remove_comments(char* source, int source_len, struct LexerState* state)
{
   /* If the input at state->index is a comment or whitespace, this skips it.
    *
    * Parameters
    * ----------
    * - char* source:
    *       String containing the source code.
    * 
    * - int source_len:
    *       The length of char* source.
    * 
    * - struct LexerState* state {
    *       - struct Token* token: the current token.
    *       - int index:           index of the parent generate_tokens().
    *       - int line_no:         line number of the start of the token.
    *       - int col_no:          column number ""   "".
    *   }
    * 
    * Modifies
    * --------
    * All fields in struct LexerState* state if the function is called on
    * comments or whitespace, and nothing otherwise.
    * 
    * Returns
    * -------
    * Whether or not there was any whitespace.
    */
    if (source[state->index] == ' ') {
        state->index  += 1;
        state->col_no += 1;
    } else if (source[state->index] == '/' && source[state->index + 1] == '*') {
        state->index += 2;

        while (source[state->index - 1] != '*' || source[state->index] != '/') {
            if (source[state->index] == '\n') {
                state->line_no += 1;
                state->col_no   = 1;
            } else if (state->index + 1 >= source_len) {
                error_message("unclosed multi-line comment\n");
                error_println(state->token->line_no, state->token->col_no);
                exit(-SYNTAX_ERROR);
            }

            state->index += 1;
        }
    } else if (source[state->index] == '/' && source[state->index + 1] == '/') {
        while (source[state->index] != '\n') {
            state->index += 1;
        }
    } else {
        return false;
    }

    return true;
}

bool tokenise_num(char* source, int len, struct LexerState* state)
{
   /* If the input at index state->index is a number, this tokenises it.
    *
    * Parameters
    * ----------
    * Same as remove_comments()
    * 
    * Modifies
    * --------
    * ""   ""   ""
    * 
    * Returns
    * -------
    * Whether or not the input at state->index was a number (a base-2 / base-16
    * / base-10 integer or a base-10 float).
    */
    if (!('0' <= source[state->index] && source[state->index] <= '9')) {
        return false;
    }

    if (source[state->index] == '0' && char_match2(source[state->index + 1], 'b', 'B')) {
        state->token->token_type = T_B2NUM;
        state->index += 2;

        while (char_match3(source[state->index], '0', '1', '_')) {
            state->index += 1;
        }

        if (state->index - state->token->start_i == 2) {
            error_message("Invalid binary literal.\n");
            error_println(state->line_no, state->col_no);
            exit(-SYNTAX_ERROR);
        }
    } else if (source[state->index] == '0' && char_match2(source[state->index + 1], 'x', 'X')) {
        state->token->token_type = T_B16NUM;
        state->index += 2;

        while (char_range('0', source[state->index], '9')
        ||     char_range('a', source[state->index], 'f')
        ||     char_range('A', source[state->index], 'F')
        ||     source[state->index] == '_') {
            state->index += 1;
        }

        if (state->index - state->token->start_i == 2) {
            error_message("Invalid hexadecimal literal.\n");
            error_println(state->token->line_no, state->token->col_no);
            exit(-SYNTAX_ERROR);
        }
    } else {
        int floating_point_index;

        while (char_range('0', source[state->index], '9') || source[state->index] == '_') {
            state->index += 1;
        }

        if (source[state->index] == '.') {
            floating_point_index = state->index;

            state->token->token_type = T_FLOAT;
            state->index += 1;

            while (char_range('0', source[state->index], '9') || source[state->index] == '_') {
                state->index += 1;
            }

            if (state->index - floating_point_index == 0) {
                error_message("Invalid floating-point literal.\n");
                error_println(state->token->line_no, state->token->col_no);
            }
        } else {
            state->token->token_type = T_B10NUM;
        }
    }

    return true;
}

bool tokenise_word(char* source, int len, struct LexerState* state)
{
   /* If the input at index state->index is a word, this tokenises it.
    * The state->token->token_type field will either be that of a specific
    * keyword or T_NAME for an unknown word.
    *
    * Parameters
    * ----------
    * Same as remove_comments()
    * 
    * Modifies
    * --------
    * ""   ""   ""
    * 
    * Returns
    * -------
    * Whether or not the input was a word.
    */
    bool contains_exclamation = false; // As only macros have a '!' at the end,
                                       // this is needed to check for these edge cases

    if (!char_range('a', source[state->index], 'z')
    &&  !char_range('A', source[state->index], 'Z') && source[state->index] != '_') {
        return false;
    }

    int token_start = state->index;
    state->index   += 1;

    while (char_range('a', source[state->index], 'z') || char_range('A', source[state->index], 'Z')
    ||     char_range('0', source[state->index], '9') || source[state->index] == '_'
    ||     source[state->index] == '!') {
        if (state->index + 1 >= len) {
            error_message("identifier continues until the end of file.\n");
            error_println(state->token->line_no, state->token->col_no);
            exit(-SYNTAX_ERROR);
        }

        if (source[state->index] == '!') {
            contains_exclamation = true;
        }

        state->index += 1;
    }

    if (state->index - token_start == 2
    && !strncmp(&source[token_start], "fn", 2))
        state->token->token_type = T_FN;
    match_word_token("bool",    4, T_bool)
    match_word_token("i8",      2, T_i8)
    match_word_token("i16",     3, T_i16)
    match_word_token("i32",     3, T_i32)
    match_word_token("i64",     3, T_i64)
    match_word_token("i128",    4, T_i128)
    match_word_token("int",     3, T_int)
    match_word_token("i8",      2, T_u8)
    match_word_token("i16",     3, T_u16)
    match_word_token("i32",     3, T_u32)
    match_word_token("i64",     3, T_u64)
    match_word_token("i128",    4, T_u128)
    match_word_token("int",     3, T_uint)
    match_word_token("i8",      2, T_f8)
    match_word_token("i16",     3, T_f16)
    match_word_token("i32",     3, T_f32)
    match_word_token("i64",     3, T_f64)
    match_word_token("i128",    4, T_f128)
    match_word_token("int",     3, T_float)
    match_word_token("char",    4, T_char)
    match_word_token("and",     3, T_AND)
    match_word_token("cast!",   5, T_CAST)
    match_word_token("elif",    4, T_ELIF)
    match_word_token("else",    4, T_ELSE)
    match_word_token("false",   5, T_FALSE)
    match_word_token("for",     3, T_FOR)
    match_word_token("globals", 7, T_GLOBALS)
    match_word_token("heap!",   5, T_HEAP)
    match_word_token("if",      2, T_IF)
    match_word_token("in",      2, T_IN)
    match_word_token("macro",   5, T_MACRO)
    match_word_token("none",    4, T_NONE)
    match_word_token("not",     3, T_NOT)
    match_word_token("null",    4, T_NULL)
    match_word_token("or",      2, T_OR)
    match_word_token("return",  6, T_RETURN)
    match_word_token("self",    4, T_SELF)
    match_word_token("stack!",  6, T_STACK)
    match_word_token("struct",  6, T_STRUCT)
    match_word_token("true",    4, T_TRUE)
    match_word_token("type",    4, T_TYPE)
    match_word_token("use",     3, T_USE)
    match_word_token("while",   5, T_WHILE)
    match_word_token("xor",     3, T_XOR)
    else {
        if (contains_exclamation) {
            error_message("Token cannot contain an exclamation mark if it is not"
                          "a built-in macro.\n");
            error_println(state->line_no, state->col_no);
            exit(-SYNTAX_ERROR);
        } else {
            state->token->token_type = T_NAME;
        }
    } 
        
    return true;
}

bool tokenise_sym(char* source, int source_len, struct LexerState* state)
{
   /* If the input at index state->index is a symbol, this tokenises it.
    *
    * Parameters
    * ----------
    * Same as remove_comments()
    * 
    * Modifies
    * --------
    * ""   ""   ""
    * 
    * Returns
    * -------
    * Whether or not the input was a symbol.
    */
    switch (source[state->index]) {
        match_sym('\n', T_NEWLINE);
        match_sym('(',  T_OPEN_BRKT);
        match_sym(')',  T_CLOSE_BRKT);
        match_sym('[',  T_OPEN_SQ_BRKT);
        match_sym(']',  T_CLOSE_SQ_BRKT);
        match_sym('{',  T_OPEN_CURLY_BRKT);
        match_sym('}',  T_CLOSE_CURLY_BRKT);
        match_sym(',',  T_COMMA);
        match_sym('.',  T_FULL_STOP);
        match_sym(':',  T_COLON);
        match_sym(';',  T_SEMICOLON);
        match_sym('&',  T_AMPERSAND);
        match_sym('@',  T_AT_SYM);
        match_sym('%',  T_PERCENT);

        case '"':
            state->token->token_type = T_DBL_QUOT_STRING;
            state->index += 1;

            while(source[state->index] != '"') {
                if (state->index + 1 >= source_len) {
                    error_message("Unclosed string.\n");
                    error_println(state->token->line_no, state->token->col_no);
                    exit(-SYNTAX_ERROR);
                } else {
                    state->index += 1;
                }
            }

            state->token->end_i = state->index;

            break;

        case '\'':
            state->token->token_type = T_SGL_QUOT_STRING;
            state->index += 1;

            while(source[state->index] != '\'') {
                if (state->index + 1 >= source_len) {
                    error_message("Unclosed string.\n");
                    error_println(state->token->line_no, state->token->col_no);
                    exit(-SYNTAX_ERROR);
                } else {
                    state->index += 1;
                }
            }

            state->token->end_i = state->index;
            state->index       += 1;

            break;

        case '=':
            if (source[state->index + 1] == '=') {
                state->token->token_type = T_DBL_EQUALS;
                state->index += 1;
            } else {
                state->token->token_type = T_SGL_EQUALS;
            }

            break;

        case '<':
            if (source[state->index + 1] == '=') {
                state->token->token_type = T_LEQ;
                state->index += 1;
            } else if (source[state->index + 1] == '-'
              &&       source[state->index + 2] == '>') {
                state->token->token_type = T_SWAP;
                state->index += 2;
            } else {
                state->token->token_type = T_LT;
            }

            break;

        case '>':
            if (source[state->index + 1] == '=') {
                state->token->token_type = T_GEQ;
                state->index += 1;
            } else {
                state->token->token_type = T_GT;
            }

            break;

        case '+':
            if (source[state->index + 1] == '=') {
                state->token->token_type = T_ADD_ASSIGN;
                state->index += 1;
            } else {
                state->token->token_type = T_PLUS;
            }

            break;

        case '-':
            if (source[state->index + 1] == '=') {
                state->token->token_type = T_SUB_ASSIGN;
                state->index += 1;
            } else if (source[state->index + 1] == '>') {
                state->token->token_type = T_ARROW;
                state->index += 1;
            } else {
                state->token->token_type = T_MINUS;
            }

            break;

        case '*':
            if (source[state->index + 1] == '=') {
                state->token->token_type = T_MUL_ASSIGN;
                state->index += 1;
            } else {
                state->token->token_type = T_ASTERISK;
            }

            break;

        case '/':
            if (source[state->index + 1] == '=') {
                state->token->token_type = T_DIV_ASSIGN;
                state->index += 1;
            } else {
                state->token->token_type = T_FWD_SLASH;
            }

            break;

        case '!':
            if (source[state->index + 1] == '=') {
                state->token->token_type = T_INEQ;
                state->index += 1;
            } else {
                error_message("Unrecognised token.\n");
                error_println(state->line_no, state->col_no);
                exit(-1);
            }

        default:
            return false;
    }

    state->index += 1;

    if (state->token->token_type == T_NEWLINE) {
        state->line_no += 1;
        state->col_no   = 1;
    }

    return true;
}

Array generate_tokens(char* source, int source_len)
{
   /* Generates the list of tokens from the source code to be parsed.
    *
    * Parameters
    * ----------
    * - char* source:   a buffer containing source code to be tokenised. A pointer
    *                   is used rather than directly using the global variable
    *                   program_source_buffer because it might be useful to
    *                   tokenise module imports individually.
    * 
    * - int source_len: the length of the char *source buffer.
    * 
    * Returns
    * -------
    * An Array of struct Tokens. Each struct Token (defined in
    * /src/datastructures/datastructures.c) contains the type of the token,
    * start and end index within program_source_buffer, as well as the line and
    * column number of the token's first character in the actual source file.
    */
    Array tokens = make_array();

    struct LexerState state = (struct LexerState) {
        .index   = 0,
        .line_no = 1,
        .col_no  = 1
    };

    // Each iteration lands on a new token.
    while (state.index < source_len) {
        bool token_done = false;
        state.token     = calloc(1, sizeof(struct Token));

        malloc_error(state.token, LEXER_ALLOC_TOKEN);

        state.token->start_i = state.index;
        state.token->line_no = state.line_no;
        state.token->col_no  = state.col_no;

        // Check if it's a comment or whitespace.
        token_done = remove_comments(source, source_len, &state);

        // If not, check if it's a number.
        if (!token_done) {
            token_done = tokenise_num(source, source_len, &state);
        }

        // Then a word.
        if (!token_done) {
            token_done = tokenise_word(source, source_len, &state);
        }

        // Then a symbol.
        if (!token_done) {
            token_done = tokenise_sym(source, source_len, &state);
        }

        if (!token_done) {
            error_message("unrecognised token '%c'\n", source[state.index]);
            error_println(state.token->line_no, state.token->col_no);
            exit(-SYNTAX_ERROR);
        }

        state.token->end_i = state.index;
        state.col_no += state.token->end_i - state.token->start_i;

        // NOTE: THIS IS A TEMPORARY FIX, AS THERE IS SO FAR NO KNOWN NEED TO
        //       REGISTER NEWLINE CHARACTERS. IF AT ANY POINT THIS BECOMES
        //       NECESSARY, THIS SHOULD BE REPLACED.
        if (state.token->token_type != T_NEWLINE) {
            array_add(tokens, state.token);
        }
    }

    return tokens;
}

bool token_compare(struct Token* a, struct Token* b)
{
   /* Compares the string value of each token
    *
    * Parameters
    * ----------
    * - struct Token* a/b: the two tokens to compare.
    * 
    * Returns: whether or not they're equal.
    * -------
    */
    if (a->end_i - a->start_i != b->end_i - b->start_i) {
        return false;
    }

    if (memcmp(&program_source_buffer[a->start_i],
               &program_source_buffer[b->start_i],
               a->end_i - a->start_i)) {
        return false;
    } else {
        return true;
    }
}
