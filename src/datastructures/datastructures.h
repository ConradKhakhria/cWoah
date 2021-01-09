#pragma once

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "../constants.h"
#include "../enums.h"
#include "../woah_error.h"

/* Generic datastructures */

// The growable array type, containing both the size of the buffer
// and the highest occupied index.
typedef struct Array {
    void** buffer;
    int buffer_len;
    int index;
}* Array;

Array make_array();

void array_add(Array array, void* val);

void* array_index(Array array, int index);

bool array_contains_string(Array array, char* string, size_t len);

/* Lexing and parsing */

struct Token {
    int token_type; /* As defined in /src/enums.h */
    int start_i;    /* The index of the token's first character in the source file */
    int end_i;      /* The index of the token's last character */
    int line_no;    /* The line number of the token */
    int col_no;     /* The column number of the token's firdst character */
};

/* The use of each field defined by the type form:  
 *
 * type form = TF_LIST: T[]
 *  - derivs = struct WType: T
 *  - num    = junk value
 *  - name   = "" ""
 *
 *  TF_POINTER: &T
 *  - derivs = struct WType: T
 *  - num    = junk
 *  - name   = ""  ""
 *
 *  TF_PARAMETRIC: T<x, y, ..>
 *  - derivs = struct WType[]: [x, y, ...]
 *  - num    = len(derivs)
 *  - name   = token for the name
 *
 *  TF_STRUCT: struct foo
 *  - derivs = (struct Token *)name_token
 *  - num    = junk
 *  - name   = ""   ""
 *
 *  TF_ATOMIC: T
 *  - derivs = junk
 *  - num    = numeric TokenType value
 *  - name   = if num is a built-in type, junk. Otherwise, the token's name.
 */
struct WType {
    struct Token* name;
    int           type_form; /* defined in /src/enums.h */
    int           num;
    void*         derivs;
};

/* Top level parsing structures */

struct WFunction {
    struct Token*       function_name; /* The token for the function's name. */
    struct WTypedef*    parent_type;   /* For if the function is a method (NULL if not) */
    struct Token**      arg_names;     /* The (ordered) list of names of the arguments. */
    struct WType**      arg_types;     /* The corresponding types of each arguments. */
    struct WType*       ret_type;      /* The return type (NULL if none) of the function. */
    struct WParseExpr*  body;          /* The function body's list of statements. */
    uint_fast32_t       arg_count;     /* The number of arguments. */
    uint_fast32_t       body_len;      /* The length of the function body array. */
};

struct WStruct {
    struct Token*  struct_name; /* Name of the struct. */
    struct Token** field_names; /* Names of the struct's fields. */
    struct WType** field_types; /* Types of the struct's fields. */
    uint_fast32_t  field_count; /* The number of fields the struct has. */
};

struct WTypedef {
    struct Token* typedef_name;     /* The name of the defined type. */
    void*         type_definition;  /* The contents of the defined type. */
};

struct WModuleImport {
    struct Token*  import_name;      /* The name of the imported module. */
    struct Token** imported_fields;  /* What specifically has been imported. */
    uint_fast32_t  import_type;      /* How the module has been imported. */
    uint_fast32_t  fields_count;     /* The number of fields */
};

struct WModuleExport {
    struct Token*  module_name;  /* The name of the exported module */
    struct Token** export_names; /* The functions to be exported */
    uint_fast32_t  export_count; /* The number of exported functions */
};

struct WGlobals {
    struct Token** variable_names; /* Names of the global variables  */
    struct WType** variable_types; /* Types of the global variables  */
    uint_fast32_t  variable_count; /* The number of global variables */
};

/**** Parsing statements within a function ****/

/* Nested expressions */

struct FunctionCall {
    struct Token* function_name;
    struct ParseExpr* parent_expr;
    struct ParseExpr** argument_exprs;
    int argument_count;
};

struct ListIndex {
    struct Token* list_name;
    struct ParseExpr* index;
};

// This will become more useful as the macros become more well-defined
struct MacroUse {
    int macro_name;           /* Whether it's heap![], stack![] or cast![] */
    struct ParseExpr* derivs; /* General purpose internal ParseExpr* */
};

// E.g. library.function(..), struct_name.field_name, type.method(..) etc
struct AttrResolution {
    bool is_pointer;                /* foo.bar or foo->bar */
    struct ParseExpr* parent_attr;  /* [parent_name].something */
    struct ParseExpr* child_attr;   /* something.[attribute_name] */
};

struct ParseExpr {
    uint_fast32_t type;           /* The type of expression. */
    uint_fast32_t value;          /* Multi-purpose integer */
    union {
        struct Token*     atom;       /* Variable name, true or false, numeric literal etc */
        struct ParseExpr** derivs;    /* Elements of a compound expression */
        struct FunctionCall call;     /* For if it's a function call */
        struct ListIndex list_index;  /* If a list is being indexed. */
        struct MacroUse macro_use;    /* if it's heap![], stack![] or cast![] */
        struct AttrResolution at_res; /* If it's a method call, struct field etc */
    } expression;
};

/* Statements */

struct VarDeclare {
    struct Token* var_name; /* The name of the declared variable */
    struct WType* var_type; /* The type of the declared variable */
    void* var_definition;   /* The variable's value (NULL if not assigned) */
};

struct VarAssign {
    struct Token* var_name; /* The name of the variable being assigned. */
    void* var_new_value;    /* The variable's new value. */
};

struct IfStatement {
    uint_fast32_t       type; /* Whether it's "if", "elif" or "else" */
    struct BoolExpr*    cond; /* The boolean conditional statement */
    struct WParseExpr** statements; /* The statements in the if block */
    uint_fast32_t       stmt_count; /* The number of statements */
};

struct ForLoop {

};

struct WhileLoop {

};

struct WParseExpr {
    int expression_type;
    union {
        struct VarDeclare*  decl;
        struct VarAssign*   assign;
        struct IfStatement* if_stmt;
        struct ForLoop*     for_loop;
        struct WhileLoop*   while_loop;
    } expression;
};
