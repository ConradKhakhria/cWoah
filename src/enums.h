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
    T_WHILE,
    T_FOR,
    T_IF,
    T_ELIF,
    T_ELSE,
    T_USE,
    T_IN,
};

enum ASTnodetypes {
    N_PROG,
    N_FUNC,
    N_STRUCT,
    N_CODE,
    N_DECL,
    N_ASSIGN,
    N_DECL_ASSIGN,
    N_IF,
    N_ELIF,
    N_ELSE,
    N_WHILE,
    N_FOR,
    N_FUNCALL,
    N_EXPRESSION,
    N_SEMICOLON,
    N_EOF
};
