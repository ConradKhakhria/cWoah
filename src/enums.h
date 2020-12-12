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
    T_EQUALS,           // =
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
    T_EQ,               // ==
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

    // Keywords (all of the following are just the name but in lower case)
    T_FN,
    T_STRUCT,
    T_USE,
    T_MACRO,
    T_GLOBALS,
    T_WHILE,
    T_FOR,
    T_IF,
    T_ELIF,
    T_ELSE,
    T_IN,
    T_SELF,
    T_RETURN,
    T_STACK,
    T_HEAP,
    T_AND,
    T_OR,
    T_XOR,
    T_TRUE,
    T_FALSE,
    T_NOT,
};

/*  type form = TF_LIST: T[]
    - derivs = struct WType: T
    - num    = junk value

    TF_POINTER: &T
    - derivs = struct WType: T
    - num    = junk

    TF_PARAMETRIC: T<x, y, ..>
    - derivs = struct WType[]: [x, y, ...]
    - num    = len(derivs)

    TF_STRUCT: struct foo
    - derivs = (struct Token *)name_token
    - num    = junk

    TF_ATOMIC: T
    - derivs = junk
    - num    = index in the list of types
*/
enum TypeForm {
    TF_LIST,        // T[]
    TF_POINTER,     // &T
    TF_PARAMETRIC,  // T<x, y, ...>
    TF_STRUCT,      // struct foo
    TF_ATOMIC,      // T
};

enum MallocErrorCodes {
    SOURCE_BUF,
    RETARRAY,
    RETARRAY_BUF,
    RETARRAY_REALLOC,
    PARAMETRIC_TYPE_DERIVS,
    TYPE_STRUCT
};
