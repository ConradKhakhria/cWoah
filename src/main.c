#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "enums.h"
#include "datastructures/datastructures.h"
#include "lexer.h"

char *get_filename(int argc, char *argv[])
{
    for (int arg_index = 0; arg_index < argc; arg_index++) {
        int end_index  = 0;

        while (argv[arg_index][end_index] != '\x00') {
            end_index++;
        }

        if (!strncmp(".woah", &argv[arg_index][end_index - 5], 5)) {
            return argv[arg_index];
        }
    }

    fprintf(stderr, "Woah: error: no files supplied\n");
    exit(1);
}

int get_file_contents(char *filename, char *contents_buffer)
{
    FILE *fp = fopen(filename, "r");
    char c;
    int i = 0;

    if (fp == NULL) {
        perror("Woah: error opening file");
        exit(errno);
    } else if (contents_buffer == NULL) {
        perror("Woah: malloc error");
        exit(errno);
    }

    while ((c = getc(fp)) != EOF) {
        contents_buffer[i++] = c;
    }

    return i;
}

// Prints a string between start and end
void printn(char *array, int start, int end)
{
    for (; start < end; start++) {
        printf("%c", array[start]);
    }

    printf("'\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Woah: error: no files supplied.\n");
        exit(1);
    }

    char *filename          = get_filename(argc, argv);
    char *contents_buffer   = calloc(0x10000, sizeof(char));
    int contents_buffer_len = get_file_contents(filename, contents_buffer);

    Array tokens = generate_tokens(contents_buffer, contents_buffer_len);
    struct Token *tok;

    for (int i = 0; i < tokens->index; i++) {
        tok = tokens->buffer[i];
        printf("token type = %d, str = '", tok->token_type);
        printn(contents_buffer, tok->start_i, tok->end_i);
    }

}
