#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "datastructures.h"
#include "lexer.h"

int main() {
    // Placeholder for a real frontent
    FILE *fp  = fopen("../examples/factorial.woah", "r");
    char *buf = calloc(0x10000, sizeof(char));
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
