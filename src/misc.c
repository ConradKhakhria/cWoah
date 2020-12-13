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
