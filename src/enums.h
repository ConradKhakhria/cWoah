#pragma once

enum TokenType {
    // Single characters
    T_NEWLINE,
    T_OPEN_BRKT,
    T_CLOSE_BRKT,
    T_OPEN_SQ_BRKT,
    T_CLOSE_SQ_BRKT,
    T_OPEN_CURLY_BRKT,
    T_CLOSE_CURLY_BRKT,
    T_COMMA,
    T_FULL_STOP,
    T_COLON,
    T_SEMICOLON,
    T_AMPERSAND,
    T_AT_SYM,
    T_EQUALS,
    T_LT,   // <
    T_GT,   // >
    T_FWD_SLASH,
    T_BACKSLASH,
    T_PLUS,
    T_MINUS,
    T_ASTERISK,
    T_DBL_QUOT, // "
    T_SGL_QUOT, // '

    // Operators etc
    T_EQ,   // ==
    T_INEQ, // !=
    T_LEQ,  // <=
    T_GEQ,  // >=
    T_SHL,  // <<
    T_SHR,  // >>
    T_ADD_ASSIGN,
    T_SUB_ASSIGN,
    T_MUL_ASSIGN,
    T_DIV_ASSIGN,
    T_INC,      // ++
    T_DEC,      // --
    T_ARROW,    // ->
    T_SWAP,     // <-> (swap operator)

    // Numbers
    T_B2NUM,
    T_B10NUM,
    T_B16NUM,

    // Names
    T_NAME,

    // Keywords
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

enum TypeForm {
    TF_LIST,        // T[]
    TF_POINTER,     // &T
    TF_PARAMETRIC,  // T<x, y, ...>
    TF_ATOMIC,      // T
};
