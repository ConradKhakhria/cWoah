#include "datastructures.h"

// Growable array
Array make_array(size_t buffer_size)
{
    Array retarray = calloc(1, sizeof(struct Array));

    HANDLEMALLOCERR(retarray, 2);

    retarray->buffer     = calloc(ARRAY_BUFFER_SIZE, sizeof(void *));
    retarray->buffer_len = ARRAY_BUFFER_SIZE;
    retarray->index      = 0;

    HANDLEMALLOCERR(retarray->buffer, 3);

    return retarray;
}

void array_add(Array array,  void *val)
{
    if (array->index >= array->buffer_len) {
        void **new_buffer = realloc(array->buffer, array->buffer_len + ARRAY_BUFFER_GROW);

        HANDLEMALLOCERR(new_buffer, 4);

        free(array->buffer);
        array->buffer = new_buffer;
    }

    array->buffer[array->index++] = val;
}

void *array_index(Array array, int index)
{
    if (index < 0 || array->index < index) {
        WSEL1(
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

bool array_contains_string(Array array, char *string, size_t len)
{
    for (int i = 0; i < array->index; i++) {
        if (!strncmp(array->buffer[i], string, len)) {
            return true;
        }
    }

    return false;
}
