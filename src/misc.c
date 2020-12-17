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
            fprintf(fd, "%s", inbuilt_types[type->num]);
            break;

        default: // This should never happen
            error_message("Unexpected internal error in print_type()\n");
            exit(-1);
    }
}
