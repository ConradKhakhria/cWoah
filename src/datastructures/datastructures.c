#include "datastructures.h"

// Growable array
Array make_array()
{
    /* Allocates and initialises an Array */
    Array retarray = calloc(1, sizeof(struct Array));

    HANDLEMALLOCERR(retarray, RETARRAY);

    retarray->buffer     = calloc(ARRAY_BUFFER_SIZE, sizeof(void *));
    retarray->buffer_len = ARRAY_BUFFER_SIZE;
    retarray->index      = 0;

    HANDLEMALLOCERR(retarray->buffer, RETARRAY_BUF);

    return retarray;
}

void array_add(Array array, void* val)
{
   /* Adds an element to an array
    *
    * Parameters
    * ----------
    * - Array array: the array to add the element to
    * - void* val:   the value to add to the array
    * 
    * Behaviour
    * ---------
    * As the only possible error is in realloc potentially failing, array_add()
    * will panic if the error is encountered rather than returning an error code.
    */
    if (array->index >= array->buffer_len) {
        void** new_buffer = realloc(array->buffer, array->buffer_len + ARRAY_BUFFER_GROW);

        HANDLEMALLOCERR(new_buffer, RETARRAY_REALLOC);

        free(array->buffer);
        array->buffer = new_buffer;
    }

    array->buffer[array->index++] = val;
}

void* array_index(Array array, int index)
{
   /* Safely gets the element at index 'index' from array 'array'
    *
    * Behaviour
    * ---------
    * If the index is invalid, array_index panics. This will probably be modified
    * in the future so that this error can be recovered from.
    */
    if (index < 0 || array->index < index) {
        WSEPRINTMESG(
            "internal error 0x%x - attempt to get %dth index of list length %d\n",
            INDEX_OUT_OF_BOUNDS,
            index,
            array->index
        );
        exit(INDEX_OUT_OF_BOUNDS);
    } else {
        return array->buffer[index];
    }
}

bool array_contains_string(Array array, char* string, size_t len)
{
   /* Checks if a string is in an array.
    *
    * Parameters
    * ----------
    * - Array array:  the array to check for the string in.
    * - char* string: the string to look for.
    * - size_t len:   the length of the string.
    * 
    * Returns
    * -------
    * Whether or not the string is in the array.
    */
    for (int i = 0; i < array->index; i++) {
        if (!strncmp(array->buffer[i], string, len)) {
            return true;
        }
    }

    return false;
}
