/* This file contains general utilities that aren't necessary for particular
 * stages of the compilation process.
 */

#include "misc.h"

#define FPE_COMP_BINOP(_TOKEN_TYPE, _TOKEN_STRING)  \
    case _TOKEN_TYPE:                               \
        fprintf(fd, _TOKEN_STRING);                 \
        break                             


void fprint_slice(FILE* fd, char* string, int start, int end)
{
   /* Prints a slice of a string between start and end.
    *
    * Parameters
    * ----------
    * - char* array: the string containing the slice.
    * 
    * - int start:   the first character in the slice.
    * 
    * - int end:     the first character after the slice.
    *
    * Modifies nothing.
    */
    for (int i = start; i < end; i++) {
        fprintf(fd, "%c", string[i]);
    }
}

void fprint_token(FILE* fd, struct Token* token, char* source)
{
   /* Prints a token's string value
    *
    * Parameters
    * ----------
    * - FILE* fd: the file descriptor to print the token to
    * 
    * - struct Token* token: the token to print
    * 
    * - char* array: the string containing the token
    * 
    * Returns: nothing.
    * -------
    */

    fprint_slice(fd, source, token->start_i, token->end_i);
}

void fprint_type(FILE* fd, struct WType* type)
{
    /* Displays a type annotation. */
    if (type == NULL) {
        fprintf(fd, "none");
        return;
    }

    struct WType** derivs     = type->derivs;
    struct Token* struct_name = type->derivs;
    int i;

    switch (type->type_form) {
        case TF_LIST:
            fprint_type(fd, type->derivs);
            fprintf(fd, "[]");
            break;

        case TF_POINTER:
            fprintf(fd, "&");
            fprint_type(fd, type->derivs);
            break;

        case TF_PARAMETRIC:
            fprint_slice(
                fd,
                program_source_buffer,
                type->name->start_i,
                type->name->end_i
            );

            fprintf(fd, "<");

            for (i = 0; i < type->num; i++) {
                fprint_type(fd, derivs[i]);

                if (i < type->num - 1) {
                    fprintf(fd, ", ");
                }
            }

            fprintf(fd, ">");
            break;

        case TF_STRUCT:
            fprintf(fd, "struct ");
            fprint_slice(
                fd,
                program_source_buffer,
                struct_name->start_i,
                struct_name->end_i
            );

            break;

        case TF_ATOMIC:
            if (type->num == T_NAME) {
                fprint_slice(
                    fd,
                    program_source_buffer,
                    type->name->start_i,
                    type->name->end_i
                );
            } else {
                fprintf(fd, "%s", keywords[type->num - T_bool]);
                fflush(stdout);
            }

            break;

        default: // This should never happen
            error_message("Unexpected internal error in print_type()\n");
            exit(-1);
    }
}

void fprint_parse_expression(FILE* fd, struct ParseExpr* expr, char* source)
{
    int i;

    if (expr == NULL) {
        fprintf(fd, " () ");
        return;
    }

    switch (expr->type) {
        case PET_ATOMIC:
            fprintf(fd, " ATOM [ ");
            fprint_token(fd, expr->expression.atom, source);
            fprintf(fd, " ] ");
            break;

        case PET_ATTR_RESOLUTION:
            fprintf(fd, " ATTR-RES [");
            fprint_parse_expression(fd, expr->expression.at_res.parent_attr, source);
            fprintf(fd, (expr->expression.at_res.is_pointer) ? "] -> [" : "] . [");
            fprint_parse_expression(fd, expr->expression.at_res.child_attr, source);
            fprintf(fd, "] ");

            break;

        case PET_COMPOUND_ARITHMETIC:
        case PET_COMPOUND_BOOLEAN:
            fprintf(fd, " COMPOUND [");
            fprint_parse_expression(fd, expr->expression.derivs[0], source);

            // Get the binary operator
            switch (expr->value) {
                FPE_COMP_BINOP(T_OR,         "or");
                FPE_COMP_BINOP(T_AND,        "and");
                FPE_COMP_BINOP(T_XOR,        "xor");
                FPE_COMP_BINOP(T_NOT,        "not");
                FPE_COMP_BINOP(T_DBL_EQUALS, "==");
                FPE_COMP_BINOP(T_INEQ,       "!=");
                FPE_COMP_BINOP(T_LT,         "<");
                FPE_COMP_BINOP(T_GT,         ">");
                FPE_COMP_BINOP(T_LEQ,        "<=");
                FPE_COMP_BINOP(T_GEQ,        ">=");
                FPE_COMP_BINOP(T_PLUS,       "+");
                FPE_COMP_BINOP(T_MINUS,      "-");
                FPE_COMP_BINOP(T_ASTERISK,   "*");
                FPE_COMP_BINOP(T_FWD_SLASH,  "/");
                FPE_COMP_BINOP(T_PERCENT,    "%%");

                default:
                    error_message("internal error 1 in fprint_parse_expression().\n");
                    exit(-2);
            }

            fprint_parse_expression(fd, expr->expression.derivs[1], source);
            fprintf(fd, "] ");

            break;

        case PET_FUNCTION_CALL:
            fprintf(fd, " FUNCALL ");
            fprint_token(fd, expr->expression.call.function_name, source);
            fprintf(fd, " (");

            for (i = 0; i < expr->expression.call.argument_count; i++) {
                if (i > 0) {
                    fprintf(fd, ",");
                }

                fprint_parse_expression(fd, expr->expression.call.argument_exprs[i], source);
            }

            fprintf(fd, ") ");
            break;

        case PET_LIST_INDEX:
            fprintf(fd, " LIST-INDEX ");
            fprint_token(fd, expr->expression.list_index.list_name, source);
            fprintf(fd, " [ ");
            fprint_parse_expression(fd, expr->expression.list_index.index, source);
            fprintf(fd, " ] ");

            break;

        case PET_MACRO_USE:
            fprintf(fd, " MACRO-USE ");

            switch (expr->expression.macro_use.macro_name) {
                case T_CAST:
                    fprintf(fd, "CAST![");
                    break;

                case T_HEAP:
                    fprintf(fd, "HEAP![");
                    break;

                case T_STACK:
                    fprintf(fd, "STACK![");
                    break;

                default:
                    error_message("Unexpected internal error 3 in fprint_parse_expression().\n");
                    exit(-2);
            }

            fprint_parse_expression(fd, expr->expression.macro_use.derivs, source);

            fprintf(fd, "] ");

            break;

        default:
            error_message("internal error 2 in fprint_parse_expression().\n");
            exit(-2);
    }
}

int get_szbits(struct WType* type, Array* blocks)
{
    /* Returns the size in bytes of a type */
    return get_szwords(type, blocks) * WORD_SIZE_BYTES * 8;
}

int get_szbytes(struct WType* type, Array* blocks)
{
    /* Returns the size in bits of a type */

    if (type->type_form == TF_ATOMIC) {
        switch (type->num) {
            case T_i8:
            case T_u8:
            case T_f8:
            case T_char:
                return 1;
            case T_i16:
            case T_u16:
            case T_f16:
                return 2;
            case T_i32:
            case T_u32:
            case T_f32:
                return 4;
            case T_i64:
            case T_u64:
            case T_f64:
                return 8;
            case T_i128:
            case T_u128:
            case T_f128:
                return 16;
            case T_bool: // The speed improvement of a word-aligned boolean type
            case T_int:  // is well-worth any memory usage benefit coming from
            case T_uint: // using either a bit or a byte in all cases.
            case T_float:
                return WORD_SIZE_BYTES;
            default:
                error_message("Internal error: Unknown type with token code %d\n", type->num);
                exit(-1);
        }
    } else {
        return get_szwords(type, blocks) * WORD_SIZE_BYTES;
    }
}

int get_szwords(struct WType* type, Array* blocks)
{
    /* Returns the size in words of a type */
    struct WStruct** struct_list = (struct WStruct **)blocks[1]->buffer;
    struct WStruct* s;
    int retval = 0;
    int index;

    switch (type->type_form) {
        case TF_LIST: // Technically a pointer
        case TF_POINTER:
            return 1;

        case TF_PARAMETRIC:
            error_message("As type definitions haven't been implemented yet, "
                          "it isn't possible to get the size of a parametric type.\n");

            exit(-1);

        case TF_STRUCT:
            for (index = 0; index < blocks[1]->index; index++) {
                if (token_compare(type->name, struct_list[index]->struct_name)) {
                    s = struct_list[index];
                }
            }

            if (s == NULL) {
                error_message("Unknown struct '");
                fprint_slice(
                    stderr,
                    program_source_buffer,
                    type->name->start_i,
                    type->name->end_i
                );
                fprintf(stderr, "'\n");

                exit(-SYNTAX_ERROR);
            }

            // As all the fields are going to be word-aligned.
            for (index = 0; index < s->field_count; s++) {
                retval += get_szwords(s->field_types[index], blocks);
            }

            return retval;

        case TF_ATOMIC:
            retval = get_szbytes(type, blocks);

            if (retval < WORD_SIZE_BYTES) {
                retval = 1;
            } else {
                retval /= WORD_SIZE_BYTES;
            }

            return retval;

        default:
            error_message("Impossible internal error in get_szwords()\n");
            exit(-1);
    }

    // Never happens
    return retval;
}
