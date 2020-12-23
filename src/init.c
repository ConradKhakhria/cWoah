/* This file contains information that the compiler needs to use during
 * initialisation, including a list of predefined types, utilities for
 * getting word-sizes as well as typeclasses in the future.
 */

#include "init.h"

// This string contains the names of all built-in types.
static char keyword_string[1000] = \
    // signed integers
    "i8"      "\x0a"\
    "i16"     "\x0a"\
    "i32"     "\x0a"\
    "i64"     "\x0a"\
    "i128"    "\x0a"\
    "int"     "\x0a"\
    // unsigned integers
    "u8"      "\x0a"\
    "u16"     "\x0a"\
    "u32"     "\x0a"\
    "u64"     "\x0a"\
    "u128"    "\x0a"\
    "uint"    "\x0a"\
    // floating point numbers
    "f8"      "\x0a"\
    "f16"     "\x0a"\
    "f32"     "\x0a"\
    "f64"     "\x0a"\
    "f128"    "\x0a"\
    "float"   "\x0a"\
    // character (same as u8)
    "char"    "\x0a"\
    // Keywords
    "and"     "\x0a"\
    "elif"    "\x0a"\
    "else"    "\x0a"\
    "false"   "\x0a"\
    "fn"      "\x0a"\
    "for"     "\x0a"\
    "globals" "\x0a"\
    "heap"    "\x0a"\
    "if"      "\x0a"\
    "in"      "\x0a"\
    "macro"   "\x0a"\
    "none"    "\x0a"\
    "not"     "\x0a"\
    "null"    "\x0a"\
    "or"      "\x0a"\
    "return"  "\x0a"\
    "self"    "\x0a"\
    "stack"   "\x0a"\
    "struct"  "\x0a"\
    "true"    "\x0a"\
    "type"    "\x0a"\
    "use"     "\x0a"\
    "while"   "\x0a"\
    "xor";

void get_keywords()
{
   /* This fills inbuilt_types with the strings in type_string. */
    int slice_start = 0;
    int slice_end;

    for (int i = 0; i < KEYWORD_COUNT - 1; i++) {
        slice_end = slice_start;

        while (keyword_string[slice_end] != '\x0a') {
            slice_end += 1;
        }

        keywords[i] = &keyword_string[slice_start];
        keyword_string[slice_end]   = '\x00';

        slice_start = slice_end + 1;
    }

    keywords[KEYWORD_COUNT - 1] = &keyword_string[slice_start];
}
