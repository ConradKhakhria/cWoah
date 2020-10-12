#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "datastructures.h"
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

int main(int argc, char *argv[]) {
    // Placeholder for a real frontent
    if (argc < 2) {
        fprintf(stderr, "Woah: error: no files supplied.\n");
        exit(1);
    }

    char *filename = get_filename(argc, argv);
    char *buf = calloc(0x10000, sizeof(char));
    FILE *fp  = fopen(filename, "r");
    char c;
    int buf_i = 0;

    if (fp == NULL) {
        perror("Woah: error opening file");
        exit(errno);
    } else if (buf == NULL) {
        perror("Woah: malloc error");
        exit(errno);
    }

    while ((c = getc(fp)) != EOF) {
        buf[buf_i++] = c;
    }

    Array tokens = generate_tokens(buf, buf_i);

    for (int i = 0; i < tokens->index; i++) {
        struct Token *tok = tokens->buffer[i];
        printf("Line number = %d, string = '", tok->line_no);

        for (int j = tok->start_i; j < tok->end_i; j++) {
            printf("%c", buf[j]);
        }

        printf("', token code = %d\n", tok->token_type);
    }
}
