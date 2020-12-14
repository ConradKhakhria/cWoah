/* This file contains general utilities that aren't necessary for particular
 * stages of the compilation process.
 */

#include "misc.h"

void print_slice(char* array, int start, int end)
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
        printf("%c", array[i]);
    }
}

void print_type(struct WType* type)
{
    /* Displays a type annotation. */
    struct WType** derivs     = type->derivs;
    struct Token* struct_name = type->derivs;
    int i;

    switch (type->type_form) {
        case TF_LIST:
            print_type(type->derivs);
            printf("[]");
            break;

        case TF_POINTER:
            printf("&");
            print_type(type->derivs);
            break;

        case TF_PARAMETRIC:
            print_slice(
                program_source_buffer, type->name->start_i, type->name->end_i
            );

            printf("<");

            for (i = 0; i < type->num; i++) {
                print_type(derivs[i]);

                if (i < type->num - 1) {
                    printf(", ");
                }
            }

            printf(">");
            break;

        case TF_STRUCT:
            printf("struct ");
            print_slice(
                program_source_buffer, struct_name->start_i, struct_name->end_i
            );
            break;

        case TF_ATOMIC:
            printf("[LIST OF TYPES NOT YET IMPLEMENTED]");
            break;

        default: // This should never happen
            error_message("Unexpected internal error in print_type()\n");
            exit(-1);
    }
}
