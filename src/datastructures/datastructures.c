#include "datastructures.h"

// Growable array
Array makeArray(size_t buffer_size)
{
    Array retarray = calloc(1, sizeof(struct Array));

    retarray->buffer     = calloc(ARRAY_BUFFER_SIZE, sizeof(void *));
    retarray->buffer_len = ARRAY_BUFFER_SIZE;
    retarray->index      = 0;

    if (retarray == NULL || retarray->buffer == NULL) {
        perror("Woah: malloc error");
        exit(errno);
    }

    return retarray;
}

void arrayAdd(Array array,  void *val)
{
    if (array->index >= array->buffer_len) {
        void **new_buffer = realloc(array->buffer, array->buffer_len + ARRAY_BUFFER_GROW);

        if (new_buffer == NULL) {
            perror("Woah: realloc error");
            exit(errno);
        }

        free(array->buffer);
        array->buffer = new_buffer;
    }

    array->buffer[array->index++] = val;
}

void *arrayIndex(Array array, int index)
{
    if (index < 0 || array->index < index) {
        fprintf(stderr, "Woah: index %d out of range [0, %d]\n", index, array->index);
        exit(INDEX_ERROR);
    } else {
        return array->buffer[index];
    }
}