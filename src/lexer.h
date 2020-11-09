#pragma once

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "constants.h"
#include "datastructures/datastructures.h"
#include "enums.h"
#include "woah_error.h"

Array generate_tokens(char *source, int source_len);


int eq_type(char *source, int index);

int lt_type(char *source, int index);

int gt_type(char *source, int index);

int add_type(char *source, int index);

int hyp_type(char *source, int index);

int ast_type(char *source, int index);

int fwd_slash_type(char *source, int index);
