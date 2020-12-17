/* This file contains information that the compiler needs to use during
 * initialisation, including a list of predefined types, utilities for
 * getting word-sizes as well as typeclasses in the future.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"

#define INBUILT_TYPES_COUNT 19

void get_inbuilt_types();
