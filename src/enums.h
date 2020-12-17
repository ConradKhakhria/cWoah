#pragma once

// All types of token (for use in struct Token)
enum TokenType {
    // Single characters
    T_NEWLINE,
    T_OPEN_BRKT,        // (
    T_CLOSE_BRKT,       // )
    T_OPEN_SQ_BRKT,     // [
    T_CLOSE_SQ_BRKT,    // ]
    T_OPEN_CURLY_BRKT,  // {
    T_CLOSE_CURLY_BRKT, // }
    T_COMMA,            // ,
    T_FULL_STOP,        // .
    T_COLON,            // :
    T_SEMICOLON,        // ;
    T_AMPERSAND,        // &
    T_AT_SYM,           // @
    T_SGL_EQUALS,       // =
    T_LT,               // <
    T_GT,               // >
    T_FWD_SLASH,        // /
    T_BACKSLASH,        // (the reverse of the above)
    T_PLUS,             // +
    T_MINUS,            // -
    T_ASTERISK,         // *
    T_DBL_QUOT,         // "
    T_SGL_QUOT,         // '

    // Operators
    T_DBL_EQUALS,       // ==
    T_INEQ,             // !=
    T_LEQ,              // <=
    T_GEQ,              // >=
    T_ADD_ASSIGN,       // +=
    T_SUB_ASSIGN,       // -=
    T_MUL_ASSIGN,       // *=
    T_DIV_ASSIGN,       // /=
    T_INC,              // ++
    T_DEC,              // --
    T_ARROW,            // ->
    T_SWAP,             // <->

    // Numbers
    T_B2NUM,            // -?0b[01]+
    T_B10NUM,           // -?[0-9]+
    T_B16NUM,           // -?(0x|0X)[0-9a-fA-F]+
    T_FLOAT,            // -?[0-9]*\.[0-9]+

    // Names
    T_NAME,             // [a-zA-Z_][a-zA-Z0-9_]*

    // Keywords (all of the following are just the name but in upper case)
    T_AND,
    T_ELIF,
    T_ELSE,
    T_FALSE,
    T_FN,
    T_FOR,
    T_GLOBALS,
    T_HEAP,
    T_IF,
    T_IN,
    T_MACRO,
    T_NOT,
    T_OR,
    T_RETURN,
    T_SELF,
    T_STACK,
    T_STRUCT,
    T_TRUE,
    T_TYPE,
    T_USE,
    T_WHILE,
    T_XOR
};

enum TypeForm {
    TF_LIST,        // T[]
    TF_POINTER,     // &T
    TF_PARAMETRIC,  // T<x, y, ...>
    TF_STRUCT,      // struct foo
    TF_ATOMIC,      // T
};

enum ModuleImportType {
    MT_QUALIFIED,
    MT_SPECIFIC,
    MT_ALL
};

enum MallocErrorCodes {
    MAIN_PROGRAM_SOURCE_BUFFER,
    MAKE_ARRAY_INIT_STRUCT,
    MAKE_ARRAY_INIT_BUFFER,
    ARRAY_ADD_BUFFER_REALLOC,
    LEXER_ALLOC_TOKEN,
    PARAMETRIC_TYPE_DERIVS,
    TYPE_STRUCT,
    COLLECT_BLOCK_FUNCTION_STRUCT,
    COLLECT_BLOCK_FUNCTION_ARG_NAMES,
    COLLECT_BLOCK_FUNCTION_ARG_TYPES,
    COLLECT_BLOCK_FUNCTION_NAMES_REALLOC,
    COLLECT_BLOCK_FUNCTION_TYPES_REALLOC,
    COLLECT_BLOCK_STRUCT_STRUCT,
    COLLECT_BLOCK_STRUCT_FIELD_NAMES,
    COLLECT_BLOCK_STRUCT_FIELD_TYPES,
    COLLECT_BLOCK_STRUCT_NAMES_REALLOC,
    COLLECT_BLOCK_STRUCT_TYPES_REALLOC,
    COLLECT_BLOCK_IMPORT_STRUCT,
    COLLECT_BLOCK_IMPORT_LIST_ALLOC,
    COLLECT_BLOCK_GLOBALS_STRUCT,
    COLLECT_BLOCK_GLOBALS_NAMES,
    COLLECT_BLOCK_GLOBALS_TYPES
};
