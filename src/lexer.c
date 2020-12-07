#include "lexer.h"

// Expands to condition for matching a keyword
#define WORD_MATCH(_STR, _LEN, _TOK)                        \
    else if (!strncmp(_STR, &source[tok->start_i], _LEN)) { \
                tok->token_type = _TOK;                     \
    }                                                       \

Array generate_tokens(char *source, int source_len)
{
   /* Generates the list of tokens from the source code to be parsed.
    *
    * Each struct Token (defined in /src/datastructures/datastructures.c) contains
    * the type of the token, start and end index within program_source_buffer, as
    * well as the line and column number of the token's first character in the
    * actual source file.
    */
    Array tokens = make_array(TOKENS_ARRAY_SIZE);
    int i = 0, line_no = 1, col_no = 1;

    // Iterate on each new token
    while (i < source_len) {
        struct Token *tok = calloc(1, sizeof(struct Token));
        bool token_type_determined = true;

        // Skip all whitespace and comments
        if (source[i] == ' ') {
            i++;
            col_no++;
            continue;
        } else if (source[i] == '/' && source[i + 1] == '*') {
            i += 2;

            while (!(source[i - 1] == '*' && source[i] == '/')) {
                if (source[i++] == '\n') {
                    line_no++;
                    col_no = 1;
                }
            }

            continue;
        } else if (source[i] == '/' && source[i] == '/') {
            while (source[i] != '\n') {
                i++;
            }

            continue;
        }

        tok->start_i = i++;
        tok->col_no  = col_no;

        // Go through symbol tokens
        switch (source[i - 1]) {
            case '\n':
                tok->token_type = T_NEWLINE;
                line_no++;
                col_no = 0;
                break;
            case '(':
                tok->token_type = T_OPEN_BRKT;
                break;
            case ')':
                tok->token_type = T_CLOSE_BRKT;
                break;
            case '[':
                tok->token_type = T_OPEN_SQ_BRKT;
                break;
            case ']':
                tok->token_type = T_CLOSE_SQ_BRKT;
                break;
            case '{':
                tok->token_type = T_OPEN_CURLY_BRKT;
                break;
            case '}':
                tok->token_type = T_CLOSE_CURLY_BRKT;
                break;
            case ',':
                tok->token_type = T_COMMA;
                break;
            case '.':
                tok->token_type = T_FULL_STOP;
                break;
            case ':':
                tok->token_type = T_COLON;
                break;
            case ';':
                tok->token_type = T_SEMICOLON;
                break;
            case '&':
                tok->token_type = T_AMPERSAND;
                break;
            case '@':
                tok->token_type = T_AT_SYM;
                break;
            case '"':
                tok->token_type = T_DBL_QUOT;
                break;
            case '\'':
                tok->token_type = T_SGL_QUOT;
                break;
            case '=':
                if ((tok->token_type = eq_type(source, i - 1)) == T_EQ) {
                    i++;
                }

                break;
            case '<':
                if ((tok->token_type = lt_type(source, i - 1)) == T_LEQ) {
                    i += 1;
                } else if (tok->token_type == T_SWAP) {
                    i += 2;
                }

                break;
            case '>':
                if ((tok->token_type = gt_type(source, i - 1)) == T_GEQ) {
                    i++;
                }

                break;
            case '/':
                if (source[i] == '=') {
                    tok->token_type = T_DIV_ASSIGN;
                    i++;
                } else {
                    tok->token_type = T_FWD_SLASH;
                }

                break;
            case '\\':
                tok->token_type = T_BACKSLASH;
                break;
            case '+':
                if ((tok->token_type = add_type(source, i - 1)) == T_ADD_ASSIGN
                ||   tok->token_type == T_INC) {
                    i++;
                }

                break;
            case '-':
                if ((tok->token_type = hyp_type(source, i - 1)) == T_SUB_ASSIGN
                ||   tok->token_type == T_ARROW
                ||   tok->token_type == T_DEC) {
                    i++;
                }

                break;
            case '*':
                if (source[i] == '=') {
                    tok->token_type = T_MUL_ASSIGN;
                    i++;
                } else {
                    tok->token_type = T_ASTERISK;
                }

                break;
            default:
                token_type_determined = false;
                i--;
                break;
        }

        // Testing if the token is a word
        if (!token_type_determined) {
            int depth = 0;

            while ((96 < source[i] && source[i] < 123)
                || (64 < source[i] && source[i] < 91)
                || (0 < depth && (47 < source[i] && source[i] < 58))
                || source[i] == '_') {
                token_type_determined = true;
                depth++;
                i++;
            }

            if (!strncmp("fn ", &source[tok->start_i], 3))
                tok->token_type    =  T_FN;
            WORD_MATCH("struct ",  7, T_STRUCT)
            WORD_MATCH("use ",     4, T_USE)
            WORD_MATCH("macro ",   6, T_MACRO)
            WORD_MATCH("globals ", 8, T_GLOBALS)
            WORD_MATCH("while ",   6, T_WHILE)
            WORD_MATCH("for ",     4, T_FOR)
            WORD_MATCH("if ",      3, T_IF)
            WORD_MATCH("elif ",    5, T_ELIF)
            WORD_MATCH("else ",    5, T_ELSE)
            WORD_MATCH("in ",      3, T_IN)
            WORD_MATCH("self ",    5, T_SELF)
            WORD_MATCH("return ",  7, T_RETURN)
            WORD_MATCH("stack ",   6, T_STACK)
            WORD_MATCH("heap ",    5, T_HEAP)
            WORD_MATCH("and ",     4, T_AND)
            WORD_MATCH("or ",      3, T_OR)
            WORD_MATCH("xor ",     4, T_XOR)
            WORD_MATCH("true ",    5, T_TRUE)
            WORD_MATCH("false ",   6, T_FALSE)
            WORD_MATCH("not ",     4, T_NOT)
            else
                tok->token_type    =  T_NAME;
        }

        // Testing if the token is a numeric literal
        if (!token_type_determined) {
            if (source[i] == '0' && source[i + 1] == 'b') {
                tok->token_type = T_B2NUM;
                i += 2;

                while (source[i] == '0' || source[i] == '1' || source[i] == '_') {
                    i++;
                }
            } else if (source[i] == '0' && (source[i + 1] == 'x' || source[i + 1] == 'X')) {
                tok->token_type = T_B16NUM;
                i += 2;

                while (('0' <= source[i] && source[i] <= '9')
                    || ('a' <= source[i] && source[i] <= 'f')
                    || ('A' <= source[i] && source[i] <= 'F')
                    || source[i] == '_') {
                        i++;
                }
            } else if (('0' <= source[i] && source[i] <= '9') || source[i] == '_') {
                while (('0' <= source[i] && source[i] <= '9') || source[i] == '_') {
                    i++;
                }
            } else {
                WSEPRINTMESG("completely unrecognised token '%c'\n", source[i]);
                WSEPRINTLINE(line_no, col_no);
                exit(UNRECOGNISED_TOKEN);
            }

            if (('a' <= source[i] && source[i] <= 'z')
             || ('A' <= source[i] && source[i] <= 'Z')) {
                 WSEPRINTMESG("tokens cannot consist of digits followed by letters\n");
                 WSEPRINTLINE(line_no, col_no);
                 exit(SYNTAX_ERROR);
             }
        }

        tok->end_i   = i;
        tok->line_no = line_no;
        col_no += tok->end_i - tok->start_i;
        array_add(tokens, tok);
    }

    return tokens;
}

// is the token = or ==
int eq_type(char *source, int index)
{
    if (source[index + 1] == '=') {
        return T_EQ;
    } else {
        return T_EQUALS;
    }
}

// is the token < or <=
int lt_type(char *source, int index)
{
    switch (source[index + 1]) {
        case '=':
            return T_LEQ;
        case '-':
            if (source[index + 2] == '>') {
                return T_SWAP;
            }
        default:
            return T_LT;
    }
}

// is the token > or >=
int gt_type(char *source, int index)
{
   return (source[index + 1] == '=') ? T_GEQ : T_GT;
}

// is the token +, += or ++
int add_type(char *source, int index)
{
    switch (source[index + 1]) {
        case '=':
            return T_ADD_ASSIGN;
        case '+':
            return T_INC;
        default:
            return T_PLUS;
    }
}

// is the token -, -=, -> or --
int hyp_type(char *source, int index)
{
    switch (source[index + 1]) {
        case '=':
            return T_SUB_ASSIGN;
        case '>':
            return T_ARROW;
        case '-':
            return T_DEC;
        default:
            return T_MINUS;
    }
}
