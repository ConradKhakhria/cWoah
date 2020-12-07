#pragma once

// Various size parameters
#define ARRAY_BUFFER_SIZE 0x1000
#define ARRAY_BUFFER_GROW 0x200
#define TOKENS_ARRAY_SIZE 0x1000

// These are here so that they can be used globally throughout the program
// (read-only except for in /src/main.c)
char* program_source_buffer;
char* filename;
