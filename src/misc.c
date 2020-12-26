/* This file contains general utilities that aren't necessary for particular
 * stages of the compilation process.
 */

#include "misc.h"

void fprint_slice(FILE* fd, char* array, int start, int end)
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
        fprintf(fd, "%c", array[i]);
    }
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
