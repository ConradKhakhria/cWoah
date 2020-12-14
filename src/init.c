/* This file contains information that the compiler needs to use during
 * initialisation, including a list of predefined types, utilities for
 * getting word-sizes as well as typeclasses in the future.
 */

#include "init.h"

// This string contains the names of all built-in types.
static char type_string[1000] = \
    // signed integers
    "i8"    "\x0a"\
    "i16"   "\x0a"\
    "i32"   "\x0a"\
    "i64"   "\x0a"\
    "i128"  "\x0a"\
    "int"   "\x0a"\
    // unsigned integers
    "u8"    "\x0a"\
    "u16"   "\x0a"\
    "u32"   "\x0a"\
    "u64"   "\x0a"\
    "u128"  "\x0a"\
    "uint"  "\x0a"\
    // floating point numbers
    "f8"    "\x0a"\
    "f16"   "\x0a"\
    "f32"   "\x0a"\
    "f64"   "\x0a"\
    "f128"  "\x0a"\
    "float" "\x0a"\
    // character (same as u8)
    "char";

void get_inbuilt_types()
{
   /* This fills inbuilt_types with the strings in type_string. */
    int slice_start = 0;
    int slice_end;

    for (int i = 0; i < 18; i++) {
        slice_end = slice_start;

        while (type_string[slice_end] != '\x0a') {
            slice_end += 1;
        }

        inbuilt_types[i] = &type_string[slice_start];
        type_string[slice_end]   = '\x00';

        slice_start = slice_end + 1;
    }

    inbuilt_types[18] = &type_string[slice_start];
}
